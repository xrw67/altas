#include "bbt/bus/connection_manager.h"
#include "bbt/bus/connection.h"
namespace bbt {
namespace bus {

ConnectionManager::ConnectionManager() {}

void ConnectionManager::Start(ConnectionPtr c) {
  connections_.insert(c);
  c->set_on_close_handler(std::bind(&ConnectionManager::HandleConnectionClose,
                                    this, std::placeholders::_1));
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

void ConnectionManager::HandleConnectionClose(ConnectionPtr c) { Stop(c); }

}  // namespace bus
}  // namespace bbt