#include "bbt/base/logging.h"
#include <gmock/gmock.h>

namespace bbt {

char msg_buffer[3000];

void mock_log_func(LogSeverity severity, const char* file, int line,
                   const char* message, int size) {
  sprintf(msg_buffer, "[%s %s] %s", LogSeverityName(severity), file, message);
}

TEST(Loggin, Print) {
  RegisterInternalLogFunction(mock_log_func);

  BBT_LOG(DEBUG, "it is %s msg", "debug");
  ASSERT_STREQ(msg_buffer, "[DEBUG logging_test.cc] it is debug msg");
  BBT_LOG(INFO, "it is %s msg", "info");
  ASSERT_STREQ(msg_buffer, "[INFO logging_test.cc] it is info msg");
  BBT_LOG(WARNING, "it is %s msg", "warning");
  ASSERT_STREQ(msg_buffer, "[WARNING logging_test.cc] it is warning msg");
  BBT_LOG(ERROR, "it is %s msg", "error");
  ASSERT_STREQ(msg_buffer, "[ERROR logging_test.cc] it is error msg");
}

}  // namespace bbt