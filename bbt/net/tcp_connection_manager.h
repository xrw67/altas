#ifndef BBT_NET_TCP_CONNECTION_MANAGER_H_
#define BBT_NET_TCP_CONNECTION_MANAGER_H_

#include <set>

#include "bbt/net/callbacks.h"
#include "tcp_connection.h"

namespace bbt {
namespace net {

class MyTcpConnectionManager {
 public:
  typedef std::set<MyTcpConnectionPtr> Type;
  typedef Type::iterator TypeIter;

  MyTcpConnectionManager(const MyTcpConnectionManager&) = delete;
  MyTcpConnectionManager& operator=(const MyTcpConnectionManager&) = delete;

  /// Construct a connection manager.
  MyTcpConnectionManager();

  /// Add the specified connection to the manager and start it.
  void Start(const MyTcpConnectionPtr& c);

  /// Stop the specified connection.
  void Stop(const MyTcpConnectionPtr& c);

  /// Stop all connections.
  void StopAll();

  TypeIter begin() const { return connections_.begin(); }
  TypeIter end() const { return connections_.end(); }

 private:
  /// The managed connections.
  std::set<MyTcpConnectionPtr> connections_;  // TODO: mutex
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CONNECTION_MANAGER_H_
