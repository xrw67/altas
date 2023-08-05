#ifndef BBT_BUS_METHOD_H_
#define BBT_BUS_METHOD_H_

#include <string>
#include <functional>
#include "bbt/base/status.h"

namespace bbt {
namespace bus {

class In {
 public:
  std::string get(const std::string& key) const { return ""; };
  void set(const std::string& key, const std::string& value) {}
};

typedef In Out;

class Result {
 public:
  Status Wait() { return OkStatus(); }
  std::string get(const std::string& key) const { return ""; };
  void set(const std::string& key, const std::string& value) {}
};

typedef std::function<void(const In&, Out*)> MethodFunc;

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_METHOD_H_