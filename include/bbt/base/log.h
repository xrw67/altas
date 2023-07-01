#ifndef BBT_BASE_LOG_H_
#define BBT_BASE_LOG_H_

enum class LogSeverity : int {
  kInfo = 0,
  kWarning = 1,
  kError = 2,
  kFatal = 3,
};

#define BBT_LOGGING_INFO ::bbt::LogSeverity::kInfo
#define BBT_LOGGING_WARNING ::bbt::LogSeverity::kWarning
#define BBT_LOGGING_ERROR ::bbt::LogSeverity::kError
#define BBT_LOGGING_FATAL ::bbt::LogSeverity::kFatal

#define BBT_RAW_LOG(severity, ...)
#define BBT_RAW_CHECK(condition, message)

#define BBT_LOG(severity, ...)

#endif  // BBT_BASE_LOG_H_
