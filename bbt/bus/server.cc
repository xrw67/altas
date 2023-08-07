#include "bbt/bus/server.h"
#include "bbt/bus/server/connection.h"

namespace bbt {

namespace bus {

Server::Server(const std::string& name, asio::io_context& io_context)
    : name_(name), io_context_(io_context), acceptor_(io_context) {}

Server::~Server() {}

Status Server::Listen(const std::string& address, const std::string& port) {
  asio::ip::tcp::resolver resolver(io_context_);
  asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();

  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  DoAccept();
  return bbt::OkStatus();
}

void Server::Shutdown() {
  acceptor_.close();
  connection_manager_.StopAll();
}

void Server::DoAccept() {
  acceptor_.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open()) {
          return;
        }

        if (!ec) {
          connection_manager_.Start(std::make_shared<Connection>(
              std::move(socket),
              std::bind(&Server::HandleMsg, this, std::placeholders::_1,
                        std::placeholders::_2)));
        }

        DoAccept();  // Wait Next
      });
}

void Server::HandleMsg(const Msg& in, Msg* out) {
  // 分发给具体的Client的服务来处理
}

}  // namespace bus
}  // namespace bbt