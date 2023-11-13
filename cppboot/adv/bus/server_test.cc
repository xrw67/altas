#include "gmock/gmock.h"

#include "client.h"
#include "server.h"
#include "cppboot/net/testing/mocks.h"

namespace cppboot {
namespace {

using cppboot::BusClient;
using cppboot::net::Conn;
using cppboot::net::TcpServer;
using cppboot::net::testing::MockConnectionPair;

TEST(BusServer, DISABLED_use_tcp_server_as_transport_protocol) {
  asio::io_context io_context;

  TcpServer tcp_svr(io_context);
  ASSERT_TRUE(tcp_svr.Listen("0.0.0.0", "55555"));

  BusServer bus_svr1("svr1");

  tcp_svr.set_conn_callback(std::bind(&BusServer::HandleConnection, &bus_svr1,
                                      std::placeholders::_1));
  tcp_svr.set_receive_callback(std::bind(&BusServer::OnReceive, &bus_svr1,
                                         std::placeholders::_1,
                                         std::placeholders::_2));

  tcp_svr.Stop();
}

TEST(BusServer, should_register_method_by_client) {
  // SetUp: 1 server + 2 client
  //
  // BusClient|Conn <------ connect() -----> Conn|BusServer
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

  svr_conn1->set_receive_callback([&server](const ConnPtr& conn, Buffer* buf) {
    server.OnReceive(conn, buf);
  });
  svr_conn2->set_receive_callback([&server](const ConnPtr& conn, Buffer* buf) {
    server.OnReceive(conn, buf);
  });

  server.HandleConnection(svr_conn1);
  server.HandleConnection(svr_conn2);

  BusClient client1("client1", cli_conn1);
  BusClient client2("client2", cli_conn2);

  client1.Start();
  client2.Start();

  client1.AddMethod("func1", [](const In& in, Out* out) {
    out->set("name", in.get("name"));
    out->set("key2", "666");
  });

  cppboot::In in;
  in.set("name", "xrw");

  cppboot::Out out;
  ASSERT_TRUE(client2.Call("client1/func1", in, &out));
  ASSERT_EQ(out.get("name"), "xrw");
  ASSERT_EQ(out.get("key2"), "666");
}

}  // namespace
}  // namespace cppboot