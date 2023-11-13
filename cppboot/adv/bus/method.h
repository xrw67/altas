#ifndef CPPBOOT_ADV_BUS_METHOD_H_
#define CPPBOOT_ADV_BUS_METHOD_H_

#include <map>
#include <string>
#include <functional>

#include <mutex>
#include <condition_variable>

#include "cppboot/base/status.h"
#include "cppboot/net/connection.h"

namespace cppboot {

class In {
 public:
  typedef std::map<std::string, std::string> Type;

  cppboot::net::ConnPtr conn;

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
  Result() : ok_(false) {}

  void Wait() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (ok_) return;
    cond_.wait(lock, [this] { return ok_; });
  }

  void WeakUp() {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      ok_ = true;
    }
    cond_.notify_all();
  }

  Out& out() { return out_; }
  std::string get(const std::string& key) const { return out_.get(key); };
  void set(const std::string& key, const std::string& value) {
    out_.set(key, value);
  }

 private:
  std::mutex mutex_;
  std::condition_variable cond_;

  bool ok_;
  Out out_;
};

typedef std::function<void(const In&, Out*)> MethodFunc;

}  // namespace cppboot

#endif  // CPPBOOT_ADV_BUS_METHOD_H_