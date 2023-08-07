#ifndef BBT_BUS_METHOD_H_
#define BBT_BUS_METHOD_H_

#include <map>
#include <string>
#include <functional>

#include "bbt/base/status.h"

namespace bbt {
namespace bus {

class In {
 public:
  typedef std::map<std::string, std::string> Type;

  std::string get(const std::string& key) const {
    auto it = values_.find(key);
    return (it != values_.end()) ? it->second : "";
  };
  
  void set(const std::string& key, const std::string& value) {
    values_[key] = value;
  }

  const Type& params() const noexcept { return values_; }

 private:
  std::map<std::string, std::string> values_;
};

typedef In Out;

class Result {
 public:
  Status Wait() { return OkStatus(); }

  Out& out() { return out_; }
  std::string get(const std::string& key) const { return out_.get(key); };
  void set(const std::string& key, const std::string& value) {
    out_.set(key, value);
  }

 private:
  Out out_;
  ;
};

typedef std::function<void(const In&, Out*)> MethodFunc;

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_METHOD_H_