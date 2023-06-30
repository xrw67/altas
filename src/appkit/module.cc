#include "bbt/appkit/module.h"

#include <map>
#include <set>
#include <string>
#include <memory>

namespace bbt {

struct Module {
  std::string name;
  std::string
      depends;  // TODO 使用下面的source_list（require_list， depend_by_list?）
  int version;
  // int (*init)(const char* param);
  void (*exit)(void);

  std::string param;  // 保存一份，调试用

  // TODO
  // std::set<std::string> source_list;
  // std::set<std::string> target_list;

  Module(bbt_module_t* m, const char* param)
      : name(m->name), version(m->version), exit(m->exit), param(param) {
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

  return OkStatus();
}

Status ModuleManagerImpl::LoadModule(bbt_module_t* mod, const char* param) {
  Status st = CheckModule(mod);
  if (!st.ok()) return st;
  if (mods_.find(mod->name) != mods_.end())
    return AlreadyExistsError(mod->name);
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
  if (!m->exit) return NotSupportedError(mod_name);

  m->exit();

  mods_.erase(it);
  return OkStatus();
}

}  // namespace bbt