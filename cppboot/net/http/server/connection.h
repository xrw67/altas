#ifndef CPPBOOT_NET_HTTP_CONNECTION_H_
#define CPPBOOT_NET_HTTP_CONNECTION_H_

#include <array>
#include <memory>

#include "asio.hpp"

#include "cppboot/net/http/request.h"
#include "cppboot/net/http/server/request_parser.h"
#include "cppboot/net/http/server/serve_mux.h"
#include "cppboot/net/http/response.h"

namespace cppboot {
namespace http {

class ServeMux;
class ConnectionManager;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(const TcpConnection&) = delete;
  TcpConnection& operator=(const TcpConnection&) = delete;

  /// Construct a connection with the given socket.
  explicit TcpConnection(asio::ip::tcp::socket socket,
                         ConnectionManager& manager, ServeMux& handler);

  /// Start the first asynchronous operation for the connection.
  void Start();

  /// Stop all asynchronous operations associated with the connection.
  void Stop();

 private:
  /// Perform an asynchronous read operation.
  void DoRead();
  /// Perform an asynchronous write operation.
  void DoWrite();

  /// Socket for the connection.
  asio::ip::tcp::socket socket_;

  /// The manager for this connection.
  ConnectionManager& connection_manager_;

  /// The handler used to process the incoming request.
  ServeMux& request_handler_;

  /// Buffer for incoming data.
  std::array<char, 8192> buffer_;

  /// The incoming request.
  Request request_;

  /// The parser for the incoming request.
  RequestParser request_parser_;

  /// The reply to be sent back to the client.
  Response reply_;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_NET_HTTP_CONNECTION_H_