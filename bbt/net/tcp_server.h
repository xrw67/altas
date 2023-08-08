#ifndef BBT_NET_TCP_SERVER_H_
#define BBT_NET_TCP_SERVER_H_

#include <string>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

typedef std::function<void(std::error_code ec, asio::ip::tcp::socket socket)>
    NewConnectionCallback;

class TcpServer {
 public:
  TcpServer(asio::io_context& ioctx);

  Status Listen(const std::string& address, const std::string& port);
  void Stop();

  void set_new_connection_callback(const NewConnectionCallback& cb) {
    new_connection_callback_ = cb;
  }

  /// Perform an asynchronous accept operation.
  void DoAccept();

  /// The io_context used to perform asynchronous operations.
  asio::io_context& io_context_;

  /// Acceptor used to listen for incoming connections.
  asio::ip::tcp::acceptor acceptor_;

  NewConnectionCallback new_connection_callback_;
};
}  // namespace net
}  // namespace bbt
#endif  // BBT_NET_TCP_SERVER_H_