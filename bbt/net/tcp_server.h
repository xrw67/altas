#ifndef BBT_NET_TCP_SERVER_H_
#define BBT_NET_TCP_SERVER_H_

#include <string>

#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

class TcpConnectionManager;

class TcpServer {
 public:
  explicit TcpServer(asio::io_context& io);
  ~TcpServer();

  Status Listen(const std::string& address, const std::string& port);
  void Stop();

  void Boardcast(const void* data, size_t len) noexcept;

  void set_conn_callback(const ConnCallback& cb) { conn_callback_ = cb; }
  void set_read_callback(const ReadCallback& cb) { read_callback_ = cb; }

 private:
  /// Perform an asynchronous accept operation.
  void DoAccept();

  /// The io_context used to perform asynchronous operations.
  asio::io_context& io_context_;

  /// Acceptor used to listen for incoming connections.
  asio::ip::tcp::acceptor acceptor_;

  TcpConnectionManager* connection_manager_;

  ConnCallback conn_callback_;
  ReadCallback read_callback_;
};

}  // namespace net
}  // namespace bbt
#endif  // BBT_NET_TCP_SERVER_H_