#ifndef CPPBOOT_NET_TCP_CLIENT_H_
#define CPPBOOT_NET_TCP_CLIENT_H_

#include <string>

#include "cppboot/net/callbacks.h"

namespace cppboot {
namespace net {

class TcpClient {
 public:
  TcpClient(asio::io_context& io);
  ~TcpClient();

  Status Connect(const std::string& address, const std::string& port);
  void Stop();

  void Send(const void* data, int len);
  TcpConnPtr connection() { return conn_; }

  void set_conn_callback(const ConnCallback& cb) { conn_callback_ = cb; }
  void set_receive_callback(const ReceiveCallback& cb) {
    receive_callback_ = cb;
  }

  asio::io_context& io_context_;
  TcpConnPtr conn_;

  ConnCallback conn_callback_;
  ReceiveCallback receive_callback_;
};

}  // namespace net
}  // namespace cppboot

#endif  // CPPBOOT_NET_TCP_CLIENT_H_