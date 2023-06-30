#include "bbt/appkit/module.h"

#include <map>
#include <string>

namespace bbt {

class ModuleManagerImpl : public ModuleManager {
 public:
  ModuleManagerImpl() {}
  ~ModuleManagerImpl() {}

  Status RegisterModule(bbt_module_t* m) {
    if (!m || !m->name || !*m->name || !m->version || !m->init)
      return InvalidArgumentError("");

    if (mods_.find(m->name) != mods_.end()) return AlreadyExistsError("");

    int err = m->init("");
    if (err) return IOError("");

    mods_[m->name] = m;
    return OkStatus();
  }

 private:
  std::map<std::string, bbt_module_t*> mods_;
};

ModuleManager* CreateModuleManager() { return new ModuleManagerImpl(); }
void DeleteModuleManager(ModuleManager* mod_mgr) { delete mod_mgr; }

}  // namespace bbt