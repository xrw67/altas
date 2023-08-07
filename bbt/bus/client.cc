#include "bbt/bus/client.h"

#include <exception>

#include "bbt/bus/method.h"
#include "bbt/bus/msg_packer.h"

namespace bbt {
namespace bus {

Client::Client(const std::string& name, asio::io_context& io_context)
    : name_(name), io_context_(io_context), socket_(io_context) {}

Client::~Client() { socket_.close(); }

Status Client::Connect(const std::string& address, const std::string& port) {
  try {
    asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(address, port);
    asio::connect(socket_, endpoints);

    DoRead();
  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

void Client::Close() { socket_.close(); }

Status Client::RegisterMethod(const std::string& name, MethodFunc func) {
  if (services_.find(name) != services_.end())
    return AlreadyExistsError(name);

  // TODO： 支持一次性上传多个函数
  // CALL RegisterMethod RPC
  In in;
  in.set("MethodName", name);

  auto st = Call("SvcMgr.RegisterMethod", in, NULL);
  if (!st) return st;

  // ADD LocalServiceList
  // TODO: mutex
  services_[name] = func;
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
  Msg msg(NextMsgId());
  msg.set_method(method);
  for (auto i : in.params()) {
    msg.Set(i.first, i.second);
  }

  JsonPacker packer;
  std::string buffer;
  packer.Pack(msg, &buffer);

  // Send to server

  // Add waiting list for response & timeout

  // Return result

  return OkStatus();
}

Msg::Id Client::NextMsgId() noexcept { return 0; }

void Client::DoRead() {}

void Client::DoWrite() {}

}  // namespace bus
}  // namespace bbt