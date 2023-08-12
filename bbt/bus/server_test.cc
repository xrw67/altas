#include "gmock/gmock.h"

#include "client.h"
#include "server.h"
#include "bbt/net/testing/mocks.h"

namespace bbt {
namespace bus {
namespace {

using bbt::bus::BusClient;
using bbt::net::Connection;
using bbt::net::TcpServer;
using bbt::net::testing::MockConnectionPair;

TEST(BusServer, use_tcp_server_as_transport_protocol) {
  asio::io_context io_context;

  TcpServer tcp_svr(io_context);
  ASSERT_TRUE(tcp_svr.Listen("0.0.0.0", "55555"));

  BusServer bus_svr1("svr1");

  tcp_svr.set_connection_callback(std::bind(&BusServer::HandleConnection,
                                            &bus_svr1, std::placeholders::_1));
  tcp_svr.set_read_callback(std::bind(&BusServer::HandleRead, &bus_svr1,
                                      std::placeholders::_1,
                                      std::placeholders::_2));

  tcp_svr.Stop();
}

TEST(BusServer, should_register_method_by_client) {
  // SetUp: 1 server + 2 client
  //
  // BusClient|Connection <------ connect() -----> Connection|BusServer
  //

  auto svr_conn1 = std::make_shared<MockConnectionPair>();
  auto svr_conn2 = std::make_shared<MockConnectionPair>();
  auto cli_conn1 = std::make_shared<MockConnectionPair>();
  auto cli_conn2 = std::make_shared<MockConnectionPair>();

  cli_conn1->connect(svr_conn1);
  svr_conn1->connect(cli_conn1);
  cli_conn2->connect(svr_conn2);
  svr_conn2->connect(cli_conn2);
  
  BusServer server("server1");

  svr_conn1->set_read_callback(
      [&server](const ConnectionPtr& conn, Buffer* buf) {
        server.HandleRead(conn, buf);
      });
  svr_conn2->set_read_callback(
      [&server](const ConnectionPtr& conn, Buffer* buf) {
        server.HandleRead(conn, buf);
      });

  server.HandleConnection(svr_conn1);
  server.HandleConnection(svr_conn2);

  BusClient client1("client1", cli_conn1);
  BusClient client2("client2", cli_conn2);

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