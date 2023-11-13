#ifndef CPPBOOT_IO_HTTP_SERVER_H_
#define CPPBOOT_IO_HTTP_SERVER_H_

#include <functional>

#include "asio.hpp"

#include "cppboot/base/status.h"
#include "cppboot/base/string_view.h"

#include "cppboot/net/http/server/connection_manager.h"
#include "cppboot/net/http/server/serve_mux.h"
#include "cppboot/net/http/request.h"
#include "cppboot/net/http/response.h"

namespace cppboot {
namespace http {

class Server {
 public:
  Server();
  ~Server();

  void Handle(const std::string& path, const ServeMux::Func& func);
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
  ServeMux request_handler_;
};

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_IO_HTTP_SERVER_H_