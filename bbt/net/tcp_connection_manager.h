#ifndef BBT_NET_TCP_CONNECTION_MANAGER_H_
#define BBT_NET_TCP_CONNECTION_MANAGER_H_

#include <set>

#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

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

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CONNECTION_MANAGER_H_
