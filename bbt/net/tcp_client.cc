#include "bbt/net/tcp_client.h"

namespace bbt {
namespace net {

TcpClient::TcpClient(asio::io_context& ioctx) : io_context_(ioctx) {}

Status TcpClient::Connect(const std::string& address, const std::string& port) {
  try {
    asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(address, port);
    asio::ip::tcp::socket socket(io_context_);
    asio::connect(socket, endpoints);

    conn_ = std::make_shared<TcpConnection>(std::move(socket));
    conn_->Start();
  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

void TcpClient::Stop() { conn_->Stop(); }

void TcpClient::Send(const void* message, int len) {
  conn_->Send(message, len);
}

}  // namespace net
}  // namespace bbt