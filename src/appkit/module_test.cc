#include "bbt/appkit/module.h"

#include <gmock/gmock.h>

using namespace bbt;

// TODO
//
// 定义一个插件对象
// 加载插件
//  正常的插件
//  缺少字段的插件

//  卸载插件
//      没有退出函数的插件，无法卸载

// Builtin函数不在ModuleManager的管辖范围内，只是借用一下插件的架构而已。

//
// Test Module
//

static int mod_init_ok(const char* param) { return 0; }
static int mod_init_fail(const char* param) { return 1; }
static void mod_exit(void) { return; }

TEST(Module, ModuleObject) {
  Status st;
  ModuleManager mm;

  struct register_testcase {
    StatusCode result;
    bbt_module_t mod;
  } cases[] = {
      {
          StatusCode::kOk,
          {"test1", "test1 desc", 1001, "", mod_init_ok, mod_exit},
      },
      {
          StatusCode::kOk,
          {"test2", NULL, 1001, NULL, mod_init_ok, NULL},
      },
      {
          StatusCode::kInvalidArgument,
          {NULL, NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          StatusCode::kInvalidArgument,
          {"", NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      // 名字重复
      // TODO： 定一个新的StatusCode？
      {
          StatusCode::kInvalidArgument,
          {"test1", NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          StatusCode::kInvalidArgument,
          {"test4", NULL, 0, NULL, mod_init_ok, mod_exit},
      },
      {
          StatusCode::kInvalidArgument,
          {"test5", NULL, 1, NULL, NULL, mod_exit},
      },

      // init失败
      // TODO: 添加一个StatusCode?
      {
          StatusCode::kIOError,
          {"test6", "test6 desc", 1001, "", mod_init_fail, mod_exit},
      },
  };

  for (auto& i : cases) {
    st = mm.RegisterModule(&i.mod);
    ASSERT_EQ(st.code(), i.result);
  }

  st = mm.RegisterModule(NULL);
  ASSERT_EQ(st.code(), StatusCode::kInvalidArgument);
}