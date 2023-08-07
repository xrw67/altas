#include "bbt/bus/server/connection_manager.h"

namespace bbt {
namespace bus {
    
ConnectionManager::ConnectionManager() {}

void ConnectionManager::Start(ConnectionPtr c) {
  connections_.insert(c);
  c->Start();
}

void ConnectionManager::Stop(ConnectionPtr c) {
  connections_.erase(c);
  c->Stop();
}

void ConnectionManager::StopAll() {
  for (auto c : connections_) c->Stop();
  connections_.clear();
}

}  // namespace http
}  // namespace bbt