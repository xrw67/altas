#ifndef CPPBOOT_ADV_PLUGIN_H_
#define CPPBOOT_ADV_PLUGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

// 插件头信息
struct cppboot_plugin {
  const char* name;                // 插件名称
  const char* depends;             // 依赖的其他插件
  int (*init)(const char* param);  // 插件初始化函数
  void (*exit)(void);              // 插件退出函数
};

#ifdef CPPBOOT_PLUGIN_AS_DLL
#define CPPBOOT_PLUGIN(NAME, INIT, EXIT, DEPENDS) \
  struct cppboot_plugin cppboot_plugin_entry = {  \
      .name = #NAME,                              \
      .depends = DEPENDS,                         \
      .init = INIT,                               \
      .exit = EXIT,                               \
  }
#else
#define CPPBOOT_PLUGIN(NAME, INIT, EXIT, DEPENDS)         \
  struct cppboot_plugin cppboot_plugin_builtin_##NAME = { \
      .name = #NAME,                                      \
      .depends = DEPENDS,                                 \
      .init = INIT,                                       \
      .exit = EXIT,                                       \
  }
#endif

#ifdef __cplusplus
}
#endif

#endif  // CPPBOOT_ADV_PLUGIN_H_
