#include "bbt/appkit/module.h"

namespace bbt {

Status ModuleManager::RegisterModule(const bbt_module_t* m) {
  if (!m || !m->name || !*m->name || !m->version || !m->init)
    return InvalidArgumentError("");

  return OkStatus();
}
}  // namespace bbt