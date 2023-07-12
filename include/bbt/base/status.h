// Base on leveldb project
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// A Status encapsulates the result of an operation.  It may indicate success,
// or it may indicate an error with an associated error message.
//
// Multiple threads can invoke const methods on a Status without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same Status must use
// external synchronization.

#ifndef BBT_BASE_STATUS_H_
#define BBT_BASE_STATUS_H_

#include <algorithm>
#include <string>

#include "bbt/base/attributes.h"
#include "bbt/base/string_view.h"

namespace bbt {

enum class StatusCode : int {
  kOk = 0,
  kCancelled = 1,
  kUnknown = 2,
  kInvalidArgument = 3,
  kNotFound = 5,
  kAlreadyExists = 6,
  kPermissionDenied = 7,
  kOutOfRange = 11,
  kUnimplemented = 12,
};

// StatusCodeToString()
//
// Returns the name for the status code, or "" if it is an unknown value.
std::string StatusCodeToString(StatusCode code);

// operator<<
//
// Streams StatusCodeToString(code) to `os`.
std::ostream& operator<<(std::ostream& os, StatusCode code);

class Status {
 public:
  // Create a success status.
  Status() noexcept;
  Status(StatusCode code, string_view msg, string_view msg2 = string_view());
  Status(const Status& rhs);
  Status& operator=(const Status& rhs);

  Status(Status&& rhs) noexcept;
  Status& operator=(Status&& rhs) noexcept;

  ~Status();

  void Update(const Status& new_status) noexcept;
  void Update(Status&& new_status);

  StatusCode code() const;
  string_view message() const;

  // Returns true if the status indicates success.
  BBT_MUST_USE_RESULT bool ok() const;

  explicit operator bool() const noexcept;

  // Return a string representation of this status suitable for printing.
  // Returns the string "OK" for success.
  std::string ToString() const;

  // Ignores any errors. This method does nothing except potentially suppress
  // complaints from any tools that are checking that errors are not dropped on
  // the floor.
  void IgnoreError() const;

 private:
  static const char* CopyState(const char* s);
  std::string ToStringSlow() const;

  // OK status has a null state_.  Otherwise, state_ is a new[] array
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4]    == code
  //    state_[5..]  == message
  const char* state_;
};

// Returns an OK status, equivalent to a default constructed instance. Prefer
// usage of `:OkStatus()` when constructing such an OK status.
Status OkStatus();

// Prints a human-readable representation of `x` to `os`.
std::ostream& operator<<(std::ostream& os, const Status& x);

// These convenience functions return `true` if a given status matches the
// `StatusCode` error code of its associated function.

BBT_MUST_USE_RESULT bool IsCancelled(const Status& status);
BBT_MUST_USE_RESULT bool IsUnknown(const Status& status);
BBT_MUST_USE_RESULT bool IsInvalidArgument(const Status& status);
BBT_MUST_USE_RESULT bool IsNotFound(const Status& status);
BBT_MUST_USE_RESULT bool IsAlreadyExists(const Status& status);
BBT_MUST_USE_RESULT bool IsPermissionDenied(const Status& status);
BBT_MUST_USE_RESULT bool IsOutOfRange(const Status& status);
BBT_MUST_USE_RESULT bool IsUnimplemented(const Status& status);

// These convenience functions create an `Status` object with an error
// code as indicated by the associated function name, using the error message
// passed in `message`.

Status CancelledError(string_view message);
Status UnknownError(string_view message);
Status InvalidArgumentError(string_view message);
Status NotFoundError(string_view message);
Status AlreadyExistsError(string_view message);
Status PermissionDeniedError(string_view message);
Status OutOfRangeError(string_view message);
Status UnimplementedError(string_view message);

//-------------------------------------------------------------------
// Implementation
//-------------------------------------------------------------------

inline Status::Status() noexcept : state_(nullptr) {}

inline Status::Status(const Status& rhs) {
  state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}
inline Status& Status::operator=(const Status& rhs) {
  // The following condition catches both aliasing (when this == &rhs),
  // and the common case where both rhs and *this are ok.
  if (state_ != rhs.state_) {
    delete[] state_;
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
  }
  return *this;
}

inline Status::Status(Status&& rhs) noexcept : state_(rhs.state_) {
  rhs.state_ = nullptr;
}

inline Status& Status::operator=(Status&& rhs) noexcept {
  std::swap(state_, rhs.state_);
  return *this;
}

inline void Status::Update(const Status& new_status) noexcept {
  if (ok()) {
    *this = new_status;
  }
}

inline void Status::Update(Status&& new_status) {
  if (ok()) {
    *this = std::move(new_status);
  }
}

inline StatusCode Status::code() const {
  return (state_ == nullptr) ? StatusCode::kOk
                             : static_cast<StatusCode>(state_[4]);
}

inline bool Status::ok() const { return (state_ == nullptr); }

inline Status::operator bool() const noexcept { return ok(); }

inline std::string Status::ToString() const {
  return ok() ? "OK" : ToStringSlow();
}

inline void Status::IgnoreError() const {
  // no-op
}

}  // namespace bbt

#endif  // BBT_BASE_STATUS_H_
