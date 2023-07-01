#include <gmock/gmock.h>
#include "bbt/appkit/module.h"

using namespace bbt;

TEST(Plugin, DISABLED_DllPluginLoader) {
  // setup
  ModuleLoader* ldr = ModuleLoader::New(ModuleLoader::Type::kDll, "./build/");
  ModuleManager* manager = ModuleManager::New(ldr);

  ASSERT_EQ(manager->Load("hello", NULL).code(), StatusCode::kOk);

  manager->Unload("hello");

  // teardown
  ModuleManager::Release(manager);
  ModuleLoader::Release(ldr);
}