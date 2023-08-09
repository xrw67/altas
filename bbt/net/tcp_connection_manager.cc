#include "bbt/net/tcp_connection_manager.h"
#include "bbt/net/tcp_connection.h"

namespace bbt {
namespace net {

ConnectionManager::ConnectionManager() {}

void ConnectionManager::Start(TcpConnectionPtr c) {
  connections_.insert(c);
  c->Start();
}

void ConnectionManager::Stop(TcpConnectionPtr c) {
  connections_.erase(c);
  c->Stop();
}

void ConnectionManager::StopAll() {
  for (auto c : connections_) c->Stop();
  connections_.clear();
}

}  // namespace net
}  // namespace bbt