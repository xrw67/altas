#ifndef BBT_APPKIT_MODULE_H_
#define BBT_APPKIT_MODULE_H_

#include <vector>
#include <string>

#include "bbt/base/config.h"
#include "bbt/base/status.h"

#ifdef __cplusplus
extern "C" {
#endif

// 插件头信息
typedef struct BBT_EXPORT _BBT_MODULE_HEADER {
  const char* name;                // 插件名称
  const char* version;             // 插件版本号
  const char* requires;            // 依赖的其他插件
  int (*init)(const char* param);  // 插件初始化函数
  void (*exit)(void);              // 插件退出函数
} BBT_MODULE_HEADER, *PBBT_MODULE_HEADER;

#ifdef __cplusplus
}
#endif

namespace bbt {

class ModuleLoader {
 public:
  enum Type {
    kDll = 1,
  };

  virtual ~ModuleLoader() {}

  static ModuleLoader* New(Type type, const char* dir);
  static void Release(ModuleLoader* ldr);

  virtual Status Load(const char* name, PBBT_MODULE_HEADER* result) = 0;
  virtual Status Unload(const char* name) = 0;
};

class ModuleManager {
 public:
  virtual ~ModuleManager() {}

  static ModuleManager* New(ModuleLoader* loader);
  static void Release(ModuleManager* mgr);

  virtual Status Load(const char* name, const char* param) = 0;
  virtual Status Unload(const char* name) = 0;
  virtual std::vector<std::string> List() = 0;
};

}  // namespace bbt

#endif  // BBT_APPKIT_MODULE_H_
