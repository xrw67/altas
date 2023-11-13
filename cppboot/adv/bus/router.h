#ifndef CPPBOOT_ADV_BUS_ROUTER_H_
#define CPPBOOT_ADV_BUS_ROUTER_H_

#include <mutex>
#include <string>
#include <unordered_map>

#include "cppboot/net/callbacks.h"

namespace cppboot {

class BusRouter {
 public:
  void Add(const std::string& name, const cppboot::net::ConnPtr& conn);
  void Remove(const std::string& name);
  cppboot::net::ConnPtr Find(const std::string& name);

 private:
  std::mutex mutex_;
  std::unordered_map<std::string, cppboot::net::ConnPtr> connections_;
};
}  // namespace cppboot

#endif  // CPPBOOT_ADV_BUS_ROUTER_H_