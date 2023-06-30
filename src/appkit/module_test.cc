#include "bbt/appkit/module.h"

#include <gmock/gmock.h>

using namespace bbt;

//
// Test Module
//

static int mod_init_ok(const char* param) { return 0; }
static int mod_init_fail(const char* param) { return 1; }
static int mod_init_with_param(const char* param) {
  int ret = atoi(param);
  return ret;
}

static void mod_exit(void) { return; }

TEST(Module, LoadAndUnload) {
  Status st;
  ModuleManager* mm = CreateModuleManager();

  // Setup

  struct register_testcase {
    const char* errmsg;
    StatusCode result;
    bbt_module_t mod;
    const char* param;
  } cases[] = {
      {
          "case1",
          StatusCode::kOk,
          {"test1", 1001, "", mod_init_ok, mod_exit},
      },
      {
          "case2: wothout exit function",
          StatusCode::kOk,
          {"test2", 1001, NULL, mod_init_ok, NULL},
      },
      {
          "case3: without name",
          StatusCode::kInvalidArgument,
          {NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case4: empty name",
          StatusCode::kInvalidArgument,
          {"", 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case5: duplicated name",
          StatusCode::kAlreadyExists,
          {"test1", 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case6: invalid version",
          StatusCode::kInvalidArgument,
          {"test6", 0, NULL, mod_init_ok, mod_exit},
      },
      {
          "case7: without init function",
          StatusCode::kInvalidArgument,
          {"test7", 1, NULL, NULL, mod_exit},
      },
      {
          "case8",
          StatusCode::kIOError,
          {"test8", 1001, "", mod_init_fail, mod_exit},
      },
      {
          "case8: with param",
          StatusCode::kIOError,
          {"test8", 1001, "", mod_init_with_param, mod_exit},
          "255",
      },
  };

  // Load

  for (auto& i : cases) {
    st = mm->LoadModule(&i.mod, i.param);
    ASSERT_EQ(st.code(), i.result) << i.errmsg;
  }

  st = mm->LoadModule(NULL, NULL);
  ASSERT_EQ(st.code(), StatusCode::kInvalidArgument);

  // Unload
  ASSERT_TRUE(IsInvalidArgument(mm->UnloadModule(NULL)));
  ASSERT_TRUE(IsInvalidArgument(mm->UnloadModule("")));
  ASSERT_TRUE(mm->UnloadModule("test1").ok());
  ASSERT_TRUE(IsNotFound(mm->UnloadModule("test1")));
  ASSERT_TRUE(IsNotSupported(mm->UnloadModule("test2")));

  ReleaseModuleManager(mm);
}

// TEST(Module, Relation) {}