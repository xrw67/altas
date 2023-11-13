#include "cppboot/adv/bus/router.h"

namespace cppboot {

using cppboot::net::ConnPtr;

void BusRouter::Add(const std::string& name, const ConnPtr& conn) {
  std::lock_guard<std::mutex> guard(mutex_);
  connections_[name] = conn;
}

void BusRouter::Remove(const std::string& name) {
  std::lock_guard<std::mutex> guard(mutex_);
  connections_.erase(name);
}

ConnPtr BusRouter::Find(const std::string& name) {
  std::lock_guard<std::mutex> guard(mutex_);
  auto it = connections_.find(name);
  return (it != connections_.end()) ? it->second : nullptr;
}

}  // namespace cppboot