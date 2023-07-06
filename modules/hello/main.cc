#include "bbt/appkit/module.h"
#include "bbt/base/logging.h"

int hello_init(const char* param) {
  BBT_LOG(INFO, "Hello init");
  return 0;
}

void hello_exit(void) { BBT_LOG(INFO, "Hello exit"); }

extern "C" __attribute__((visibility("default")))
BBT_MODULE_HEADER bbt_module_header = {
    .name = "hello",
    .version = "0.1.0",
    .requires = "",
    .init = hello_init,
    .exit = hello_exit,
};