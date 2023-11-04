#include "cppboot/net/tcp/client.h"
#include "cppboot/net/tcp/connection.h"

namespace cppboot {
namespace net {

TcpClient::TcpClient(asio::io_context& io) : io_context_(io) {}

TcpClient::~TcpClient() { Stop(); }

Status TcpClient::Connect(const std::string& address, const std::string& port) {
  try {
    asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(address, port);
    asio::ip::tcp::socket socket(io_context_);
    asio::connect(socket, endpoints);

    conn_ = std::make_shared<TcpConn>(std::move(socket));
    conn_->set_conn_callback(conn_callback_);
    conn_->set_receive_callback(receive_callback_);
    conn_->Start();
  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

void TcpClient::Stop() { conn_->Stop(); }

void TcpClient::Send(const void* data, int len) { conn_->Send(data, len); }

}  // namespace net
}  // namespace cppboot