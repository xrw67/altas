#include "cppboot/http/server/connection_manager.h"

namespace cppboot {
namespace http {

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

}  // namespace http
}  // namespace cppboot