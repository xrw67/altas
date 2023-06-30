#include "bbt/appkit/module.h"

#include <map>
#include <string>

namespace bbt {

class ModuleManagerImpl : public ModuleManager {
 public:
  ModuleManagerImpl() {}
  ~ModuleManagerImpl() {}

  Status LoadModule(bbt_module_t* mod) {
    if (!mod || !mod->name || !*mod->name || !mod->version || !mod->init)
      return InvalidArgumentError("");

    if (mods_.find(mod->name) != mods_.end()) return AlreadyExistsError("");

    int err = mod->init("");
    if (err) return IOError("");

    mods_[mod->name] = mod;
    return OkStatus();
  }

  Status UnloadModule(string_view mod_name) {
    // TODO: string_view 到 string的自动转换。
    auto it = mods_.find(mod_name.data());

    if (it == mods_.end()) {
      return NotFoundError(mod_name);
    } else {
      if (it->second->exit)
        it->second->exit();
      else
        return NotSupportedError(mod_name);

      mods_.erase(it);
      return OkStatus();
    }
  }

 private:
  typedef std::map<std::string, bbt_module_t*> ModuleMap;

  ModuleMap mods_;
};

ModuleManager* CreateModuleManager() { return new ModuleManagerImpl(); }
void ReleaseModuleManager(ModuleManager* mod_mgr) { delete mod_mgr; }

}  // namespace bbt