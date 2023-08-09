#include "bbt/net/tcp_client.h"

namespace bbt {
namespace net {

TcpClient::TcpClient(asio::io_context& io) : io_context_(io) {}

TcpClient::~TcpClient() { Stop(); }

Status TcpClient::Connect(const std::string& address,
                            const std::string& port) {
  try {
    asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(address, port);
    asio::ip::tcp::socket socket(io_context_);
    asio::connect(socket, endpoints);

    conn_ = std::make_shared<MyTcpConnection>(std::move(socket));
    conn_->set_conn_callback(conn_callback_);
    conn_->set_read_callback(read_callback_);
    conn_->Start();
  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

}  // namespace net
}  // namespace bbt