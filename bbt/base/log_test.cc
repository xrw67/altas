#include "bbt/base/log.h"
#include "gmock/gmock.h"

#include "bbt/base/fmt.h"

namespace bbt {

char msg_buffer[3000];

void mock_log_func(LogSeverity severity, const char* file, int line,
                   const std::string& message) {
  snprintf(msg_buffer, sizeof(msg_buffer), "[%s %s] %s",
           LogSeverityName(severity), file, message.c_str());
}

TEST(Logging, Print) {
  RegisterInternalLogFunction(mock_log_func);

  BBT_LOG(DEBUG, "it is {} msg", "debug");
  ASSERT_STREQ(msg_buffer, "[DEBUG log_test.cc] it is debug msg");
  BBT_LOG(INFO, "it is {} msg", "info");
  ASSERT_STREQ(msg_buffer, "[INFO log_test.cc] it is info msg");
  BBT_LOG(WARNING, "it is {} msg", "warning");
  ASSERT_STREQ(msg_buffer, "[WARNING log_test.cc] it is warning msg");
  BBT_LOG(ERROR, "it is {} msg", "error");
  ASSERT_STREQ(msg_buffer, "[ERROR log_test.cc] it is error msg");
}

}  // namespace bbt