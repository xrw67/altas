#include "bbt/appkit/module.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>

#include "bbt/base/str_util.h"
namespace bbt {

struct Module {
  bbt_module_t* obj;

  std::string param;  // 保存一份，调试用

  std::string depends;
  // TODO
  // std::set<std::string> source_list;
  // std::set<std::string> target_list;

  Module(bbt_module_t* m, const char* param) : obj(m), param(param) {
    InitDepends(m->depends);
  }

  void InitDepends(const char* depends) {
    if (!depends || !*depends) return;
    this->depends = depends;
  }
};

class ModuleManagerImpl : public ModuleManager {
 public:
  ModuleManagerImpl() {}
  ~ModuleManagerImpl() {}

 private:
  Status LoadModule(bbt_module_t* mod, const char* param);
  Status UnloadModule(const char* mod_name);
  std::vector<bbt_module_t*> ListModules();

  typedef std::map<std::string, std::shared_ptr<Module>> ModuleMap;

  ModuleMap mods_;
};

ModuleManager* CreateModuleManager() { return new ModuleManagerImpl(); }
void ReleaseModuleManager(ModuleManager* mod_mgr) { delete mod_mgr; }

Status CheckModule(bbt_module_t* mod) {
  if (!mod) return InvalidArgumentError("no module");
  if (!mod->name || !*mod->name) return InvalidArgumentError("name is empty");
  if (!mod->version) return InvalidArgumentError("version is 0");
  if (!mod->init) return InvalidArgumentError("no init func");
  if (!mod->exit) return InvalidArgumentError("no exit func");

  return OkStatus();
}

Status ModuleManagerImpl::LoadModule(bbt_module_t* mod, const char* param) {
  Status st = CheckModule(mod);
  if (!st.ok()) return st;
  if (mods_.find(mod->name) != mods_.end())
    return AlreadyExistsError(mod->name);

  // check depends
  if (mod->depends) {
    std::vector<std::string> depend_list = StrSplit(mod->depends, ',');
    for (auto& i : depend_list) {
      if (mods_.find(i) == mods_.end()) {
        return NotFoundError("depend out found");
      }
    }
  }

  if (!param) param = "";

  int err = mod->init(param);
  if (err) return IOError("");

  // insert and save
  std::shared_ptr<Module> m(new Module(mod, param));
  mods_[mod->name] = m;
  return OkStatus();
}

Status ModuleManagerImpl::UnloadModule(const char* mod_name) {
  if (!mod_name || !*mod_name) return InvalidArgumentError("empty module name");

  auto it = mods_.find(mod_name);
  if (it == mods_.end()) {
    return NotFoundError(mod_name);
  }

  // 判断依赖关系

  std::shared_ptr<Module> m(it->second);
  if (!m->obj->exit) return NotSupportedError(mod_name);

  m->obj->exit();

  mods_.erase(it);
  return OkStatus();
}

std::vector<bbt_module_t*> ModuleManagerImpl::ListModules() {
  std::vector<bbt_module_t*> result;

  for (auto& it : mods_) {
    result.push_back(it.second->obj);
  }
  return result;
}
}  // namespace bbt