#include "cppboot/adv/bus/invoker.h"

namespace cppboot {

BusInvoker::BusInvoker(const std::string& name) : name_(name), next_id_(1) {}

Status BusInvoker::Call(const std::string& method, const In& in, Out* out) {
  Result result;
  auto st = ACall(method, in, &result);
  if (!st) return st;

  if (out) {
    result.Wait();
    *out = result.out();
  }
  return st;
}

Status BusInvoker::ACall(const std::string& method, const In& in,
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

void BusInvoker::HandleResponseMessage(const MsgPtr& msg) {
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
    for (auto& i : msg->params()) {
      result->out().set(i.first, i.second);
    }

    result->WeakUp();
  }
}
}  // namespace cppboot
