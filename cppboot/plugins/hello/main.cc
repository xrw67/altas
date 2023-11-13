#include "cppboot/adv/plugin.h"
#include "cppboot/base/log.h"

int hello_init(const char* param) {
  CPPBOOT_LOG(INFO, "Hello init");
  return 0;
}

void hello_exit(void) { CPPBOOT_LOG(INFO, "Hello exit"); }

CPPBOOT_PLUGIN(hello, hello_init, hello_exit, "");
