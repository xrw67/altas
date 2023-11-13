#ifndef CPPBOOT_BASE_LOG_H_
#define CPPBOOT_BASE_LOG_H_

#include <string>
#include "cppboot/base/attributes.h"
#include "cppboot/base/optimization.h"
#include "cppboot/base/fmt.h"

//
// 高层次的打印，可以使用各种花样
//
#define CPPBOOT_LOG(severity, ...)                                             \
  do {                                                                         \
    constexpr const char* cppboot_logging_internal_basename =                  \
        ::cppboot::logging_internal::Basename(__FILE__, sizeof(__FILE__) - 1); \
    ::cppboot::logging_internal::Log(CPPBOOT_LOGGING_INTERNAL_##severity,      \
                                     cppboot_logging_internal_basename,        \
                                     __LINE__, __VA_ARGS__);                   \
  } while (0)

#define CPPBOOT_CHECK(condition, message)                                 \
  do {                                                                    \
    if (CPPBOOT_PREDICT_FALSE(!(condition))) {                            \
      CPPBOOT_LOG(FATAL, "Check {:s} failed: {:s}", #condition, message); \
    }                                                                     \
  } while (0)

//
// 低层次的打印，实现非常简单，通常仅仅输出到STDERR
//
#define CPPBOOT_RAW_LOG(severity, ...)                                         \
  do {                                                                         \
    constexpr const char* cppboot_logging_internal_basename =                  \
        ::cppboot::logging_internal::Basename(__FILE__, sizeof(__FILE__) - 1); \
    ::cppboot::logging_internal::RawLog(CPPBOOT_LOGGING_INTERNAL_##severity,   \
                                        cppboot_logging_internal_basename,     \
                                        __LINE__, __VA_ARGS__);                \
  } while (0)

#define CPPBOOT_RAW_CHECK(condition, message)                                 \
  do {                                                                        \
    if (CPPBOOT_PREDICT_FALSE(!(condition))) {                                \
      CPPBOOT_RAW_LOG(FATAL, "Check {:s} failed: {:s}", #condition, message); \
    }                                                                         \
  } while (0)

#define CPPBOOT_LOGGING_INTERNAL_DEBUG ::cppboot::LogSeverity::kDebug
#define CPPBOOT_LOGGING_INTERNAL_INFO ::cppboot::LogSeverity::kInfo
#define CPPBOOT_LOGGING_INTERNAL_WARNING ::cppboot::LogSeverity::kWarning
#define CPPBOOT_LOGGING_INTERNAL_ERROR ::cppboot::LogSeverity::kError
#define CPPBOOT_LOGGING_INTERNAL_FATAL ::cppboot::LogSeverity::kFatal

namespace cppboot {

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

// Internal logging function for CPPBOOT_LOG to dispatch to.
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
}  // namespace cppboot

#endif  // CPPBOOT_BASE_LOG_H_
