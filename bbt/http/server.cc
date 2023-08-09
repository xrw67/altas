#include "bbt/http/server.h"
#include "bbt/http/server/connection.h"

namespace bbt {
namespace http {

Server::Server() : io_context_(1), acceptor_(io_context_) {}

Server::~Server() {}

void Server::Handle(const std::string& path,
                    const ServeMux::Func& func) {
  request_handler_.set_handler(path, func);
}

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

void Server::Serve() { io_context_.run(); }

void Server::Shutdown() {
  acceptor_.close();
  connection_manager_.StopAll();
  io_context_.stop();
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
          connection_manager_.Start(std::make_shared<TcpConnection>(
              std::move(socket), connection_manager_, request_handler_));
        }

        DoAccept();  // Wait Next
      });
}

}  // namespace http
}  // namespace bbt