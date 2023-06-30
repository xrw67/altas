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

class ModuleManager final {
 public:
  Status RegisterModule(const bbt_module_t* m);

 private:
};

}  // namespace bbt
#endif  // BBT_APPKIT_MODULE_H_
