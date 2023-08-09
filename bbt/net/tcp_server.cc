#include "bbt/net/tcp_server.h"

namespace bbt {
namespace net {

MyTcpServer::MyTcpServer(asio::io_context& io)
    : io_context_(io), acceptor_(io) {}

Status MyTcpServer::Listen(const std::string& address,
                           const std::string& port) {
  asio::ip::tcp::resolver resolver(io_context_);
  asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();

  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  DoAccept();
  return bbt::OkStatus();
}

void MyTcpServer::Stop() { acceptor_.close(); }

void MyTcpServer::Boardcast(const void* data, size_t len) noexcept {
  for (auto& i : connection_manager_) {
    if (i->state() == BaseConnection::kConnected) {
      i->Send(data, len);
    }
  }
}

void MyTcpServer::DoAccept() {
  acceptor_.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open()) {
          return;
        }

        auto conn = std::make_shared<MyTcpConnection>(std::move(socket));
        conn->set_conn_callback(conn_callback_);
        conn->set_read_callback(read_callback_);
        connection_manager_.Start(conn);
        DoAccept();  // Wait Next
      });
}

}  // namespace net
}  // namespace bbt