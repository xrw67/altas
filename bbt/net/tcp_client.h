#ifndef BBT_NET_TCP_CLIENT_H_
#define BBT_NET_TCP_CLIENT_H_

#include <string>
#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/callbacks.h"
#include "bbt/net/tcp_connection.h"

namespace bbt {
namespace net {

class TcpClient {
 public:
  TcpClient(asio::io_context& ioctx);

  Status Connect(const std::string& address, const std::string& port);
  void Stop();
  void Send(const void* message, int len);

  asio::io_context& io_context() { return io_context_; }

  void set_connection_context(TcpConnection::Context* ctx) {
    conn_->set_connection_context(ctx);
  }
  void set_connection_callback(const ConnectionCallback& cb) {
    conn_->set_connection_callback(cb);
  }

  void set_message_callback(const MessageCallback& cb) {
    conn_->set_message_callback(cb);
  }

  asio::io_context& io_context_;
  TcpConnectionPtr conn_;
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CLIENT_H_