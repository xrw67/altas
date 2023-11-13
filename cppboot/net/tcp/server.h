#ifndef CPPBOOT_NET_TCP_SERVER_H_
#define CPPBOOT_NET_TCP_SERVER_H_

#include <string>

#include "cppboot/net/callbacks.h"

namespace cppboot {
namespace net {

class TcpConnManager;

class TcpServer {
 public:
  explicit TcpServer(asio::io_context& io);
  ~TcpServer();

  Status Listen(const std::string& address, const std::string& port);
  void Stop();

  void Boardcast(const void* data, size_t len) noexcept;

  void set_conn_callback(const ConnCallback& cb) { conn_callback_ = cb; }
  void set_receive_callback(const ReceiveCallback& cb) {
    receive_callback_ = cb;
  }

 private:
  /// Perform an asynchronous accept operation.
  void DoAccept();

  /// The io_context used to perform asynchronous operations.
  asio::io_context& io_context_;

  /// Acceptor used to listen for incoming connections.
  asio::ip::tcp::acceptor acceptor_;

  TcpConnManager* connection_manager_;

  ConnCallback conn_callback_;
  ReceiveCallback receive_callback_;
};

}  // namespace net
}  // namespace cppboot
#endif  // CPPBOOT_NET_TCP_SERVER_H_