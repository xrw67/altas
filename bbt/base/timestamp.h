#ifndef BBT_BASE_TIMESTAMP_H_
#define BBT_BASE_TIMESTAMP_H_

#include <bbt/base/config.h>

#include <time.h>
#include <string>
#include <algorithm>

namespace bbt {

///
/// Time stamp in UTC, in microseconds resolution.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///
class Timestamp {
 public:
  ///
  /// Constucts an invalid Timestamp.
  ///
  Timestamp() : micro_seconds_since_epoch_(0) {}

  ///
  /// Constucts a Timestamp at specific time
  ///
  explicit Timestamp(int64_t microSecondsSinceEpochArg)
      : micro_seconds_since_epoch_(microSecondsSinceEpochArg) {}

  void swap(Timestamp& that) {
    std::swap(micro_seconds_since_epoch_, that.micro_seconds_since_epoch_);
  }

  // default copy/assignment/dtor are Okay

  std::string ToString() const;
  std::string ToFormattedString(bool showMicroseconds = true) const;

  bool IsValid() const { return micro_seconds_since_epoch_ > 0; }

  // for internal usage.
  int64_t micro_seconds_since_epoch() const {
    return micro_seconds_since_epoch_;
  }
  time_t seconds_since_epoch() const {
    return static_cast<time_t>(micro_seconds_since_epoch_ /
                               kMicroSecondsPerSecond);
  }

  ///
  /// Get time of now.
  ///
  static Timestamp Now();
  static Timestamp Invalid() { return Timestamp(); }

  static Timestamp FromUnixTime(time_t t) { return FromUnixTime(t, 0); }

  static Timestamp FromUnixTime(time_t t, int microseconds) {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond +
                     microseconds);
  }

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t micro_seconds_since_epoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs) {
  return lhs.micro_seconds_since_epoch() < rhs.micro_seconds_since_epoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
  return lhs.micro_seconds_since_epoch() == rhs.micro_seconds_since_epoch();
}

///
/// Gets time difference of two timestamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microsecond
/// resolution for next 100 years.
inline double TimeDifference(Timestamp high, Timestamp low) {
  int64_t diff =
      high.micro_seconds_since_epoch() - low.micro_seconds_since_epoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

///
/// Add @c seconds to given timestamp.
///
/// @return timestamp+seconds as Timestamp
///
inline Timestamp AddTime(Timestamp timestamp, double seconds) {
  int64_t delta =
      static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.micro_seconds_since_epoch() + delta);
}
}  // namespace bbt

#endif  // BBT_BASE_TIMESTAMP_H_