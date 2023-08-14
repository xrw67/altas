#include "bbt/net/tcp/server.h"
#include "bbt/net/tcp/connection.h"
#include "bbt/net/tcp/connection_manager.h"
namespace bbt {
namespace net {

TcpServer::TcpServer(asio::io_context& io)
    : io_context_(io),
      acceptor_(io),
      connection_manager_(new TcpConnectionManager()) {}

TcpServer::~TcpServer() {
  Stop();
  delete connection_manager_;
}

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

void TcpServer::Boardcast(const void* data, size_t len) noexcept {
  connection_manager_->Boardcast(data, len);
}

void TcpServer::DoAccept() {
  acceptor_.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open()) {
          return;
        }

        auto conn = std::make_shared<TcpConnection>(std::move(socket));
        conn->set_connection_callback(connection_callback_);
        conn->set_receive_callback(receive_callback_);
        connection_manager_->Start(conn);
        DoAccept();  // Wait Next
      });
}

}  // namespace net
}  // namespace bbt