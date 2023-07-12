#include "bbt/base/status.h"

#include <cstdio>
#include <cassert>
#include <ostream>

namespace bbt {

std::string StatusCodeToString(StatusCode code) {
  switch (code) {
    case StatusCode::kOk:
      return "OK";
    case StatusCode::kCancelled:
      return "CANCELLED";
    case StatusCode::kUnknown:
      return "UNKNOWN";
    case StatusCode::kInvalidArgument:
      return "INVALID_ARGUMENT";
    case StatusCode::kNotFound:
      return "NOT_FOUND";
    case StatusCode::kAlreadyExists:
      return "ALREADY_EXISTS";
    case StatusCode::kPermissionDenied:
      return "PERMISSION_DENIED";
    case StatusCode::kOutOfRange:
      return "OUT_OF_RANGE";
    case StatusCode::kUnimplemented:
      return "UNIMPLEMENTED";
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

bool IsOutOfRange(const Status& status) {
  return status.code() == StatusCode::kOutOfRange;
}

bool IsUnimplemented(const Status& status) {
  return status.code() == StatusCode::kUnimplemented;
}

Status CancelledError(string_view message) {
  return Status(StatusCode::kCancelled, message);
}

Status UnknownError(string_view message) {
  return Status(StatusCode::kUnknown, message);
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

Status UnimplementedError(string_view message) {
  return Status(StatusCode::kUnimplemented, message);
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

Status::Status(StatusCode code, string_view msg, string_view msg2) {
  assert(code != StatusCode::kOk);
  const uint32_t len1 = static_cast<uint32_t>(msg.size());
  const uint32_t len2 = static_cast<uint32_t>(msg2.size());
  const uint32_t size = len1 + (len2 ? (2 + len2) : 0);
  char* result = new char[size + 5];
  std::memcpy(result, &size, sizeof(size));
  result[4] = static_cast<char>(code);
  std::memcpy(result + 5, msg.data(), len1);
  if (len2) {
    result[5 + len1] = ':';
    result[6 + len1] = ' ';
    std::memcpy(result + 7 + len1, msg2.data(), len2);
  }
  state_ = result;
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

}  // namespace bbt