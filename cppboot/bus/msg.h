#ifndef CPPBOOT_BUS_MSG_H_
#define CPPBOOT_BUS_MSG_H_

#include <stdint.h>
#include <string>
#include <map>

#include "cppboot/net/callbacks.h"
#include "cppboot/net/connection.h"

namespace cppboot {
namespace bus {

using cppboot::net::ConnPtr;

static const uint32_t kMsgMagic = 0x20141021;

typedef uint32_t MsgId;

struct MsgHeader {
  uint32_t magic;
  uint32_t length;
};

class Msg;
typedef std::shared_ptr<Msg> MsgPtr;
typedef std::function<void(const MsgPtr&)> BusMsgCallback;

struct Params {
  typedef std::map<std::string, std::string> Type;
  typedef Type::const_iterator TypeConstIter;

  bool has(const std::string& key) const {
    if (key.empty()) return false;
    return values_.find(key) != values_.end();
  }

  void set(const std::string& key, const std::string& value) {
    if (!key.empty()) values_[key] = value;
  }

  std::string get(const std::string& key) const {
    if (key.empty()) return "";
    auto it = values_.find(key);
    return (it != values_.end()) ? it->second : "";
  }

  TypeConstIter begin() const noexcept { return values_.begin(); }
  TypeConstIter end() const noexcept { return values_.end(); }

  Type values_;
};

class Msg {
 public:
  ConnPtr src;

  Msg() : id_(0), is_request_(true) {}

  bool is_request() const noexcept { return is_request_; }
  void set_request(bool b) { is_request_ = b; }

  std::string caller() const noexcept { return caller_; }
  void set_caller(const std::string& caller) { caller_ = caller; }

  std::string method_provider() const noexcept { return method_provider_; }
  void set_method_provider(const std::string& mp) { method_provider_ = mp; }
  MsgId id() const { return id_; }
  void set_id(MsgId id) { id_ = id; }

  std::string method() const { return method_; }

  void set_method(const std::string& method) {
    auto pos = method.find_first_of('/');
    if (pos != method.npos) {
      method_provider_ = method.substr(0, pos);
      method_ = method.substr(pos + 1);
    } else {
      method_ = method;
    }
  }

  void set_param(const std::string& key, const std::string& value) {
    params_.set(key, value);
  }
  std::string param(const std::string& key) const { return params_.get(key); }

  const Params& params() const noexcept { return params_; }

 private:
  MsgId id_;
  bool is_request_;
  std::string caller_;
  std::string method_;
  std::string method_provider_;
  Params params_;
};

void SendMessageToConnection(const ConnPtr& conn, const MsgPtr& msg);

}  // namespace bus
}  // namespace cppboot

#endif  // CPPBOOT_BUS_MSG_H_