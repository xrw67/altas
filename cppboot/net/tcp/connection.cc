#include "cppboot/net/tcp/connection.h"

#include "cppboot/base/fmt.h"

namespace cppboot {
namespace net {

TcpConn::TcpConn(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {}

void TcpConn::Start() {
  state_ = kConnected;
  if (conn_callback_) conn_callback_(shared_from_this());
  ReadFromSocket();
}

void TcpConn::Stop() {
  socket_.shutdown(socket_.shutdown_both);
  state_ = kDisconnected;
  if (conn_callback_) conn_callback_(shared_from_this());
  socket_.close();
}

void TcpConn::Send(const void* data, size_t len) {
  std::lock_guard<std::mutex> guard(mutex_);
  output_buffer_.Append((const char*)data, len);

  auto ex = socket_.get_executor();
  asio::post(ex, [this]() { WriteToSocket(); });
}

std::string TcpConn::GetLocalAddress() const noexcept {
  try {
    auto ep = socket_.local_endpoint();
    return cppboot::format("{}:{}", ep.address().to_string(), ep.port());
  } catch (std::exception& e) {
    // FIXME:
    return {};
  }
}

std::string TcpConn::GetRemoteAddress() const noexcept {
  try {
    auto ep = socket_.remote_endpoint();
    return cppboot::format("{}:{}", ep.address().to_string(), ep.port());
  } catch (std::exception& e) {
    // FIEME：
    return {};
  }
}

void TcpConn::ReadFromSocket() {
  auto self = shared_from_this();
  socket_.async_read_some(
      asio::buffer(buffer_),
      [this, self](std::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          input_buffer_.Append(buffer_.data(), bytes_transferred);
          if (receive_callback_) receive_callback_(self, &input_buffer_);
          ReadFromSocket();
        } else if (ec != asio::error::operation_aborted) {
          Stop();
        }
      });
}

// 必须单线程执行
void TcpConn::WriteToSocket() {
  if (output_buffer_.ReadableBytes() != 0) {
    socket_.async_write_some(
        asio::buffer(output_buffer_.Peek(), output_buffer_.ReadableBytes()),
        [this](std::error_code ec, std::size_t bytes_transferred) {
          if (!ec) {
            output_buffer_.Retrive(bytes_transferred);
            if (output_buffer_.ReadableBytes() == 0) {
              // write_complete_callback_(shared_from_this());
            } else {
              WriteToSocket();  // continue write
            }
          } else if (ec != asio::error::operation_aborted) {
            Stop();
          }
        });
  }
}

}  // namespace net
}  // namespace cppboot