#include "bbt/base/status.h"

#include <cstdio>
#include <cassert>
#include <ostream>

namespace bbt {

Status OkStatus() { return Status(); }

std::ostream& operator<<(std::ostream& os, const Status& x) {
  os << x.ToString();
  return os;
}

bool IsNotFound(const Status& status) {
  return status.code() == StatusCode::kNotFound;
}

bool IsCorruption(const Status& status) {
  return status.code() == StatusCode::kCorruption;
}

bool IsIOError(const Status& status) {
  return status.code() == StatusCode::kIOError;
}

bool IsNotSupportedError(const Status& status) {
  return status.code() == StatusCode::kNotSupported;
}

bool IsInvalidArgument(const Status& status) {
  return status.code() == StatusCode::kInvalidArgument;
}

Status NotFoundError(string_view message) {
  return Status(StatusCode::kNotFound, message);
}

Status CorruptionError(string_view message) {
  return Status(StatusCode::kCorruption, message);
}

Status NotSupportedError(string_view message) {
  return Status(StatusCode::kNotSupported, message);
}

Status InvalidArgumentError(string_view message) {
  return Status(StatusCode::kInvalidArgument, message);
}

Status IOError(string_view message) {
  return Status(StatusCode::kIOError, message);
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

std::string Status::ToStringSlow() const {
  if (state_ == nullptr) {
    return "OK";
  } else {
    char tmp[30];
    const char* type;
    switch (code()) {
      case StatusCode::kOk:
        type = "OK";
        break;
      case StatusCode::kNotFound:
        type = "NotFound: ";
        break;
      case StatusCode::kCorruption:
        type = "Corruption: ";
        break;
      case StatusCode::kNotSupported:
        type = "Not implemented: ";
        break;
      case StatusCode::kInvalidArgument:
        type = "Invalid argument: ";
        break;
      case StatusCode::kIOError:
        type = "IO error: ";
        break;
      default:
        std::snprintf(tmp, sizeof(tmp),
                      "Unknown code(%d): ", static_cast<int>(code()));
        type = tmp;
        break;
    }
    std::string result(type);
    uint32_t length;
    std::memcpy(&length, state_, sizeof(length));
    result.append(state_ + 5, length);
    return result;
  }
}

}  // namespace bbt