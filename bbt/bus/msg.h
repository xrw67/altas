#ifndef BBT_BUS_MSG_H_
#define BBT_BUS_MSG_H_

#include <stdint.h>
#include <string>
#include <map>

#include "bbt/net/callbacks.h"
#include "bbt/net/connection.h"

namespace bbt {
namespace bus {

using bbt::net::ConnectionPtr;

static const uint32_t kMsgMagic = 0x20141021;

typedef uint32_t MsgId;

struct MsgHeader {
  uint32_t magic;
  uint32_t length;
};

class Msg;
typedef std::shared_ptr<Msg> MsgPtr;
typedef std::function<void(const MsgPtr&)> BusMsgCallback;

class Msg {
 public:
  typedef std::map<std::string, std::string> Type;
  typedef Type::const_iterator TypeConstIter;

  ConnectionPtr src;

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

  bool has_param(const std::string& key) const;
  void set_param(const std::string& key, const std::string& value);
  std::string param(const std::string& key) const;

  TypeConstIter begin() const noexcept { return values_.begin(); }
  TypeConstIter end() const noexcept { return values_.end(); }

 private:
  MsgId id_;
  bool is_request_;
  std::string caller_;
  std::string method_;
  std::string method_provider_;
  Type values_;
};

void SendMessageToConnection(const ConnectionPtr& conn, const MsgPtr& msg);

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_MSG_H_