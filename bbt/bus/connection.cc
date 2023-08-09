#include "bbt/bus/connection.h"

#include "bbt/bus/msg_packer.h"
#include "bbt/net/buffer.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;

Status BusConnection::AddMethod(const std::string& name, MethodFunc func) {

  // TODO： 支持一次性上传多个函数
  // CALL AddMethod RPC
  In in;
  in.set("MethodName", name);

  auto st = Call("SvcMgr.AddMethod", in, NULL);
  if (!st) return st;


  return st;
}

Status BusConnection::Call(const std::string& method, const In& in, Out* out) {
  Result result;
  auto st = ACall(method, in, &result);
  if (!st) return st;

  st = result.Wait();
  if (!st) return st;

  if (out) *out = result.out();
  return st;
}

Status BusConnection::ACall(const std::string& method, const In& in,
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

  send_msg_func_(msg);

  // result->set_in(msg);
  // result->ResetWait();
  // AddWaitintList(result);

  return OkStatus();
}

void BusConnection::OnMsg(const MsgPtr& msg) {
  // 服务端发过来的rpc请求，需要响应它
  // if (msg.is_request()) {
  //   // 其他客户端的访问请求
  //   Msg resp(msg.id(), msg.caller());
  //   HandleMethodRequest(msg, &resp);
  //   send_msg_func_(resp);
  // } else {
  //   // 我的请求应答来了
  //   auto it = waitings_.find(msg.id());
  //   if (it != waitings_.end()) {
  //     auto result = it->second;

  //     result->set_out(msg);
  //     result->WeakUp();
  //   }
  // }
}

}  // namespace bus
}  // namespace bbt