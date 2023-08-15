#ifndef BBT_BUS_ROUTER_H_
#define BBT_BUS_ROUTER_H_

#include <mutex>
#include <string>
#include <unordered_map>

#include "bbt/net/callbacks.h"

namespace bbt {
namespace bus {

class BusRouter {
 public:
  void Add(const std::string& name, const bbt::net::ConnPtr& conn);
  void Remove(const std::string& name);
  bbt::net::ConnPtr Find(const std::string& name);

 private:
  std::mutex mutex_;
  std::unordered_map<std::string, bbt::net::ConnPtr> connections_;
};
}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_ROUTER_H_