#include "bbt/net/tcp/connection_manager.h"
#include "bbt/net/tcp/connection.h"

namespace bbt {
namespace net {

TcpConnectionManager::TcpConnectionManager() {}

void TcpConnectionManager::Start(const TcpConnectionPtr& c) {
  connections_.insert(c);
  c->Start();
}

void TcpConnectionManager::Stop(const TcpConnectionPtr& c) {
  connections_.erase(c);
  c->Stop();
}

void TcpConnectionManager::StopAll() {
  for (auto& c : connections_) c->Stop();
  connections_.clear();
}

void TcpConnectionManager::Boardcast(const void* data, size_t len) noexcept {
  for (auto& c : connections_) c->Send(data, len);
}

}  // namespace net
}  // namespace bbt