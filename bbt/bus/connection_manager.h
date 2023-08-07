#ifndef BBT_BUS_CONNECTION_MANAGER_H_
#define BBT_BUS_CONNECTION_MANAGER_H_

#include <set>

#include "bbt/bus/connection.h"

namespace bbt {
namespace bus {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class ConnectionManager {
 public:
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;

  /// Construct a connection manager.
  ConnectionManager();

  /// Add the specified connection to the manager and start it.
  void Start(ConnectionPtr c);

  /// Stop the specified connection.
  void Stop(ConnectionPtr c);

  /// Stop all connections.
  void StopAll();

 private:
  void HandleConnectionClose(ConnectionPtr c);

  /// The managed connections.
  std::set<ConnectionPtr> connections_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONNECTION_MANAGER_H_
