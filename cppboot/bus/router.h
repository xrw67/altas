#ifndef CPPBOOT_BUS_ROUTER_H_
#define CPPBOOT_BUS_ROUTER_H_

#include <mutex>
#include <string>
#include <unordered_map>

#include "cppboot/net/callbacks.h"

namespace cppboot {
namespace bus {

class BusRouter {
 public:
  void Add(const std::string& name, const cppboot::net::ConnPtr& conn);
  void Remove(const std::string& name);
  cppboot::net::ConnPtr Find(const std::string& name);

 private:
  std::mutex mutex_;
  std::unordered_map<std::string, cppboot::net::ConnPtr> connections_;
};
}  // namespace bus
}  // namespace cppboot

#endif  // CPPBOOT_BUS_ROUTER_H_