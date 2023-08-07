#include "bbt/bus/client.h"

#include <exception>

#include "bbt/bus/connection.h"
#include "bbt/bus/method.h"
#include "bbt/bus/msg.h"

namespace bbt {
namespace bus {

TcpClient::TcpClient(asio::io_context& ioctx)
    : io_context_(ioctx), socket_(ioctx) {}

Status TcpClient::Connect(const std::string& address, const std::string& port) {
  try {
    asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(address, port);
    asio::connect(socket_, endpoints);

  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

//
// Client
//

Client::Client(const std::string& name, asio::io_context& io)
    : name_(name), tcp_(io) {}

Client::~Client() { Close(); }

Status Client::Connect(const std::string& address, const std::string& port) {
  tcp_.Connect(address, port);

  conn_ = std::make_shared<BusConnection>(
      std::move(tcp_.socket_),
      std::bind(&Client::HandleMsg, this, std::placeholders::_1));
  // TODO: conn_->set_on_close_handler();
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
  conn_->Send(msg);

  // result->set_in(msg);
  // result->ResetWait();
  // AddWaitintList(result);

  return OkStatus();
}

MsgId Client::NextMsgId() noexcept { return next_id_.fetch_add(1); }

void Client::HandleMsg(const MsgPtr& msg) {
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
