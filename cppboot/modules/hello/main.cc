#include "cppboot/adv/module.h"
#include "cppboot/base/log.h"

int hello_init(const char* param) {
  CPPBOOT_LOG(INFO, "Hello init");
  return 0;
}

void hello_exit(void) { CPPBOOT_LOG(INFO, "Hello exit"); }

extern "C" __attribute__((visibility("default")))
CPPBOOT_MODULE_HEADER cppboot_module_header = {
    .name = "hello",
    .version = "0.1.0",
    .requires = "",
    .init = hello_init,
    .exit = hello_exit,
};