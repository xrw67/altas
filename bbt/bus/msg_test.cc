#include "gmock/gmock.h"

#include "bbt/bus/msg.h"

namespace {

using bbt::bus::Msg;

TEST(Msg, create) {
  Msg msg;

  msg.Set("key1", "str1");
  msg.Set("key2", "1002");

  ASSERT_EQ(msg.Get("key1"), "str1");
  ASSERT_EQ(msg.Get("key2"), "1002");
  ASSERT_EQ(msg.Get("key3"), "");
}

}  // namespace