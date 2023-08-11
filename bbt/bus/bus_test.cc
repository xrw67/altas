#include "gmock/gmock.h"

#include <thread>

#include "bbt/bus/client.h"
#include "bbt/bus/server.h"
#include "bbt/net/tcp_client.h"
#include "bbt/net/tcp_server.h"
#include "bbt/net/tcp_connection.h"
namespace {

using bbt::bus::BusServer;
using bbt::net::TcpServer;

TEST(Service, DISABLED_EchoService) {
  asio::io_context io_context(1);
  std::thread t([&]() { io_context.run(); });

  // BusServer
  TcpServer tcp_svr(io_context);
  BusServer server("mybus");

  tcp_svr.set_conn_callback(
      std::bind(&BusServer::HandleConnection, &server, std::placeholders::_1));
  tcp_svr.set_read_callback(std::bind(&BusServer::HandleRead, &server,
                                      std::placeholders::_1,
                                      std::placeholders::_2));

  auto st = tcp_svr.Listen("127.0.0.1", "59998");
  ASSERT_TRUE(st) << st.ToString();

  // client1 发布服务
  bbt::net::TcpClient net1(io_context);
  bbt::bus::BusClient c1("my_client1", net1.connection());

  // c1.set_net_connection(net1.connection());
  st = net1.Connect("127.0.0.1", "59998");

  ASSERT_TRUE(st) << st.ToString();

  c1.AddMethod("echo", [](const bbt::bus::In& in, bbt::bus::Out* out) {
    std::string name = in.get("name");
    out->set("greeting", "Hello, " + name);
  });

  // client2 调用服务
  bbt::net::TcpClient net2(io_context);
  bbt::bus::BusClient c2("my_client2", net2.connection());

  // c2.set_net_connection(net2.connection());
  st = net2.Connect("127.0.0.1", "59998");

  ASSERT_TRUE(st) << st.ToString();

  // 同步
  bbt::bus::In in;
  bbt::bus::Out out;
  in.set("name", "BBT");
  st = c2.Call("echo", in, &out);
  ASSERT_TRUE(st);
  ASSERT_EQ(out.get("greeting"), "Hello, BBT");

  // 异步
  bbt::bus::Result result;
  c2.ACall("echo", in, &result);
  st = result.Wait();
  ASSERT_TRUE(st);
  ASSERT_EQ(result.get("greeting"), "Hello, BBT");

  // teardown
  server.Stop();
  // c1.Shutdown();
  // c2.Shutdown();
  io_context.stop();
  t.join();
}

}  // namespace