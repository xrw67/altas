#include "gmock/gmock.h"

#include "cppboot/base/timestamp.h"

namespace {

// Invalid()
TEST(Timestamp, default_is_invalid) {
  cppboot::Timestamp ts;
  ASSERT_FALSE(ts.IsValid());
  ASSERT_FALSE(cppboot::Timestamp::Invalid().IsValid());
}

// FromUnixTime
TEST(Timestamp, convert_from_unixtime) {
  auto ts = cppboot::Timestamp::Now();
  auto unixtime = ts.seconds_since_epoch();
  ASSERT_EQ(
      ts.ToFormattedString(false),
      cppboot::Timestamp::FromUnixTime(unixtime).ToFormattedString(false));
}

// ToString

TEST(Timestamp, verify_to_string) {
  auto ts = cppboot::Timestamp::FromUnixTime(1234, 5678);
  ASSERT_EQ(ts.ToString(), "1234.005678");
}

// Swap
TEST(TImestamp, can_swap) {
  auto ts1 = cppboot::Timestamp::Invalid();
  auto ts2 = cppboot::Timestamp::FromUnixTime(1234);
  ts1.swap(ts2);
  ASSERT_FALSE(ts2.IsValid());
  ASSERT_EQ(ts1.seconds_since_epoch(), 1234);
}

}  // namespace