#ifndef BBT_NET_TCP_CONNECTION_MANAGER_H_
#define BBT_NET_TCP_CONNECTION_MANAGER_H_

#include <set>

#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

class TcpConnectionManager {
 public:
  TcpConnectionManager(const TcpConnectionManager&) = delete;
  TcpConnectionManager& operator=(const TcpConnectionManager&) = delete;

  /// Construct a connection manager.
  TcpConnectionManager();

  /// Add the specified connection to the manager and start it.
  void Start(const TcpConnectionPtr& c);

  /// Stop the specified connection.
  void Stop(const TcpConnectionPtr& c);

  /// Stop all connections.
  void StopAll();

  void Boardcast(const void* data, size_t len) noexcept;

 private:
  /// The managed connections.
  std::set<TcpConnectionPtr> connections_;  // TODO: mutex
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CONNECTION_MANAGER_H_
