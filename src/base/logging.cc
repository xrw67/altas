#include "bbt/base/logging.h"

#include <cstdio>
#include <cstdarg>

namespace bbt {

namespace {
constexpr int kLogBufSize = 3000;

void null_log_func(LogSeverity severity, const char* file, int line,
                   const char* message, int size) {
  // nothing
  return;
}

InternalLogFunction internal_log_function = null_log_func;

}  // namespace

void RegisterInternalLogFunction(InternalLogFunction func) {
  internal_log_function = (func);
}

namespace logging_internal {

void RawLogVA(LogSeverity severity, const char* file, int line,
              const char* format, va_list ap) {
  char buf[kLogBufSize];
  int size = sizeof(buf);

  int n = vsnprintf(buf, size, format, ap);
  internal_log_function(severity, file, line, buf, n);
}

void RawLog(LogSeverity severity, const char* file, int line,
            const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  RawLogVA(severity, file, line, format, ap);
  va_end(ap);
}

void Log(LogSeverity severity, const char* file, int line, const char* format,
         ...) {
  va_list ap;
  va_start(ap, format);
  RawLogVA(severity, file, line, format, ap);
  va_end(ap);
}

}  // namespace logging_internal
}  // namespace bbt