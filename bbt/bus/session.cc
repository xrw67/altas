#include "bbt/bus/session.h"

namespace bbt {
namespace bus {

BusSession::BusSession(const std::string& name) : name_(name), next_id_(1) {}

Status BusSession::AddMethod(const std::string& name, MethodFunc func) {
  if (methods_.find(name) != methods_.end()) return AlreadyExistsError(name);

  // TODO： 支持一次性上传多个函数
  // CALL AddMethod RPC
  In in;
  in.set("MethodName", name);

  auto st = Call("SvcMgr/AddMethod", in, NULL);
  if (!st) return st;

  // ADD LocalServiceList
  // TODO: mutex
  methods_[name] = func;

  return st;
}

Status BusSession::Call(const std::string& method, const In& in, Out* out) {
  Result result;
  auto st = ACall(method, in, &result);
  if (!st) return st;

  if (out) {
    result.Wait();
    *out = result.out();
  }
  return st;
}

Status BusSession::ACall(const std::string& method, const In& in,
                         Result* result) {
  if (!result) return InvalidArgumentError("no result param");

  // Pack Msg
  MsgPtr msg(new Msg());
  msg->set_id(NextMsgId());
  msg->set_caller(name_);
  msg->set_request(true);
  msg->set_method(method);
  for (auto i : in.params()) {
    msg->set_param(i.first, i.second);
  }

  //
  //  先加入队列，否则可能还没来得及加入队列，就返回了。
  //
  {
    std::lock_guard<std::mutex> guard(mutex_);
    waitings_[msg->id()] = result;
  }

  msg_writer_(msg);
  return OkStatus();
}

void BusSession::HandleMessage(const MsgPtr& msg) {
  // 服务端发过来的rpc请求，需要响应它
  if (msg->is_request()) {
    HandleRequestMessage(msg);
  } else {
    HandleResponseMessage(msg);
  }
}

void BusSession::HandleRequestMessage(const MsgPtr& msg) {
  // 其他客户端的访问请求
  MsgPtr resp(new Msg());
  resp->set_id(msg->id());
  resp->set_caller(msg->caller());
  resp->set_request(false);

  OnMethodRequest(*msg, resp.get());
  msg_writer_(resp);
}

void BusSession::HandleResponseMessage(const MsgPtr& msg) {
  // 我的请求应答来了
  Result* result = nullptr;

  {
    std::lock_guard<std::mutex> guard(mutex_);
    auto it = waitings_.find(msg->id());
    if (it != waitings_.end()) {
      result = it->second;
      waitings_.erase(it);
    }
  }

  if (result) {
    for (auto& i : *msg) {
      result->out().set(i.first, i.second);
    }

    result->WeakUp();
  }
}

void BusSession::OnMethodRequest(const Msg& req, Msg* resp) {
  auto it = methods_.find(req.method());
  if (it != methods_.end()) {
    In in;
    for (auto i : req) {
      in.set(i.first, i.second);
    }
    Out out;
    it->second(in, &out);

    for (auto i : out.params()) {
      resp->set_param(i.first, i.second);
    }
  } else {
    resp->set_param("return", "not found");
  }
}

}  // namespace bus
}  // namespace bbt
