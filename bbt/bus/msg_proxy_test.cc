#include "gmock/gmock.h"

#include "bbt/bus/msg_proxy.h"
namespace {

using bbt::bus::Msg;
using bbt::bus::MsgProxy;

struct MockBaseConnection : public bbt::net::Connection {
  void Send(const void* data, size_t len) {}
};

TEST(MsgProxy, hello) {
  MsgProxy proxy;

  auto conn1 = std::make_shared<MockBaseConnection>();
  auto conn2 = std::make_shared<MockBaseConnection>();

  proxy.AddMethod(conn1, "method1");
  proxy.AddMethod(conn2, "method2");

  Msg msg;
  msg.set_method("method2");
  ASSERT_TRUE(proxy.ProxyRequestMsg(&msg));
  ASSERT_EQ(msg.dst, conn2);

  msg.set_method("methodX");
  ASSERT_FALSE(proxy.ProxyRequestMsg(&msg));
}

}  // namespace
