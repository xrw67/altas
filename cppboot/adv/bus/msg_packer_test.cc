#include "gmock/gmock.h"

#include "cppboot/adv/bus/msg.h"
#include "cppboot/adv/bus/msg_packer.h"

namespace {

using cppboot::Msg;

static const char* json_data =
    "{\"caller\":\"\",\"id\":123,\"is_req\":true,\"method\":\"foo\",\"method_"
    "provider\":\"\",\"param\":[{\"t\":\"key1\",\"v\":\"str1\"},{\"t\":"
    "\"key2\",\"v\":\"1001\"}]}";

TEST(MsgPacker, should_pack_by_json) {
  Msg msg;
  msg.set_id(123);
  msg.set_method("foo");
  msg.set_param("key1", "str1");
  msg.set_param("key2", "1001");

  cppboot::JsonPacker j;
  std::string s;

  j.Pack(msg, &s);
  ASSERT_EQ(s, json_data);
}

TEST(MsgPacker, should_unpack_by_json) {
  Msg msg;
  cppboot::JsonPacker j;

  auto st = j.Unpack(json_data, &msg);
  ASSERT_TRUE(st) << st.ToString();
  ASSERT_EQ(msg.id(), 123);
  ASSERT_EQ(msg.method(), "foo");
  ASSERT_EQ(msg.param("key1"), "str1");
  ASSERT_EQ(msg.param("key2"), "1001");
}

TEST(MsgPacker, should_catch_exception_with_unpack) {
  Msg msg;
  cppboot::JsonPacker j;

  ASSERT_FALSE(j.Unpack("{\"id\":\"abc\"}", &msg));
  ASSERT_FALSE(j.Unpack("{\"id\":123, \"method\": 123}", &msg));
  ASSERT_FALSE(j.Unpack(
      "{\"id\":123,\"method\":\"foo\",\"param\":[{\"t\":\"key1\"}]}", &msg));
  ASSERT_FALSE(j.Unpack(
      "{\"id\":123,\"method\":\"foo\",\"param\":[{\"v\":\"str1\"}]}", &msg));
  ASSERT_FALSE(j.Unpack(
      "{\"id\":123,\"method\":\"foo\",\"param\":[{\"nokey\":\"str1\"}]}",
      &msg));
}

}  // namespace