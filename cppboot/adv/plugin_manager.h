#ifndef CPPBOOT_ADV_PLUGIN_MANAGER_H_
#define CPPBOOT_ADV_PLUGIN_MANAGER_H_

#include <vector>
#include <string>

#include "cppboot/base/config.h"
#include "cppboot/base/status.h"

#include "cppboot/adv/plugin.h"

namespace cppboot {

class PluginLoader {
 public:
  enum Type {
    kDll = 1,
  };

  virtual ~PluginLoader() {}

  static PluginLoader* New(Type type, const char* dir);
  static void Release(PluginLoader* ldr);

  virtual Status Load(const char* name, struct cppboot_plugin** result) = 0;
  virtual Status Unload(const char* name) = 0;
};

class PluginManager {
 public:
  virtual ~PluginManager() {}

  static PluginManager* New(PluginLoader* loader);
  static void Release(PluginManager* mgr);

  virtual Status Load(const char* name, const char* param) = 0;
  virtual Status Unload(const char* name) = 0;
};

}  // namespace cppboot

#endif  // CPPBOOT_ADV_PLUGIN_MANAGER_H_
