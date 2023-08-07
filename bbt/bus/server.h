#ifndef BBT_BUS_SERVER_H_
#define BBT_BUS_SERVER_H_

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/bus/connection_manager.h"
namespace bbt {
namespace bus {

class Server {
 public:
  explicit Server(const std::string& name, asio::io_context& io_context);
  ~Server();
  Status Listen(const std::string& address, const std::string& port);
  void Shutdown();

 protected:
  /// Perform an asynchronous accept operation.
  void DoAccept();

  void HandleMsg(const Msg& in, Msg* out);

  /// Server name, used by multi server enviroment;
  std::string name_;

  /// The io_context used to perform asynchronous operations.
  asio::io_context& io_context_;

  /// Acceptor used to listen for incoming connections.
  asio::ip::tcp::acceptor acceptor_;

  /// The connection manager which owns all live connections.
  ConnectionManager connection_manager_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_SERVER_H_