#ifndef BBT_NET_TCP_CLIENT_H_
#define BBT_NET_TCP_CLIENT_H_

#include <string>

#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

class TcpClient {
 public:
  TcpClient(asio::io_context& io);
  ~TcpClient();

  Status Connect(const std::string& address, const std::string& port);
  void Stop();

  void Send(const void* data, int len);
  TcpConnectionPtr connection() { return conn_; }

  void set_conn_callback(const ConnCallback& cb) { conn_callback_ = cb; }
  void set_read_callback(const ReadCallback& cb) { read_callback_ = cb; }

  asio::io_context& io_context_;
  TcpConnectionPtr conn_;

  ConnCallback conn_callback_;
  ReadCallback read_callback_;
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CLIENT_H_