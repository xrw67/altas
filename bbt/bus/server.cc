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

void Server::Stop() {
  tcp_.Stop();
  connection_manager_->StopAll();
}

void Server::OnNewConnection(std::error_code ec, asio::ip::tcp::socket socket) {
  if (!ec) {
    auto conn = std::make_shared<TcpConnection>(std::move(socket));
    conn->set_connection_context(new BusContext());
    conn->set_connection_callback(std::bind(&Server::OnConnection, this, _1));
    conn->set_message_callback(std::bind(&Server::OnMessage, this, _1, _2));
    connection_manager_->Start(conn);
  }
}

void Server::OnConnection(const TcpConnectionPtr& conn) {
  switch (conn->state()) {
    case TcpConnection::kConnected:
      break;
    case TcpConnection::kDisconnected:
      connection_manager_->Stop(conn);
      break;
  }
}

void Server::OnMessage(const TcpConnectionPtr& conn, Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());

  MsgPtr msg(new Msg());
  auto ret = ctx->Parse(buf, msg.get());
  switch (ret) {
    case BusContext::kBad:
      Stop();  // tcp。stop , conn.stop ??
      break;
    case BusContext::kGood:
      OnBusMsg(conn, msg);
      break;
    case BusContext::kContinue:
      return;  // 继续接收数据
  }
}

void Server::OnBusMsg(const TcpConnectionPtr& conn, const MsgPtr& msg) {
  // 分发给具体的Client的服务来处理
}

}  // namespace bus
}  // namespace bbt