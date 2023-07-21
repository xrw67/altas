#include "bbt/base/log.h"

#include <cstdio>

namespace bbt {

namespace {

void null_log_func(LogSeverity severity, const char* file, int line,
                   const std::string& message) {
  // nothing
  return;
}

InternalLogFunction internal_log_function = null_log_func;

}  // namespace

void RegisterInternalLogFunction(InternalLogFunction func) {
  internal_log_function = (func);
}

namespace logging_internal {

void CallRawLogHandler(LogSeverity severity, const char* file, int line,
                       const std::string& message) {
  internal_log_function(severity, file, line, message);
}

void CallLogHandler(LogSeverity severity, const char* file, int line,
                    const std::string& message) {
  internal_log_function(severity, file, line, message);
}

}  // namespace logging_internal
}  // namespace bbt