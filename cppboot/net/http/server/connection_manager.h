#ifndef CPPBOOT_NET_HTTP_CONNECTION_MANAGER_H_
#define CPPBOOT_NET_HTTP_CONNECTION_MANAGER_H_

#include <set>

#include "cppboot/net/http/server/connection.h"

namespace cppboot {
namespace http {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class ConnectionManager {
 public:
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;

  /// Construct a connection manager.
  ConnectionManager();

  /// Add the specified connection to the manager and start it.
  void Start(TcpConnectionPtr c);

  /// Stop the specified connection.
  void Stop(TcpConnectionPtr c);

  /// Stop all connections.
  void StopAll();

 private:
  /// The managed connections.
  std::set<TcpConnectionPtr> connections_;
};

}  // namespace http
}  // namespace cppboot
#endif  // CPPBOOT_NET_HTTP_CONNECTION_MANAGER_H_
