#include "bbt/net/tcp/connection.h"

#include "bbt/base/fmt.h"

namespace bbt {
namespace net {

TcpConnection::TcpConnection(asio::ip::tcp::socket socket)
    : socket_(std::move(socket)) {}

void TcpConnection::Start() {
  state_ = kConnected;
  connection_callback_(shared_from_this());
  ReadFromSocket();
}

void TcpConnection::Stop() {
  socket_.shutdown(socket_.shutdown_both);
  state_ = kDisconnected;
  connection_callback_(shared_from_this());
  socket_.close();
}

void TcpConnection::Send(const void* data, size_t len) {
  std::lock_guard<std::mutex> guard(mutex_);
  output_buffer_.Append((const char*)data, len);

  auto ex = socket_.get_executor();
  asio::post(ex, [this]() { WriteToSocket(); });
}

std::string TcpConnection::GetLocalAddress() const noexcept {
  try {
    auto ep = socket_.local_endpoint();
    return bbt::format("{}:{}", ep.address().to_string(), ep.port());
  } catch (std::exception& e) {
    // FIXME:
    return {};
  }
}

std::string TcpConnection::GetRemoteAddress() const noexcept {
  try {
    auto ep = socket_.remote_endpoint();
    return bbt::format("{}:{}", ep.address().to_string(), ep.port());
  } catch (std::exception& e) {
    // FIEME：
    return {};
  }
}

void TcpConnection::ReadFromSocket() {
  auto self = shared_from_this();
  socket_.async_read_some(
      asio::buffer(buffer_),
      [this, self](std::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          input_buffer_.Append(buffer_.data(), bytes_transferred);
          read_callback_(self, &input_buffer_);
          ReadFromSocket();
        } else if (ec != asio::error::operation_aborted) {
          Stop();
        }
      });
}

// 必须单线程执行
void TcpConnection::WriteToSocket() {
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
}  // namespace bbt