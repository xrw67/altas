#include "bbt/util/status.h"

#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <ostream>
#include <vector>

#include "bbt/util/fmt.h"

namespace bbt {

std::string StatusCodeToString(StatusCode code) {
  switch (code) {
    case StatusCode::kOk:
      return "OK";
    case StatusCode::kCancelled:
      return "CANCELLED";
    case StatusCode::kUnknown:
      return "UNKNOWN";
    case StatusCode::kDeadlineExceeded:
      return "DEADLINE_EXCEEDED";
    case StatusCode::kInvalidArgument:
      return "INVALID_ARGUMENT";
    case StatusCode::kNotFound:
      return "NOT_FOUND";
    case StatusCode::kAlreadyExists:
      return "ALREADY_EXISTS";
    case StatusCode::kPermissionDenied:
      return "PERMISSION_DENIED";
    case StatusCode::kResourceExhausted:
      return "RESOURCE_EXHAUSTED";
    case StatusCode::kFailedPrecondition:
      return "FAILED_PRECONDITION";
    case StatusCode::kAborted:
      return "ABORTED";
    case StatusCode::kOutOfRange:
      return "OUT_OF_RANGE";
    case StatusCode::kUnimplemented:
      return "UNIMPLEMENTED";
    case StatusCode::kInternal:
      return "INTERNAL";
    case StatusCode::kUnavailable:
      return "UNAVAILABLE";
    default:
      return "";
  }
}

std::ostream& operator<<(std::ostream& os, StatusCode code) {
  return os << StatusCodeToString(code);
}

Status OkStatus() { return Status(); }

std::ostream& operator<<(std::ostream& os, const Status& x) {
  os << x.ToString();
  return os;
}

bool IsCancelled(const Status& status) {
  return status.code() == StatusCode::kCancelled;
}

bool IsUnknown(const Status& status) {
  return status.code() == StatusCode::kUnknown;
}

bool IsDeadlineExceeded(const Status& status) {
  return status.code() == StatusCode::kDeadlineExceeded;
}
bool IsInvalidArgument(const Status& status) {
  return status.code() == StatusCode::kInvalidArgument;
}

bool IsNotFound(const Status& status) {
  return status.code() == StatusCode::kNotFound;
}

bool IsAlreadyExists(const Status& status) {
  return status.code() == StatusCode::kAlreadyExists;
}

bool IsPermissionDenied(const Status& status) {
  return status.code() == StatusCode::kPermissionDenied;
}

bool IsResourceExhausted(const Status& status) {
  return status.code() == StatusCode::kResourceExhausted;
}

bool IsFailedPrecondition(const Status& status) {
  return status.code() == StatusCode::kFailedPrecondition;
}

bool IsAborted(const Status& status) {
  return status.code() == StatusCode::kAborted;
}

bool IsOutOfRange(const Status& status) {
  return status.code() == StatusCode::kOutOfRange;
}

bool IsUnimplemented(const Status& status) {
  return status.code() == StatusCode::kUnimplemented;
}

bool IsInternal(const Status& status) {
  return status.code() == StatusCode::kInternal;
}

bool IsUnavailable(const Status& status) {
  return status.code() == StatusCode::kUnavailable;
}

Status CancelledError(string_view message) {
  return Status(StatusCode::kCancelled, message);
}

Status UnknownError(string_view message) {
  return Status(StatusCode::kUnknown, message);
}

Status DeadlineExceededError(string_view message) {
  return Status(StatusCode::kDeadlineExceeded, message);
}
Status InvalidArgumentError(string_view message) {
  return Status(StatusCode::kInvalidArgument, message);
}

Status NotFoundError(string_view message) {
  return Status(StatusCode::kNotFound, message);
}

Status AlreadyExistsError(string_view message) {
  return Status(StatusCode::kAlreadyExists, message);
}

Status PermissionDeniedError(string_view message) {
  return Status(StatusCode::kPermissionDenied, message);
}

Status OutOfRangeError(string_view message) {
  return Status(StatusCode::kOutOfRange, message);
}

Status FailedPreconditionError(string_view message) {
  return Status(StatusCode::kFailedPrecondition, message);
}

Status ResourceExhaustedError(string_view message) {
  return Status(StatusCode::kResourceExhausted, message);
}

Status AbortedError(string_view message) {
  return Status(StatusCode::kAborted, message);
}

Status UnimplementedError(string_view message) {
  return Status(StatusCode::kUnimplemented, message);
}

Status InternalError(string_view message) {
  return Status(StatusCode::kInternal, message);
}

Status UnavailableError(string_view message) {
  return Status(StatusCode::kUnavailable, message);
}

StatusCode ErrnoToStatusCode(int error_number) {
  switch (error_number) {
    case 0:
      return StatusCode::kOk;
    case EINVAL:        // Invalid argument
    case ENAMETOOLONG:  // Filename too long
    case E2BIG:         // Argument list too long
    case EDESTADDRREQ:  // Destination address required
    case EDOM:          // Mathematics argument out of domain of function
    case EFAULT:        // Bad address
    case EILSEQ:        // Illegal byte sequence
    case ENOPROTOOPT:   // Protocol not available
    case ENOSTR:        // Not a STREAM
    case ENOTSOCK:      // Not a socket
    case ENOTTY:        // Inappropriate I/O control operation
    case EPROTOTYPE:    // Protocol wrong type for socket
    case ESPIPE:        // Invalid seek
      return StatusCode::kInvalidArgument;
    case ETIMEDOUT:  // Connection timed out
    case ETIME:      // Timer expired
      return StatusCode::kDeadlineExceeded;
    case ENODEV:  // No such device
    case ENOENT:  // No such file or directory
#ifdef ENOMEDIUM
    case ENOMEDIUM:  // No medium found
#endif
    case ENXIO:  // No such device or address
    case ESRCH:  // No such process
      return StatusCode::kNotFound;
    case EEXIST:         // File exists
    case EADDRNOTAVAIL:  // Address not available
    case EALREADY:       // Connection already in progress
#ifdef ENOTUNIQ
    case ENOTUNIQ:  // Name not unique on network
#endif
      return StatusCode::kAlreadyExists;
    case EPERM:   // Operation not permitted
    case EACCES:  // Permission denied
#ifdef ENOKEY
    case ENOKEY:  // Required key not available
#endif
    case EROFS:  // Read only file system
      return StatusCode::kPermissionDenied;
    case ENOTEMPTY:   // Directory not empty
    case EISDIR:      // Is a directory
    case ENOTDIR:     // Not a directory
    case EADDRINUSE:  // Address already in use
    case EBADF:       // Invalid file descriptor
#ifdef EBADFD
    case EBADFD:  // File descriptor in bad state
#endif
    case EBUSY:    // Device or resource busy
    case ECHILD:   // No child processes
    case EISCONN:  // Socket is connected
#ifdef EISNAM
    case EISNAM:  // Is a named type file
#endif
#ifdef ENOTBLK
    case ENOTBLK:  // Block device required
#endif
    case ENOTCONN:  // The socket is not connected
    case EPIPE:     // Broken pipe
#ifdef ESHUTDOWN
    case ESHUTDOWN:  // Cannot send after transport endpoint shutdown
#endif
    case ETXTBSY:  // Text file busy
#ifdef EUNATCH
    case EUNATCH:  // Protocol driver not attached
#endif
      return StatusCode::kFailedPrecondition;
    case ENOSPC:  // No space left on device
#ifdef EDQUOT
    case EDQUOT:  // Disk quota exceeded
#endif
    case EMFILE:   // Too many open files
    case EMLINK:   // Too many links
    case ENFILE:   // Too many open files in system
    case ENOBUFS:  // No buffer space available
    case ENODATA:  // No message is available on the STREAM read queue
    case ENOMEM:   // Not enough space
    case ENOSR:    // No STREAM resources
#ifdef EUSERS
    case EUSERS:  // Too many users
#endif
      return StatusCode::kResourceExhausted;
#ifdef ECHRNG
    case ECHRNG:  // Channel number out of range
#endif
    case EFBIG:      // File too large
    case EOVERFLOW:  // Value too large to be stored in data type
    case ERANGE:     // Result too large
      return StatusCode::kOutOfRange;
#ifdef ENOPKG
    case ENOPKG:  // Package not installed
#endif
    case ENOSYS:        // Function not implemented
    case ENOTSUP:       // Operation not supported
    case EAFNOSUPPORT:  // Address family not supported
#ifdef EPFNOSUPPORT
    case EPFNOSUPPORT:  // Protocol family not supported
#endif
    case EPROTONOSUPPORT:  // Protocol not supported
#ifdef ESOCKTNOSUPPORT
    case ESOCKTNOSUPPORT:  // Socket type not supported
#endif
    case EXDEV:  // Improper link
      return StatusCode::kUnimplemented;
    case EAGAIN:  // Resource temporarily unavailable
#ifdef ECOMM
    case ECOMM:  // Communication error on send
#endif
    case ECONNREFUSED:  // Connection refused
    case ECONNABORTED:  // Connection aborted
    case ECONNRESET:    // Connection reset
    case EINTR:         // Interrupted function call
#ifdef EHOSTDOWN
    case EHOSTDOWN:  // Host is down
#endif
    case EHOSTUNREACH:  // Host is unreachable
    case ENETDOWN:      // Network is down
    case ENETRESET:     // Connection aborted by network
    case ENETUNREACH:   // Network unreachable
    case ENOLCK:        // No locks available
    case ENOLINK:       // Link has been severed
#ifdef ENONET
    case ENONET:  // Machine is not on the network
#endif
      return StatusCode::kUnavailable;
    case EDEADLK:  // Resource deadlock avoided
#ifdef ESTALE
    case ESTALE:  // Stale file handle
#endif
      return StatusCode::kAborted;
    case ECANCELED:  // Operation cancelled
      return StatusCode::kCancelled;
    default:
      return StatusCode::kUnknown;
  }
}

namespace {
std::string MessageForErrnoToStatus(int error_number, string_view message) {
  return format("{}: {}", message, strerror(error_number));
}
}  // namespace

Status ErrnoToStatus(int error_number, string_view message) {
  return Status(ErrnoToStatusCode(error_number),
                MessageForErrnoToStatus(error_number, message));
}

string_view Status::message() const {
  if (state_ != nullptr) {
    uint32_t length;
    std::memcpy(&length, state_, sizeof(length));
    if (length > 0) {
      return string_view(state_ + 5, length);
    }
  }
  return string_view();
}

const char* Status::CopyState(const char* state) {
  uint32_t size;
  std::memcpy(&size, state, sizeof(size));
  char* result = new char[size + 5];
  std::memcpy(result, state, size + 5);
  return result;
}

Status::Status(StatusCode code, string_view msg) {
  assert(code != StatusCode::kOk);
  const uint32_t size = static_cast<uint32_t>(msg.size());
  char* result = new char[size + 5];
  std::memcpy(result, &size, sizeof(size));
  result[4] = static_cast<char>(code);
  std::memcpy(result + 5, msg.data(), size);
  state_ = result;
}

Status::Status(StatusCode code, const char* format, ...) {
  assert(code != StatusCode::kOk);

  va_list ap;
  int size = 100;  // 初始长度，能够覆盖到绝大部分场景
  int n = 0;

  for (int retry = 0; retry < 2; retry++) {
    char* result = new char[size + 5];
    va_start(ap, format);
    n = ::vsnprintf(result + 5, size, format, ap);
    va_end(ap);

    if (n < 0) {
      assert(true);  // not reached
    } else if (n >= size) {
      size = n + 1;  // with null char
    } else {
      const uint32_t real_size = n;
      std::memcpy(result, &real_size, sizeof(real_size));
      result[4] = static_cast<char>(code);
      state_ = result;
      return;
    }
    delete[] result;
  }
  assert(true);  // not reached
}

Status::~Status() { delete[] state_; }

std::string Status::ToStringSlow() const {
  std::string result = StatusCodeToString(code());

  if (state_ != nullptr) {
    uint32_t length;
    std::memcpy(&length, state_, sizeof(length));

    if (length > 0) {
      result += ": ";
      result.append(state_ + 5, length);
    }
  }

  return result;
}

const char* StatusMessageAsCStr(const Status& status) {
  // As an internal implementation detail, we guarantee that if status.message()
  // is non-empty, then the resulting string_view is null terminated.
  auto sv_message = status.message();
  return sv_message.empty() ? "" : sv_message.data();
}

}  // namespace bbt