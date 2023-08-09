#include "gmock/gmock.h"

#include "client.h"
#include "server.h"
#include "bbt/net/testing/mocks.h"

namespace bbt {
namespace bus {

namespace {
using bbt::bus::BusClient;
using bbt::net::BaseConnection;
using bbt::net::MyTcpServer;
using bbt::net::testing::MockConnectionPair;

TEST(BusServer, use_tcp_server_as_transport_protocol) {
  asio::io_context io_context;

  MyTcpServer tcp_svr(io_context);
  ASSERT_TRUE(tcp_svr.Listen("0.0.0.0", "55555"));

  BusServer bus_svr1("svr1");

  tcp_svr.set_conn_callback(std::bind(&BusServer::HandleConnection, &bus_svr1,
                                      std::placeholders::_1));
  tcp_svr.set_read_callback(std::bind(&BusServer::HandleRead, &bus_svr1,
                                      std::placeholders::_1,
                                      std::placeholders::_2));

  tcp_svr.Stop();
}

struct MockTransportServer : public BaseConnection {
  // 模块两个客户端
  MockConnectionPair conn1;
  MockConnectionPair conn2;


  void HandleRead(const BaseConnectionPtr&conn, Buffer *buf) {
    
  }

};

TEST(BusServer, should_register_method_by_client) {
  BusServer bus_svr1("svr1");

  // 2 clients

  auto conn1 = std::make_shared<MockConnectionPair>();
  auto conn2 = std::make_shared<MockConnectionPair>();

  conn1->connect(conn2);
  conn2->connect(conn1);
  bbt::bus::BusClient client1("name1", conn1);
  bbt::bus::BusClient client2("name1", conn2);

  client1.AddMethod("func1", [](const In& in, Out* out) {
    out->set("name", in.get("name"));
    out->set("key2", "666");
  });

  bbt::bus::In in;
  in.set("name", "xrw");

  bbt::bus::Out out;
  ASSERT_TRUE(client2.Call("func1", in, &out));
  ASSERT_EQ(out.get("name"), "xrw");
  ASSERT_EQ(out.get("key2"), "666");
}

}  // namespace
}  // namespace bus
}  // namespace bbt