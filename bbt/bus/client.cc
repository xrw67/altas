#include "bbt/bus/client.h"

#include <exception>

#include "bbt/net/tcp_connection.h"
#include "bbt/bus/method.h"
#include "bbt/bus/msg.h"
#include "bbt/bus/context.h"

namespace bbt {
namespace bus {

using bbt::net::_1;
using bbt::net::_2;

//
// Client
//

Client::Client(const std::string& name, asio::io_context& io)
    : name_(name), tcp_(io) {}

Client::~Client() { Close(); }

Status Client::Connect(const std::string& address, const std::string& port) {
  tcp_.Connect(address, port);

  conn_ = std::make_shared<Connection>(std::move(tcp_.socket_));
  conn_->set_context(
      new BusContext(std::bind(&Client::OnBusMsg, this, _1, _2)));
  conn_->set_connection_callback(std::bind(&Client::OnConnection, this, _1));
  conn_->set_message_callback(std::bind(&Client::OnMessage, this, _1, _2));

  conn_->Start();
  return OkStatus();
}

void Client::Close() { conn_->Stop(); }

Status Client::RegisterMethod(const std::string& name, MethodFunc func) {
  if (methods_.find(name) != methods_.end()) return AlreadyExistsError(name);

  // TODO： 支持一次性上传多个函数
  // CALL RegisterMethod RPC
  In in;
  in.set("MethodName", name);

  auto st = Call("SvcMgr.RegisterMethod", in, NULL);
  if (!st) return st;

  // ADD LocalServiceList
  // TODO: mutex
  methods_[name] = func;
  return st;
}

Status Client::Call(const std::string& method, const In& in, Out* out) {
  Result result;
  auto st = ACall(method, in, &result);
  if (!st) return st;

  st = result.Wait();
  if (!st) return st;

  if (out) *out = result.out();
  return st;
}

Status Client::ACall(const std::string& method, const In& in, Result* result) {
  if (!result) return InvalidArgumentError("no result param");

  // Pack Message
  MsgPtr msg(new Msg());
  msg->set_id(NextMsgId());
  msg->set_caller(name_);
  msg->set_request(true);
  msg->set_method(method);
  for (auto i : in.params()) {
    msg->set_param(i.first, i.second);
  }
  // Send to server
  BusContext* ctx = reinterpret_cast<BusContext*>(conn_->context());
  ctx->Write(msg);

  // result->set_in(msg);
  // result->ResetWait();
  // AddWaitintList(result);

  return OkStatus();
}

MsgId Client::NextMsgId() noexcept { return next_id_.fetch_add(1); }

void Client::OnConnection(const ConnectionPtr& conn) {
  switch (conn->state()) {
    case Connection::kConnected:
      break;
    case Connection::kDisconnected:
      conn->Stop(); // TODO 死循环？
      break;
  }
}

void Client::OnMessage(const ConnectionPtr& conn, Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());
  if (!ctx->Parse(conn, buf)) {
    // 解析失败,
    conn->Stop();
  }
}

void Client::OnBusMsg(const ConnectionPtr& conn, const MsgPtr& msg) {
  // 服务端发过来的rpc请求，需要响应它
  // if (msg.is_request()) {
  //   // 其他客户端的访问请求
  //   Msg resp(msg.id(), msg.caller());
  //   HandleMethodRequest(msg, &resp);
  //   conn_.Send(resp);
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
