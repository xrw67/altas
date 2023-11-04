#include "cppboot/appkit/module.h"

#include <map>
#include <set>
#include <string>
#include <memory>
#include <mutex>

#include "cppboot/base/fmt.h"
#include "cppboot/base/str_util.h"

namespace cppboot {

namespace {

Status VerifyModuleHeader(const PCPPBOOT_MODULE_HEADER hdr) {
  if (!hdr) return InvalidArgumentError("no module");
  if (!hdr->name || !*hdr->name) return InvalidArgumentError("no name");
  if (!hdr->version || !*hdr->version)
    return InvalidArgumentError("no version");
  if (!hdr->init) return InvalidArgumentError("no init func");
  if (!hdr->exit) return InvalidArgumentError("no exit func");
  return OkStatus();
}

};  // namespace

struct Module;
typedef std::shared_ptr<Module> ModulePtr;

struct Module {
  const PCPPBOOT_MODULE_HEADER hdr;
  std::string param;
  std::set<ModulePtr> sources;  // What modules depend on me?
  std::set<ModulePtr> targets;  // What modules do I depend on?

  explicit Module(const PCPPBOOT_MODULE_HEADER hdr) : hdr(hdr) {}
};

class ModuleManagerImpl : public ModuleManager {
 public:
  ModuleManagerImpl(ModuleLoader* loader) : loader_(loader) {}

 private:
  ModuleLoader* loader_;

  std::mutex mutex_;
  std::map<std::string, ModulePtr> mods_;

  Status Load(const char* name, const char* param) {
    if (!name || !*name) return InvalidArgumentError("no module name");
    {
      std::lock_guard<std::mutex> lock(mutex_);
      if (mods_.find(name) != mods_.end()) return AlreadyExistsError(name);
    }

    PCPPBOOT_MODULE_HEADER hdr;
    Status st = loader_->Load(name, &hdr);
    if (!st.ok()) return st;

    ModulePtr me(new Module(hdr));

    if (!param) param = "";
    st = InitModule(me, hdr, param);
    if (!st.ok()) {
      loader_->Unload(name);
      return st;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    me->param = param;
    mods_[hdr->name] = me;
    return OkStatus();
  }

  Status Unload(const char* name) {
    if (!name || !*name) return InvalidArgumentError("empty module name");

    ModulePtr me;

    {
      std::lock_guard<std::mutex> lock(mutex_);
      auto it = mods_.find(name);
      if (it == mods_.end()) {
        return NotFoundError(name);
      }

      me = it->second;

      // 判断依赖关系
      if (!me->sources.empty())
        return CancelledError("has depend on by others");

      for (auto target : me->targets) {
        target->sources.erase(me);
      }
      mods_.erase(it);
    }

    me->hdr->exit();
    loader_->Unload(me->hdr->name);
    return OkStatus();
  }

  Status InitModule(ModulePtr& module, const PCPPBOOT_MODULE_HEADER hdr,
                    const char* param) {
    Status st = VerifyModuleHeader(hdr);
    if (st.ok()) {
      st = VerifyDependence(module);
      if (st.ok()) {
        int err = hdr->init(param);
        if (err) return InvalidArgumentError("init retrn error");
      }
    }
    return st;
  }

  Status VerifyDependence(ModulePtr& module) {
    if (!module->hdr->requires) return OkStatus();

    auto require_list = StrSplit(module->hdr->requires, ',');
    for (auto& i : require_list) {
      auto name = StrTrim(i, " ");
      if (name.empty()) continue;
      auto it = mods_.find(to_string(name));
      if (it == mods_.end())
        return NotFoundError(format("depend on {}", name.data()));
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
};

ModuleManager* ModuleManager::New(ModuleLoader* loader) {
  return new ModuleManagerImpl(loader);
}
void ModuleManager::Release(ModuleManager* mgr) { delete mgr; }

}  // namespace cppboot