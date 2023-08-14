#include "bbt/bus/router.h"

namespace bbt {
namespace bus {

using bbt::net::ConnectionPtr;

void BusRouter::Add(const std::string& name, const ConnectionPtr& conn) {
  std::lock_guard<std::mutex> guard(mutex_);
  connections_[name] = conn;
}

void BusRouter::Remove(const std::string& name) {
  std::lock_guard<std::mutex> guard(mutex_);
  connections_.erase(name);
}

ConnectionPtr BusRouter::Find(const std::string& name) {
  std::lock_guard<std::mutex> guard(mutex_);
  auto it = connections_.find(name);
  return (it != connections_.end()) ? it->second : nullptr;
}

}  // namespace bus
}  // namespace bbt