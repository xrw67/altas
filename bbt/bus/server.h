#ifndef BBT_BUS_SERVER_H_
#define BBT_BUS_SERVER_H_

#include "bbt/base/status.h"

#include "asio.hpp"
#include "bbt/net/tcp_server.h"
#include "bbt/net/tcp_connection_manager.h"
#include "bbt/bus/msg.h"

namespace bbt {

namespace bus {

using bbt::net::Buffer;
using bbt::net::ConnectionManager;
using bbt::net::TcpConnection;
using bbt::net::TcpConnectionPtr;
using bbt::net::TcpServer;

class Server {
 public:
  explicit Server(const std::string& name, asio::io_context& ioctx);
  ~Server();
  Status Listen(const std::string& address, const std::string& port);
  void Stop();

 protected:
  // call when acceptor get a new connection
  void OnNewConnection(std::error_code ec, asio::ip::tcp::socket socket);
  // call when connection state changed
  void OnConnection(const TcpConnectionPtr& conn);
  // call when connection read bytes
  void OnMessage(const TcpConnectionPtr& conn, Buffer* buf);

  // call when receive bus message coming
  void OnBusMsg(const TcpConnectionPtr& conn, const MsgPtr& msg);

  /// Server name, used by multi server enviroment;
  std::string name_;

  TcpServer tcp_;

  /// The connection manager which owns all live connections.
  std::unique_ptr<ConnectionManager> connection_manager_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_SERVER_H_