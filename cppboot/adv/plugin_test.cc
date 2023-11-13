#include "cppboot/adv/plugin_manager.h"

#include "gmock/gmock.h"

using namespace cppboot;

using ::testing::ElementsAre;

static int plugin_init(const char* param) {
  if (param) {
    int ret = atoi(param);
    return ret;
  }
  return 0;
}

static void plugin_exit(void) { return; }

struct PluginLoadTest {
  const char* errmsg;
  StatusCode result;
  struct cppboot_plugin hdr;
  const char* param;
};

class MockPluginLoader : public PluginLoader {
 public:
  MockPluginLoader(PluginLoadTest* tests, size_t count)
      : tests_(tests), count_(count) {}

  Status Load(const char* name, struct cppboot_plugin** result) {
    if (name) {
      for (size_t i = 0; i < count_; i++) {
        struct cppboot_plugin* hdr = &tests_[i].hdr;
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

TEST(Plugin, LoadAndUnload) {
  // Setup
  PluginLoadTest cases[] = {
      {
          "case1",
          StatusCode::kOk,
          {"mod1", "", plugin_init, plugin_exit},
      },
      // name
      {
          "case2: name == NULL",
          StatusCode::kInvalidArgument,
          {NULL, NULL, plugin_init, plugin_exit},
      },
      {
          "case3: name is empty",
          StatusCode::kInvalidArgument,
          {"", NULL, plugin_init, plugin_exit},
      },
      {
          "case4: duplicated name",
          StatusCode::kAlreadyExists,
          {"mod1", NULL, plugin_init, plugin_exit},
      },
      // init and exit
      {
          "case7: no init",
          StatusCode::kInvalidArgument,
          {"mod7", NULL, NULL, plugin_exit},
      },
      {
          "case8: init return error",
          StatusCode::kInvalidArgument,
          {"mod8", "", plugin_init, plugin_exit},
          "255",
      },
      {
          "case9: no exit",
          StatusCode::kInvalidArgument,
          {"mod9", NULL, plugin_init, NULL},
      },
      // requires
      {
          "case10: null require",
          StatusCode::kOk,
          {"mod10", NULL, plugin_init, plugin_exit},
      },
      {
          "case11: require others",
          StatusCode::kOk,
          {"mod11", "mod10,mod1", plugin_init, plugin_exit},
      },
      {
          "case12: require not exist",
          StatusCode::kNotFound,
          {"mod12", "mod999", plugin_init, plugin_exit},
      },
  };

  MockPluginLoader ldr(cases, CPPBOOT_ARRAYSIZE(cases));
  PluginManager* manager = PluginManager::New(&ldr);

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

  PluginManager::Release(manager);
}
