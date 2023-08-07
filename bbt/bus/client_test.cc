#include "gmock/gmock.h"

#include <vector>

#include "bbt/bus/method.h"
#include "bbt/bus/client.h"
#include "bbt/bus/server/connection.h"

namespace {

class MsgReceiver {
 public:
  std::vector<Msg> msgs;

  MsgReceiver(asio::io_context& io_context) 
    : io_context_(io_context), () {}

 private:
    void DoNewConnection(asio::ip::tcp::socket socket)
  asio::io_context& io_context_;
  bbt::bus::Connection peer_;
};

TEST(Client, should_register_method) {
  MsgReceiver receiver("127.0.0.1", "50001");

  bbt::bus::Client client;
  client.Connect("127.0.0.1", "50001");

  bbt::Bus::In in;
  in.Set("key1", "str1");
  in.Set("key2", "1001");
  client.RegisterMethod("MyEcho.Echo", in, NULL));

  ASSERT_EQ(receiver.msgs.size(), 1);
  const Msg& msg = receiver.msgs[0];

  ASSERT_EQ(msg.method(), "MyEcho.Echo")
  ASSERT_EQ(msg.Get("key1"), "str1");
  ASSERT_EQ(msg.Get("key2"), "1001");
}

}  // namespace