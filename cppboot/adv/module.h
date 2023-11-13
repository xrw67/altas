#ifndef CPPBOOT_ADV_MODULE_H_
#define CPPBOOT_ADV_MODULE_H_

#include <vector>
#include <string>

#include "cppboot/base/config.h"
#include "cppboot/base/status.h"

#ifdef __cplusplus
extern "C" {
#endif

// 插件头信息
typedef struct CPPBOOT_EXPORT _CPPBOOT_MODULE_HEADER {
  const char* name;                // 插件名称
  const char* version;             // 插件版本号
  const char* requires;            // 依赖的其他插件
  int (*init)(const char* param);  // 插件初始化函数
  void (*exit)(void);              // 插件退出函数
} CPPBOOT_MODULE_HEADER, *PCPPBOOT_MODULE_HEADER;

#ifdef __cplusplus
}
#endif

namespace cppboot {

class ModuleLoader {
 public:
  enum Type {
    kDll = 1,
  };

  virtual ~ModuleLoader() {}

  static ModuleLoader* New(Type type, const char* dir);
  static void Release(ModuleLoader* ldr);

  virtual Status Load(const char* name, PCPPBOOT_MODULE_HEADER* result) = 0;
  virtual Status Unload(const char* name) = 0;
};

class ModuleManager {
 public:
  virtual ~ModuleManager() {}

  static ModuleManager* New(ModuleLoader* loader);
  static void Release(ModuleManager* mgr);

  virtual Status Load(const char* name, const char* param) = 0;
  virtual Status Unload(const char* name) = 0;
};

}  // namespace cppboot

#endif  // CPPBOOT_ADV_MODULE_H_
