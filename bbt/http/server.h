#ifndef BBT_IO_HTTP_SERVER_H_
#define BBT_IO_HTTP_SERVER_H_

#include <functional>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/base/string_view.h"

#include "bbt/http/connection_manager.h"
#include "bbt/http/request_handler.h"

namespace bbt {
namespace http {

struct Request;
struct Response;

class Server {
 public:
  

  Server();
  ~Server();

  void Handle(string_view path, const RequestHandler::Handler& func);
  Status Listen(const std::string& address, const std::string& port);
  void Serve();
  void Shutdown();

 private:
  /// Perform an asynchronous accept operation.
  void DoAccept();

  /// The io_context used to perform asynchronous operations.
  asio::io_context io_context_;
  /// Acceptor used to listen for incoming connections.
  asio::ip::tcp::acceptor acceptor_;

  /// The connection manager which owns all live connections.
  ConnectionManager connection_manager_;

   /// The handler for all incoming requests.
  RequestHandler request_handler_;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_IO_HTTP_SERVER_H_