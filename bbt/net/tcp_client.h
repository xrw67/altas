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
  TcpClient(asio::io_context& io);
  ~TcpClient();

  Status Connect(const std::string& address, const std::string& port);
  void Stop() { conn_->Stop(); }

  void Send(const void* data, int len) { conn_->Send(data, len); }
  MyTcpConnectionPtr connection() { return conn_; }

  void set_conn_callback(const ConnCallback& cb) { conn_callback_ = cb; }
  void set_read_callback(const ReadCallback& cb) { read_callback_ = cb; }

  asio::io_context& io_context_;
  MyTcpConnectionPtr conn_;

  ConnCallback conn_callback_;
  ReadCallback read_callback_;
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CLIENT_H_