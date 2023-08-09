#include "bbt/net/tcp_connection.h"

#include <algorithm>
#include <functional>

namespace bbt {
namespace net {

TcpConnection::TcpConnection(asio::ip::tcp::socket socket)
    : state_(kDisconnected), socket_(std::move(socket)), context_(nullptr) {}

TcpConnection::~TcpConnection() { delete context_; }

void TcpConnection::Start() {
  state_ = kConnected;
  connection_callback_(shared_from_this());
  DoRead();
}

void TcpConnection::Stop() {
  socket_.shutdown(socket_.shutdown_both);
  state_ = kDisconnected;
  connection_callback_(shared_from_this());
  socket_.close();
}

void TcpConnection::Send(const void* message, int len) {
  std::lock_guard<std::mutex> guard(mutex_);
  output_buffer_.Append((const char *)message, len);
  DoWrite();
}

void TcpConnection::DoRead() {
  auto self(shared_from_this());
  socket_.async_read_some(
      asio::buffer(buffer_),
      [this, self](std::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          input_buffer_.Append(buffer_.data(), bytes_transferred);
          message_callback_(self, &input_buffer_);
          DoRead();
        } else if (ec != asio::error::operation_aborted) {
          Stop();
        }
      });
}

void TcpConnection::DoWrite() {
  if (output_buffer_.ReadableBytes() == 0) {
    socket_.async_write_some(
        asio::buffer(output_buffer_.Peek(), output_buffer_.ReadableBytes()),
        [this](std::error_code ec, std::size_t bytes_transferred) {
          if (!ec) {
            output_buffer_.Retrive(bytes_transferred);
            if (output_buffer_.ReadableBytes() == 0) {
              // write_complete_callback_(shared_from_this());
            } else {
              DoWrite();  // continue write
            }
          } else if (ec != asio::error::operation_aborted) {
            Stop();
          }
        });
  }
}

}  // namespace net
}  // namespace bbt