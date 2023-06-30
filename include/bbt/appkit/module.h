#ifndef BBT_APPKIT_MODULE_H_
#define BBT_APPKIT_MODULE_H_

#include "bbt/base/config.h"
#include "bbt/base/status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct BBT_EXPORT bbt_module {
  const char* name;
  const char* description;
  unsigned int version;
  const char* depends;
  int (*init)(const char* param);
  void (*exit)(void);
};

typedef struct bbt_module bbt_module_t;

#ifdef __cplusplus
}
#endif

namespace bbt {

class ModuleManager {
 public:
  virtual ~ModuleManager() {}
  virtual Status RegisterModule(bbt_module_t* m) = 0;
};

ModuleManager* CreateModuleManager();
void DeleteModuleManager(ModuleManager* mod_mgr);

}  // namespace bbt
#endif  // BBT_APPKIT_MODULE_H_
