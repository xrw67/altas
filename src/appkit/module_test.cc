#include "bbt/appkit/module.h"

#include <gmock/gmock.h>

using namespace bbt;

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
  BBT_MODULE_HEADER hdr;
  const char* param;
};

class MockPluginLoader : public ModuleLoader {
 public:
  MockPluginLoader(PluginLoadTest* tests, size_t count)
      : tests_(tests), count_(count) {}

  Status Load(const char* name, PBBT_MODULE_HEADER* result) {
    if (name) {
      for (size_t i = 0; i < count_; i++) {
        PBBT_MODULE_HEADER hdr = &tests_[i].hdr;
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
          {"plugin1", "1.0.0", "", plugin_init, plugin_exit},
      },
      // name
      {
          "case2: name == NULL",
          StatusCode::kInvalidArgument,
          {NULL, "1.0.0", NULL, plugin_init, plugin_exit},
      },
      {
          "case3: name is empty",
          StatusCode::kInvalidArgument,
          {"", "1.0.0", NULL, plugin_init, plugin_exit},
      },
      {
          "case4: duplicated name",
          StatusCode::kAlreadyExists,
          {"plugin1", "1.0.0", NULL, plugin_init, plugin_exit},
      },
      // version
      {
          "case5: version = NULL",
          StatusCode::kInvalidArgument,
          {"plugin5", NULL, NULL, plugin_init, plugin_exit},
      },
      {
          "case6: invalid version",
          StatusCode::kInvalidArgument,
          {"plugin6", "", NULL, plugin_init, plugin_exit},
      },
      // init and exit
      {
          "case7: no init",
          StatusCode::kInvalidArgument,
          {"plugin7", "1.0.0", NULL, NULL, plugin_exit},
      },
      {
          "case8: init return error",
          StatusCode::kInvalidArgument,
          {"plugin8", "1.0.0", "", plugin_init, plugin_exit},
          "255",
      },
      {
          "case9: no exit",
          StatusCode::kOk,
          {"plugin9", "1.0.0", NULL, plugin_init, NULL},
      },
      // requires
      {
          "case10: null require",
          StatusCode::kOk,
          {"plugin10", "1.0.0", NULL, plugin_init, plugin_exit},
      },
      {
          "case11: require others",
          StatusCode::kOk,
          {"plugin11", "1.0.0", "plugin10,plugin1", plugin_init, plugin_exit},
      },
      {
          "case12: require not exist",
          StatusCode::kNotFound,
          {"plugin12", "1.0.0", "plugin999", plugin_init, plugin_exit},
      },
  };

  MockPluginLoader ldr(cases, BBT_ARRAYSIZE(cases));
  ModuleManager* manager = ModuleManager::New(&ldr);

  // Load
  {
    ASSERT_TRUE(IsInvalidArgument(manager->Load(NULL, NULL)))
        << "load null plugin";

    for (auto& i : cases) {
      ASSERT_EQ(manager->Load(i.hdr.name, i.param).code(), i.result)
          << i.errmsg;
    }
  }

  // Unload
  {
    ASSERT_EQ(manager->Unload(NULL).code(), StatusCode::kInvalidArgument)
        << "null plugin";
    ASSERT_EQ(manager->Unload("").code(), StatusCode::kInvalidArgument)
        << "empty plugin name";
    ASSERT_EQ(manager->Unload("plugin1").code(), StatusCode::kCancelled);
    ASSERT_EQ(manager->Unload("plugin11").code(), StatusCode::kOk);
    ASSERT_EQ(manager->Unload("plugin1").code(), StatusCode::kOk);
    ASSERT_EQ(manager->Unload("plugin1").code(), StatusCode::kNotFound);
    ASSERT_EQ(manager->Unload("plugin9").code(), StatusCode::kUnimplemented);
  }

  ModuleManager::Release(manager);
}

