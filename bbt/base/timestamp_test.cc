#include "gmock/gmock.h"

#include "bbt/base/timestamp.h"

namespace {

// Invalid()
TEST(Timestamp, default_is_invalid) {
  bbt::Timestamp ts;
  ASSERT_FALSE(ts.IsValid());
  ASSERT_FALSE(bbt::Timestamp::Invalid().IsValid());
}

// FromUnixTime
TEST(Timestamp, convert_from_unixtime) {
  auto ts = bbt::Timestamp::Now();
  auto unixtime = ts.seconds_since_epoch();
  ASSERT_EQ(ts.ToFormattedString(false),
            bbt::Timestamp::FromUnixTime(unixtime).ToFormattedString(false));
}

// ToString

TEST(Timestamp, verify_to_string) {
  auto ts = bbt::Timestamp::FromUnixTime(1234, 5678);
  ASSERT_EQ(ts.ToString(), "1234.005678");
}

// Swap
TEST(TImestamp, can_swap) {
  auto ts1 = bbt::Timestamp::Invalid();
  auto ts2 = bbt::Timestamp::FromUnixTime(1234);
  ts1.swap(ts2);
  ASSERT_FALSE(ts2.IsValid());
  ASSERT_EQ(ts1.seconds_since_epoch(), 1234);
}

}  // namespace