#include <iostream>

#include "bbt/base/fmt.h"
#include "bbt/net/tcp_server.h"
#include "bbt/net/tcp_connection.h"

using bbt::net::BaseConnection;
using bbt::net::BaseConnectionPtr;
using bbt::net::Buffer;
using bbt::net::MyTcpConnection;
using bbt::net::MyTcpServer;

int main(int argc, char* argv[]) {
  asio::io_context io_context;

  MyTcpServer svr(io_context);

  svr.set_conn_callback([](const BaseConnectionPtr& conn) {
    switch (conn->state()) {
      case BaseConnection::kConnected:
        bbt::println("conn: connected");
        break;
      case BaseConnection::kDisconnected:
        bbt::println("conn: disconnected");
    }
  });

  svr.set_read_callback([&svr](const BaseConnectionPtr& conn, Buffer* buf) {
    auto myconn = static_cast<MyTcpConnection*>(conn.get());
    auto username = myconn->GetRemoteAddress();
    auto s = buf->ToString();
    bbt::println("recv from {}: {}", username, s);

    std::string reply(username + ": " + s);
    svr.Boardcast(reply.c_str(), reply.length());
    buf->RetriveAll();
  });

  auto st = svr.Listen("0.0.0.0", "55555");
  if (!st) {
    bbt::println("error: {}", st);
    return -1;
  }

  bbt::println("listen done");
  io_context.run();
  return 0;
}