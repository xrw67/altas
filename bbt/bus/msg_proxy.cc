#include "bbt/bus/msg_proxy.h"

namespace bbt {
namespace bus {

using bbt::net::BaseConnectionPtr;

Status MsgProxy::AddMethod(const BaseConnectionPtr& owner,
                           const std::string& method) {
  std::lock_guard<std::mutex> guard(mutex_);
  if (methods_.find(method) != methods_.end())
    return AlreadyExistsError(method);

  methods_[method] = owner;
  auto it = conn_methods_.find(owner);
  if (it != conn_methods_.end()) {
    it->second.insert(method);
  } else {
    std::set<std::string> values;
    values.insert(method);
    conn_methods_[owner] = values;
  }
  return OkStatus();
}

void MsgProxy::RemoveMethod(const std::string& method) {
  std::lock_guard<std::mutex> guard(mutex_);
  auto conn_iter = methods_.find(method);
  if (conn_iter == methods_.end()) return;

  auto method_iter = conn_methods_.find(conn_iter->second);
  if (method_iter != conn_methods_.end()) {
    method_iter->second.erase(method);
  }
}

void MsgProxy::RemoveAllMethodsFromConnection(const BaseConnectionPtr& conn) {
  std::lock_guard<std::mutex> guard(mutex_);
  auto it = conn_methods_.find(conn);
  if (it == conn_methods_.end()) return;

  for (auto i : it->second) {
    methods_.erase(i);
  }
}

Status MsgProxy::ProxyRequestMsg(Msg* req) {
  req->dst.reset();
  
  std::lock_guard<std::mutex> guard(mutex_);
  auto it = methods_.find(req->method());
  if (it == methods_.end()) return NotFoundError(req->method());

  req->dst = it->second;
  return OkStatus();
}

}  // namespace bus
}  // namespace bbt