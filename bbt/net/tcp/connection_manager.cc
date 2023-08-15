#include "bbt/net/tcp/connection_manager.h"
#include "bbt/net/tcp/connection.h"

namespace bbt {
namespace net {

TcpConnManager::TcpConnManager() {}

void TcpConnManager::Start(const TcpConnPtr& c) {
  connections_.insert(c);
  c->Start();
}

void TcpConnManager::Stop(const TcpConnPtr& c) {
  connections_.erase(c);
  c->Stop();
}

void TcpConnManager::StopAll() {
  for (auto& c : connections_) c->Stop();
  connections_.clear();
}

void TcpConnManager::Boardcast(const void* data, size_t len) noexcept {
  for (auto& c : connections_) c->Send(data, len);
}

}  // namespace net
}  // namespace bbt