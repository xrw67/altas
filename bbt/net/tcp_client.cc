#include "bbt/net/tcp_client.h"

namespace bbt {
namespace net {

TcpClient::TcpClient(asio::io_context& ioctx)
    : io_context_(ioctx), socket_(ioctx) {}

Status TcpClient::Connect(const std::string& address, const std::string& port) {
  try {
    asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(address, port);
    asio::connect(socket_, endpoints);

  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

}  // namespace net
}  // namespace bbt