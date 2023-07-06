#include "bbt/appkit/module.h"

#include <map>
#include <set>
#include <string>
#include <memory>

#include <dlfcn.h>

#include "bbt/base/str_util.h"
#include "bbt/base/logging.h"
#include "bbt/base/fs.h"

namespace bbt {

namespace {

Status VerifyModuleHeader(const PBBT_MODULE_HEADER hdr) {
  if (!hdr) return InvalidArgumentError("no module");
  if (!hdr->name || !*hdr->name) return InvalidArgumentError("no name");
  if (!hdr->version || !*hdr->version)
    return InvalidArgumentError("no version");
  if (!hdr->init) return InvalidArgumentError("no init func");
  return OkStatus();
}

};  // namespace

struct Module;
typedef std::shared_ptr<Module> ModulePtr;

struct Module {
  const PBBT_MODULE_HEADER hdr;
  std::string param;
  std::set<ModulePtr> sources;  // What modules depend on me?
  std::set<ModulePtr> targets;  // What modules do I depend on?

  explicit Module(const PBBT_MODULE_HEADER hdr) : hdr(hdr) {}
};

class ModuleManagerImpl : public ModuleManager {
 public:
  ModuleManagerImpl(ModuleLoader* loader) : loader_(loader) {}

 private:
  ModuleLoader* loader_;
  std::map<std::string, ModulePtr> mods_;

  Status Load(const char* name, const char* param) {
    if (!name) {
      return InvalidArgumentError("no module name");
    }

    PBBT_MODULE_HEADER hdr;
    Status st = loader_->Load(name, &hdr);
    if (st.ok()) {
      ModulePtr me(new Module(hdr));
      if (!param) param = "";

      st = RunEntryPoint(me, hdr, param);
      if (st.ok()) {
        me->param = param;
        mods_[hdr->name] = me;
        return OkStatus();
      }
    }

    // fail
    loader_->Unload(name);
    return st;
  }

  Status Unload(const char* name) {
    if (!name || !*name) return InvalidArgumentError("empty module name");

    auto it = mods_.find(name);
    if (it == mods_.end()) {
      return NotFoundError(name);
    }

    auto me = it->second;
    if (!me->hdr->exit) return UnimplementedError("no exit function");

    // 判断依赖关系
    if (!me->sources.empty()) return CancelledError("has depend on by others");

    for (auto target : me->targets) {
      target->sources.erase(me);
    }

    me->hdr->exit();
    loader_->Unload(me->hdr->name);
    mods_.erase(it);
    return OkStatus();
  }

  std::vector<std::string> List() {
    std::vector<std::string> result;
    for (auto& it : mods_) result.push_back(it.first);
    return result;
  }

  bool IsExist(const std::string& name) const {
    return (mods_.find(name) != mods_.end());
  }

  Status ParseRequires(ModulePtr& module) {
    if (!module->hdr->requires) return OkStatus();

    auto require_list = StrSplit(module->hdr->requires, ',');
    for (auto& i : require_list) {
      std::string name = StrTrim(i, " ");
      if (name.empty()) continue;
      auto it = mods_.find(name);
      if (it == mods_.end())
        return NotFoundError(StrPrintf("depend on %s", name.c_str()));
      else
        AddModuleUsage(module, it->second);
    }
    return OkStatus();
  }

  // Module a uses b
  void AddModuleUsage(ModulePtr& a, ModulePtr& b) {
    b->sources.insert(a);
    a->targets.insert(b);
  }

  Status RunEntryPoint(ModulePtr& module, const PBBT_MODULE_HEADER hdr,
                       const char* param) {
    Status st = VerifyModuleHeader(hdr);
    if (!st.ok()) return st;
    if (IsExist(hdr->name)) return AlreadyExistsError(hdr->name);

    // 依赖关系
    if (hdr->exit) {
      st = ParseRequires(module);
      if (!st.ok()) return st;
    }

    int err = hdr->init(param);
    if (err) return InvalidArgumentError("init retrn error");

    return OkStatus();
  }
};

ModuleManager* ModuleManager::New(ModuleLoader* loader) {
  return new ModuleManagerImpl(loader);
}
void ModuleManager::Release(ModuleManager* mgr) { delete mgr; }

//
// DLL Module Loader
//

class DllLoader : public ModuleLoader {
 public:
  DllLoader(const char* dir) : dir_(dir) {}
  ~DllLoader() {
    for (auto h : handles_) {
      if (::dlclose(h.second)) {
        BBT_RAW_LOG(ERROR, "unload module %s failed: %s", h.first.c_str(),
                    dlerror());
      }
    }
  }

 private:
  Status Load(const char* name, PBBT_MODULE_HEADER* result) {
    if (!name || !*name) {
      return InvalidArgumentError("no name");
    }
    if (!result) {
      return InvalidArgumentError("invalid result");
    }

    auto path = GetModulePath(name);
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) {
      return CancelledError(
          StrPrintf("load module %s failed: %s", name, dlerror()));
    }

    PBBT_MODULE_HEADER hdr =
        (PBBT_MODULE_HEADER)::dlsym(handle, "bbt_module_header");
    if (!hdr) {
      ::dlclose(handle);
      return CancelledError(StrPrintf(
          "load module %s failed: no symbol bbt_module_header", name));
    }

    handles_[name] = handle;
    *result = hdr;
    return OkStatus();
  }

  virtual Status Unload(const char* name) {
    if (!name || !*name) return InvalidArgumentError("no name");

    auto it = handles_.find(name);
    if (it != handles_.end()) {
      if (::dlclose(it->second)) {
        return UnknownError(::dlerror());
      }
      handles_.erase(it);
      return OkStatus();
    }
    return NotFoundError(name);
  }

  std::string GetModulePath(const char* name) {
    return PathJoin(dir_, StrCat("libmod_", name, ".so"));
  }

 private:
  std::string dir_;
  std::map<std::string, void*> handles_;
};

ModuleLoader* ModuleLoader::New(ModuleLoader::Type type, const char* dir) {
  switch (type) {
    case kDll:
      return new DllLoader(dir ? dir : "");
    default:
      BBT_ASSERT(true);
      return NULL;
  }
}

void ModuleLoader::Release(ModuleLoader* loader) { delete loader; }

}  // namespace bbt