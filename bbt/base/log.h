#ifndef BBT_BASE_LOG_H_
#define BBT_BASE_LOG_H_

#include <string>
#include "bbt/base/attributes.h"
#include "bbt/base/optimization.h"
#include "bbt/base/fmt.h"

//
// 高层次的打印，可以使用各种花样
//
#define BBT_LOG(severity, ...)                                             \
  do {                                                                     \
    constexpr const char* bbt_logging_internal_basename =                  \
        ::bbt::logging_internal::Basename(__FILE__, sizeof(__FILE__) - 1); \
    ::bbt::logging_internal::Log(BBT_LOGGING_INTERNAL_##severity,          \
                                 bbt_logging_internal_basename, __LINE__,  \
                                 __VA_ARGS__);                             \
  } while (0)

#define BBT_CHECK(condition, message)                                 \
  do {                                                                \
    if (BBT_PREDICT_FALSE(!(condition))) {                            \
      BBT_LOG(FATAL, "Check {:s} failed: {:s}", #condition, message); \
    }                                                                 \
  } while (0)

//
// 低层次的打印，实现非常简单，通常仅仅输出到STDERR
//
#define BBT_RAW_LOG(severity, ...)                                           \
  do {                                                                       \
    constexpr const char* bbt_logging_internal_basename =                    \
        ::bbt::logging_internal::Basename(__FILE__, sizeof(__FILE__) - 1);   \
    ::bbt::logging_internal::RawLog(BBT_LOGGING_INTERNAL_##severity,         \
                                    bbt_logging_internal_basename, __LINE__, \
                                    __VA_ARGS__);                            \
  } while (0)

#define BBT_RAW_CHECK(condition, message)                                 \
  do {                                                                    \
    if (BBT_PREDICT_FALSE(!(condition))) {                                \
      BBT_RAW_LOG(FATAL, "Check {:s} failed: {:s}", #condition, message); \
    }                                                                     \
  } while (0)

#define BBT_LOGGING_INTERNAL_DEBUG ::bbt::LogSeverity::kDebug
#define BBT_LOGGING_INTERNAL_INFO ::bbt::LogSeverity::kInfo
#define BBT_LOGGING_INTERNAL_WARNING ::bbt::LogSeverity::kWarning
#define BBT_LOGGING_INTERNAL_ERROR ::bbt::LogSeverity::kError
#define BBT_LOGGING_INTERNAL_FATAL ::bbt::LogSeverity::kFatal

namespace bbt {

enum class LogSeverity : int {
  kDebug = 0,
  kInfo,
  kWarning,
  kError,
  kFatal,
};

// LogSeverityName()
//
// Returns the all-caps string representation (e.g. "INFO") of the specified
// severity level if it is one of the standard levels and "UNKNOWN" otherwise.
constexpr const char* LogSeverityName(LogSeverity s) {
  return s == LogSeverity::kDebug     ? "DEBUG"
         : s == LogSeverity::kInfo    ? "INFO"
         : s == LogSeverity::kWarning ? "WARNING"
         : s == LogSeverity::kError   ? "ERROR"
         : s == LogSeverity::kFatal   ? "FATAL"
                                      : "UNKNOWN";
}

// Internal logging function for BBT_LOG to dispatch to.
using InternalLogFunction = void (*)(LogSeverity severity, const char* file,
                                     int line, const std::string& message);

void RegisterInternalLogFunction(InternalLogFunction func);

namespace logging_internal {

// compile-time function to get the "base" filename, that is, the part of
// a filename after the last "/" or "\" path separator.  The search starts at
// the end of the string; the second parameter is the length of the string.
constexpr const char* Basename(const char* fname, int offset) {
  return offset == 0 || fname[offset - 1] == '/' || fname[offset - 1] == '\\'
             ? fname + offset
             : Basename(fname, offset - 1);
}

void CallRawLogHandler(LogSeverity severity, const char* file, int line,
                       const std::string& message);
void CallLogHandler(LogSeverity severity, const char* file, int line,
                    const std::string& message);

// Helper function to implement ABSL_RAW_LOG
// Logs format... at "severity" level, reporting it
// as called from file:line.
// This does not allocate memory or acquire locks.
template <typename... T>
void RawLog(LogSeverity severity, const char* file, int line,
            format_string<T...> fmt, T&&... args) {
  auto message = vformat(fmt, fmt::make_format_args(args...));
  CallRawLogHandler(severity, file, line, message);
}

template <typename... T>
void Log(LogSeverity severity, const char* file, int line,
         format_string<T...> fmt, T&&... args) {
  auto message = vformat(fmt, fmt::make_format_args(args...));
  CallLogHandler(severity, file, line, message);
}

}  // namespace logging_internal
}  // namespace bbt

#endif  // BBT_BASE_LOG_H_
