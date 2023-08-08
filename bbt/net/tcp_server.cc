#include "bbt/net/tcp_server.h"

namespace bbt {
namespace net {

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

}  // namespace net
}  // namespace bbt