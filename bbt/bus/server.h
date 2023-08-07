#ifndef BBT_BUS_SERVER_H_
#define BBT_BUS_SERVER_H_

#include "bbt/base/status.h"

#include "asio.hpp"
#include "callbacks.h"

namespace bbt {
namespace bus {

class ConnectionManager;

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

class Server {
 public:
  explicit Server(const std::string& name, asio::io_context& ioctx);
  ~Server();
  Status Listen(const std::string& address, const std::string& port);
  void Shutdown();

 protected:
 void OnNewConnection(std::error_code ec, asio::ip::tcp::socket socket);
  void HandleMsg(const MsgPtr& msg);

  /// Server name, used by multi server enviroment;
  std::string name_;

  TcpServer tcp_;

  /// The connection manager which owns all live connections.
  std::unique_ptr<ConnectionManager> connection_manager_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_SERVER_H_