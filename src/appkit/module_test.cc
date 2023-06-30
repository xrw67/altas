#include "bbt/appkit/module.h"

#include <gmock/gmock.h>

using namespace bbt;

// TODO
//
// 定义一个插件对象
// 加载插件
//  正常的插件
//  缺少字段的插件
//  检查init参数param

//  卸载插件
//      没有退出函数的插件，无法卸载

// Builtin函数不在ModuleManager的管辖范围内，只是借用一下插件的架构而已。

//
// Test Module
//

static int mod_init_ok(const char* param) { return 0; }
static int mod_init_fail(const char* param) { return 1; }
static void mod_exit(void) { return; }

TEST(Module, LoadAndUnload) {
  Status st;
  ModuleManager* mm = CreateModuleManager();

  // Setup

  struct register_testcase {
    const char* errmsg;
    StatusCode result;
    bbt_module_t mod;

  } cases[] = {
      {
          "case1",
          StatusCode::kOk,
          {"test1", "test1 desc", 1001, "", mod_init_ok, mod_exit},
      },
      {
          "case2",
          StatusCode::kOk,
          {"test2", NULL, 1001, NULL, mod_init_ok, NULL},
      },
      {
          "case3",
          StatusCode::kInvalidArgument,
          {NULL, NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case4",
          StatusCode::kInvalidArgument,
          {"", NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      // 名字重复
      {
          "case5",
          StatusCode::kAlreadyExists,
          {"test1", NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case6",
          StatusCode::kInvalidArgument,
          {"test6", NULL, 0, NULL, mod_init_ok, mod_exit},
      },
      {
          "case7",
          StatusCode::kInvalidArgument,
          {"test7", NULL, 1, NULL, NULL, mod_exit},
      },

      // init失败
      // TODO: 添加一个StatusCode?
      {
          "case8",
          StatusCode::kIOError,
          {"test8", "test6 desc", 1001, "", mod_init_fail, mod_exit},
      },
  };

  // Load

  for (auto& i : cases) {
    st = mm->LoadModule(&i.mod);
    ASSERT_EQ(st.code(), i.result) << i.errmsg;
  }

  st = mm->LoadModule(NULL);
  ASSERT_EQ(st.code(), StatusCode::kInvalidArgument);

  // Unload
  ASSERT_TRUE(mm->UnloadModule("test1").ok());
  ASSERT_EQ(mm->UnloadModule("test1").code(), StatusCode::kNotFound);
  ASSERT_EQ(mm->UnloadModule("test2").code(), StatusCode::kNotSupported);

  ReleaseModuleManager(mm);
}
