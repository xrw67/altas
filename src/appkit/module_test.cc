#include "bbt/appkit/module.h"

#include <gmock/gmock.h>

using namespace bbt;

//
// Test Module
//

static int mod_init_ok(const char* param) { return 0; }
static int mod_init_with_param(const char* param) {
  int ret = atoi(param);
  return ret;
}

static void mod_exit(void) { return; }

TEST(Module, LoadAndUnload) {
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
          {"mod1", 1001, "", mod_init_ok, mod_exit},
      },
      {
          "case2: must have exit function",
          StatusCode::kInvalidArgument,
          {"mod2", 1001, NULL, mod_init_ok, NULL},
      },
      {
          "case3: name == NULL",
          StatusCode::kInvalidArgument,
          {NULL, 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case4: name is empty",
          StatusCode::kInvalidArgument,
          {"", 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case5: duplicated name",
          StatusCode::kAlreadyExists,
          {"mod1", 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case6: invalid version",
          StatusCode::kInvalidArgument,
          {"mod6", 0, NULL, mod_init_ok, mod_exit},
      },
      {
          "case7: must have init function",
          StatusCode::kInvalidArgument,
          {"mod7", 1, NULL, NULL, mod_exit},
      },
      {
          "case8: init return error",
          StatusCode::kIOError,
          {"mod8", 1001, "", mod_init_with_param, mod_exit},
          "255",
      },
      {
          "case9: null require",
          StatusCode::kOk,
          {"mod9", 1001, NULL, mod_init_ok, mod_exit},
      },
      {
          "case10: require others",
          StatusCode::kOk,
          {"mod10", 1, "mod9,mod1", mod_init_ok, mod_exit},
      },
      {
          "case11: require not exist",
          StatusCode::kNotFound,
          {"mod11", 1, "mod999", mod_init_ok, mod_exit},
      },
  };

  ModuleManager* mm = CreateModuleManager();

  // Load
  {
    for (auto& i : cases) {
      ASSERT_EQ(mm->LoadModule(&i.mod, i.param).code(), i.result) << i.errmsg;
    }

    ASSERT_TRUE(IsInvalidArgument(mm->LoadModule(NULL, NULL)))
        << "load null module";
  }

  // Unload
  {
    ASSERT_TRUE(IsInvalidArgument(mm->UnloadModule(NULL))) << "null module";
    ASSERT_TRUE(IsInvalidArgument(mm->UnloadModule(""))) << "empty module name";
    ASSERT_TRUE(mm->UnloadModule("mod1").ok());
    ASSERT_TRUE(IsNotFound(mm->UnloadModule("mod1")));
    ASSERT_TRUE(mm->UnloadModule("mod9").ok());
  }

  {
    std::vector<bbt_module_t*> mods = mm->ListModules();
    ASSERT_EQ(mods.size(), 1);
    ASSERT_STREQ(mods[0]->name, "mod10");
  }

  ReleaseModuleManager(mm);
}

// class ModuleLoader {

// }

// TEST(Module, CycleRelation) {
//     struct bbt_module_t mods[] = {
//         {"mod1", 1, "mod3", mod_init_ok, mod_exit},
//         {"mod2", 1, "mod1", mod_init_ok, mod_exit},
//         {"mod3", 1, "mod2", mod_init_ok, mod_exit},
//     };
//     ModuleLoader ldr();
//     ModuleManager* mm = CreateModuleManager();

//     ldr->LoadAllModules(mods)
// }
