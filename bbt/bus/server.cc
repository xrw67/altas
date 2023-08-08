#include "bbt/bus/server.h"
#include "bbt/net/tcp_connection.h"
#include "bbt/net/tcp_connection_manager.h"

#include "bbt/bus/context.h"

namespace bbt {
namespace bus {

using bbt::net::_1;
using bbt::net::_2;

//
// Server
//
Server::Server(const std::string& name, asio::io_context& ioctx)
    : name_(name), tcp_(ioctx), connection_manager_(new ConnectionManager()) {}

Server::~Server() {}

Status Server::Listen(const std::string& address, const std::string& port) {
  auto st = tcp_.Listen(address, port);
  if (!st) return st;

  tcp_.set_new_connection_callback(
      std::bind(&Server::OnNewConnection, this, _1, _2));
  return st;
}

void Server::Shutdown() {
  tcp_.Stop();
  connection_manager_->StopAll();
}

void Server::OnNewConnection(std::error_code ec, asio::ip::tcp::socket socket) {
  if (!ec) {
    auto conn = std::make_shared<Connection>(std::move(socket));
    conn->set_context(
        new BusContext(std::bind(&Server::OnBusMsg, this, _1, _2)));
    conn->set_connection_callback(std::bind(&Server::OnConnection, this, _1));
    conn->set_message_callback(std::bind(&Server::OnMessage, this, _1, _2));
    connection_manager_->Start(conn);
  }
}

void Server::OnConnection(const ConnectionPtr& conn) {
  switch (conn->state()) {
    case Connection::kConnected:
      break;
    case Connection::kDisconnected:
      connection_manager_->Stop(conn);
      break;
  }
}

void Server::OnMessage(const ConnectionPtr& conn, Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());
  if (!ctx->Parse(conn, buf)) {
    // 解析失败,
    conn->Stop();
  }
}

void Server::OnBusMsg(const ConnectionPtr& conn, const MsgPtr& msg) {
  // 分发给具体的Client的服务来处理
}

}  // namespace bus
}  // namespace bbt