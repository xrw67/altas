#include "gmock/gmock.h"

#include "cppboot/bus/msg.h"

namespace {

using cppboot::bus::Msg;

TEST(Msg, create) {
  Msg msg;
  msg.set_id(123);
  msg.set_caller("me");
  msg.set_request(false);
  msg.set_param("key1", "str1");
  msg.set_param("key2", "1002");

  ASSERT_EQ(msg.id(), 123);
  ASSERT_EQ(msg.caller(), "me");
  ASSERT_FALSE(msg.is_request());
  ASSERT_EQ(msg.param("key1"), "str1");
  ASSERT_EQ(msg.param("key2"), "1002");
  ASSERT_FALSE(msg.params().has("key3"));
  ASSERT_EQ(msg.param("key3"), "");
}

}  // namespace