#ifndef CPPBOOT_NET_TCP_CONNECTION_MANAGER_H_
#define CPPBOOT_NET_TCP_CONNECTION_MANAGER_H_

#include <set>

#include "cppboot/net/callbacks.h"

namespace cppboot {
namespace net {

class TcpConnManager {
 public:
  TcpConnManager(const TcpConnManager&) = delete;
  TcpConnManager& operator=(const TcpConnManager&) = delete;

  /// Construct a connection manager.
  TcpConnManager();

  /// Add the specified connection to the manager and start it.
  void Start(const TcpConnPtr& c);

  /// Stop the specified connection.
  void Stop(const TcpConnPtr& c);

  /// Stop all connections.
  void StopAll();

  void Boardcast(const void* data, size_t len) noexcept;

 private:
  /// The managed connections.
  std::set<TcpConnPtr> connections_;  // TODO: mutex
};

}  // namespace net
}  // namespace cppboot

#endif  // CPPBOOT_NET_TCP_CONNECTION_MANAGER_H_
