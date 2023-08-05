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

static const char* json_data =
    "{\"id\":123,\"method\":\"foo\",\"param\":{\"key1\":\"value1\","
    "\"key2\":\"value2\"}}";

TEST(Msg, should_pack_by_json) {
  Msg msg;
  msg.set_id(123);
  msg.set_method("foo");
  msg.Set("key1", "value1");
  msg.Set("key2", "value2");

  bbt::bus::JsonPacker j;
  std::string s;

  j.Pack(msg, &s);
  ASSERT_EQ(s, json_data);
}

TEST(Msg, should_unpack_by_json) {
  Msg msg;
  bbt::bus::JsonPacker j;

  ASSERT_TRUE(j.UnPack(json_data, &msg));
  ASSERT_EQ(msg.id(), 123);
  ASSERT_EQ(msg.method(), "foo");
  ASSERT_EQ(msg.Get("key1"), "value1");
  ASSERT_EQ(msg.Get("key2"), "value2");
}

}  // namespace