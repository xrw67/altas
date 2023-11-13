#include "cppboot/adv/module.h"

#include "gmock/gmock.h"

using namespace cppboot;

using ::testing::ElementsAre;

static int mod_init(const char* param) {
  if (param) {
    int ret = atoi(param);
    return ret;
  }
  return 0;
}

static void mod_exit(void) { return; }

struct PluginLoadTest {
  const char* errmsg;
  StatusCode result;
  CPPBOOT_MODULE_HEADER hdr;
  const char* param;
};

class MockPluginLoader : public ModuleLoader {
 public:
  MockPluginLoader(PluginLoadTest* tests, size_t count)
      : tests_(tests), count_(count) {}

  Status Load(const char* name, PCPPBOOT_MODULE_HEADER* result) {
    if (name) {
      for (size_t i = 0; i < count_; i++) {
        PCPPBOOT_MODULE_HEADER hdr = &tests_[i].hdr;
        if (hdr->name && std::strcmp(tests_[i].hdr.name, name) == 0) {
          *result = hdr;
          return OkStatus();
        }
      }
    }
    return NotFoundError("");
  }

  Status Unload(const char* name) { return OkStatus(); }

 private:
  PluginLoadTest* tests_;
  size_t count_;
};

TEST(Module, LoadAndUnload) {
  // Setup
  PluginLoadTest cases[] = {
      {
          "case1",
          StatusCode::kOk,
          {"mod1", "1.0.0", "", mod_init, mod_exit},
      },
      // name
      {
          "case2: name == NULL",
          StatusCode::kInvalidArgument,
          {NULL, "1.0.0", NULL, mod_init, mod_exit},
      },
      {
          "case3: name is empty",
          StatusCode::kInvalidArgument,
          {"", "1.0.0", NULL, mod_init, mod_exit},
      },
      {
          "case4: duplicated name",
          StatusCode::kAlreadyExists,
          {"mod1", "1.0.0", NULL, mod_init, mod_exit},
      },
      // version
      {
          "case5: version = NULL",
          StatusCode::kInvalidArgument,
          {"mod5", NULL, NULL, mod_init, mod_exit},
      },
      {
          "case6: invalid version",
          StatusCode::kInvalidArgument,
          {"mod6", "", NULL, mod_init, mod_exit},
      },
      // init and exit
      {
          "case7: no init",
          StatusCode::kInvalidArgument,
          {"mod7", "1.0.0", NULL, NULL, mod_exit},
      },
      {
          "case8: init return error",
          StatusCode::kInvalidArgument,
          {"mod8", "1.0.0", "", mod_init, mod_exit},
          "255",
      },
      {
          "case9: no exit",
          StatusCode::kInvalidArgument,
          {"mod9", "1.0.0", NULL, mod_init, NULL},
      },
      // requires
      {
          "case10: null require",
          StatusCode::kOk,
          {"mod10", "1.0.0", NULL, mod_init, mod_exit},
      },
      {
          "case11: require others",
          StatusCode::kOk,
          {"mod11", "1.0.0", "mod10,mod1", mod_init, mod_exit},
      },
      {
          "case12: require not exist",
          StatusCode::kNotFound,
          {"mod12", "1.0.0", "mod999", mod_init, mod_exit},
      },
  };

  MockPluginLoader ldr(cases, CPPBOOT_ARRAYSIZE(cases));
  ModuleManager* manager = ModuleManager::New(&ldr);

  // Load
  {
    ASSERT_TRUE(IsInvalidArgument(manager->Load(NULL, NULL)))
        << "load null mod";

    for (auto& i : cases) {
      ASSERT_EQ(manager->Load(i.hdr.name, i.param).code(), i.result)
          << i.errmsg;
    }
  }

  // Unload
  {
    ASSERT_EQ(manager->Unload(NULL).code(), StatusCode::kInvalidArgument)
        << "null mod";
    ASSERT_EQ(manager->Unload("").code(), StatusCode::kInvalidArgument)
        << "empty mod name";
    ASSERT_EQ(manager->Unload("mod1").code(), StatusCode::kCancelled);
    ASSERT_EQ(manager->Unload("mod11").code(), StatusCode::kOk);
    ASSERT_EQ(manager->Unload("mod1").code(), StatusCode::kOk);
    ASSERT_EQ(manager->Unload("mod1").code(), StatusCode::kNotFound);
    ASSERT_EQ(manager->Unload("mod9").code(), StatusCode::kNotFound);
  }

  ModuleManager::Release(manager);
}
