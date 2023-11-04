#include <iostream>

#include "cppboot/base/fmt.h"
#include "cppboot/net/tcp/server.h"
#include "cppboot/net/tcp/connection.h"

using cppboot::net::Conn;
using cppboot::net::ConnPtr;
using cppboot::net::Buffer;
using cppboot::net::TcpConn;
using cppboot::net::TcpServer;

int main(int argc, char* argv[]) {
  asio::io_context io_context;

  TcpServer svr(io_context);

  svr.set_conn_callback([](const ConnPtr& conn) {
    switch (conn->state()) {
      case Conn::kConnected:
        cppboot::println("conn: connected");
        break;
      case Conn::kDisconnected:
        cppboot::println("conn: disconnected");
    }
  });

  svr.set_receive_callback([&svr](const ConnPtr& conn, Buffer* buf) {
    auto myconn = static_cast<TcpConn*>(conn.get());
    auto username = myconn->GetRemoteAddress();
    auto s = buf->ToString();
    cppboot::println("recv from {}: {}", username, s);

    std::string reply(username + ": " + s);
    svr.Boardcast(reply.c_str(), reply.length());
    buf->RetriveAll();
  });

  auto st = svr.Listen("0.0.0.0", "55555");
  if (!st) {
    cppboot::println("error: {}", st);
    return -1;
  }

  cppboot::println("listen done");
  io_context.run();
  return 0;
}