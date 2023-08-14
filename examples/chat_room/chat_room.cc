#include <iostream>

#include "bbt/base/fmt.h"
#include "bbt/net/tcp/server.h"
#include "bbt/net/tcp/connection.h"

using bbt::net::Connection;
using bbt::net::ConnectionPtr;
using bbt::net::Buffer;
using bbt::net::TcpConnection;
using bbt::net::TcpServer;

int main(int argc, char* argv[]) {
  asio::io_context io_context;

  TcpServer svr(io_context);

  svr.set_connection_callback([](const ConnectionPtr& conn) {
    switch (conn->state()) {
      case Connection::kConnected:
        bbt::println("conn: connected");
        break;
      case Connection::kDisconnected:
        bbt::println("conn: disconnected");
    }
  });

  svr.set_read_callback([&svr](const ConnectionPtr& conn, Buffer* buf) {
    auto myconn = static_cast<TcpConnection*>(conn.get());
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