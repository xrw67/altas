#include "gmock/gmock.h"

#include <thread>

#include "bbt/bus/client.h"
#include "bbt/bus/server.h"
#include "bbt/net/tcp_client.h"

namespace {

TEST(Service, DISABLED_EchoService) {
  asio::io_context io_context(1);
  std::thread t([&]() { io_context.run(); });

  // Server
  bbt::bus::Server server("mybus", io_context);
  auto st = server.Listen("127.0.0.1", "59998");
  ASSERT_TRUE(st) << st.ToString();

  // client1 发布服务
  bbt::net::TcpClient net1(io_context);
  bbt::bus::BusClient c1("my_client1", net1);
  st = net1.Connect("127.0.0.1", "59998");
  ASSERT_TRUE(st) << st.ToString();

  c1.AddMethod("echo", [](const bbt::bus::In& in, bbt::bus::Out* out) {
    std::string name = in.get("name");
    out->set("greeting", "Hello, " + name);
  });

  // client2 调用服务
   bbt::net::TcpClient net2(io_context);
  bbt::bus::BusClient c2("my_client2", net2);
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