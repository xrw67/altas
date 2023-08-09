#include "bbt/net/tcp_connection_manager.h"
#include "bbt/net/tcp_connection.h"

namespace bbt {
namespace net {

MyTcpConnectionManager::MyTcpConnectionManager() {}

void MyTcpConnectionManager::Start(const MyTcpConnectionPtr& c) {
  connections_.insert(c);
  c->Start();
}

void MyTcpConnectionManager::Stop(const MyTcpConnectionPtr& c) {
  connections_.erase(c);
  c->Stop();
}

void MyTcpConnectionManager::StopAll() {
  for (auto c : connections_) c->Stop();
  connections_.clear();
}

}  // namespace net
}  // namespace bbt