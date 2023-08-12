#include "gmock/gmock.h"
#include "bbt/bus/session.h"

namespace bbt {
namespace bus {
namespace {

struct BusSessionTest : public ::testing::Test {
  MsgPtr written_msg;
  BusSession session;

  BusSessionTest() : session("noname") {}

  void SetUp() {
    session.set_msg_writer([this](const MsgPtr& msg) { written_msg = msg; });
  }
};

TEST_F(BusSessionTest, should_register_method) {
  session.AddMethod("MyEcho.Echo", [](const In&, Out*) {});

  ASSERT_TRUE(written_msg);
  ASSERT_EQ(written_msg->method_provider(), "SvcMgr");
  ASSERT_EQ(written_msg->method(), "AddMethod");
  ASSERT_EQ(written_msg->param("MethodName"), "MyEcho.Echo");
}

TEST_F(BusSessionTest, shoueld_call_by_other_session) {
  session.AddMethod("MyEcho.Echo", [](const In& in, Out* out) {
    for (const auto& i : in.params()) out->set(i.first, i.second);

    out->set("result", "OK");
  });

  auto req = std::make_shared<Msg>();
  req->set_id(1234);
  req->set_method("MyEcho.Echo");
  req->set_param("key", "666");
  req->set_request(true);

  session.HandleMessage(req);

  ASSERT_TRUE(written_msg);
  ASSERT_FALSE(written_msg->is_request());
  ASSERT_EQ(written_msg->id(), 1234);
  ASSERT_EQ(written_msg->param("key"), "666");
  ASSERT_EQ(written_msg->param("result"), "OK");
}

}  // namespace
}  // namespace bus
}  // namespace bbt