#include "bbt/bus/server.h"
#include "bbt/bus/connection.h"
#include "bbt/bus/connection_manager.h"

namespace bbt {

namespace bus {

TcpServer::TcpServer(asio::io_context& ioctx)
    : io_context_(ioctx), acceptor_(ioctx) {}

Status TcpServer::Listen(const std::string& address, const std::string& port) {
  asio::ip::tcp::resolver resolver(io_context_);
  asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();

  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  DoAccept();
  return bbt::OkStatus();
}

void TcpServer::Stop() { acceptor_.close(); }

void TcpServer::DoAccept() {
  acceptor_.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open()) {
          return;
        }

        new_connection_callback_(ec, std::move(socket));
        DoAccept();  // Wait Next
      });
}

//
// Server
//
Server::Server(const std::string& name, asio::io_context& ioctx)
    : name_(name), tcp_(ioctx), connection_manager_(new ConnectionManager()) {}

Server::~Server() {}

Status Server::Listen(const std::string& address, const std::string& port) {
  auto st = tcp_.Listen(address, port);
  if (!st) return st;

  tcp_.set_new_connection_callback(std::bind(&Server::OnNewConnection, this, _1, _2));
  return st;
}

void Server::Shutdown() {
  tcp_.Stop();
  connection_manager_->StopAll();
}

void Server::OnNewConnection(std::error_code ec, asio::ip::tcp::socket socket) {
  if (!ec) {
    connection_manager_->Start(std::make_shared<BusConnection>(
        std::move(socket),
        std::bind(&Server::HandleMsg, this, std::placeholders::_1)));
  }
}

void Server::HandleMsg(const MsgPtr& msg) {
  // 分发给具体的Client的服务来处理
}

}  // namespace bus
}  // namespace bbt