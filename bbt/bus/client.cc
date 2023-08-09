#include "bbt/bus/client.h"

#include <exception>

#include "bbt/net/tcp_connection.h"
#include "bbt/bus/method.h"
#include "bbt/bus/msg.h"
#include "bbt/bus/context.h"
#include "bbt/bus/msg_packer.h"

namespace bbt {
namespace bus {

using bbt::net::_1;
using bbt::net::_2;
using bbt::net::BaseConnection;

//
// BusClient
//

BusClient::BusClient(const std::string& name, const BaseConnectionPtr& transport)
    : name_(name), transport_(transport), next_id_(1) {
  transport_->set_context(new BusContext());
  transport_->set_conn_callback(
      std::bind(&BusClient::OnTransportConnection, this, _1));
  transport_->set_read_callback(
      std::bind(&BusClient::OnTransportReadCallback, this, _1, _2));
}

BusClient::~BusClient() { Stop(); }

void BusClient::Stop() {
  // bus_conn_.SayGoodbyToSever();
  transport_->Stop();
}

Status BusClient::AddMethod(const std::string& name, MethodFunc func) {
  if (methods_.find(name) != methods_.end())
    return AlreadyExistsError(name);

  // TODO： 支持一次性上传多个函数
  // CALL AddMethod RPC
  In in;
  in.set("MethodName", name);

  auto st = Call("SvcMgr.AddMethod", in, NULL);
  if (!st) return st;

  // ADD LocalServiceList
  // TODO: mutex
  methods_[name] = func;

  return st;
}

Status BusClient::Call(const std::string& method, const In& in, Out* out) {
  Result result;
  auto st = ACall(method, in, &result);
  if (!st) return st;

  st = result.Wait();
  if (!st) return st;

  if (out) *out = result.out();
  return st;
}

Status BusClient::ACall(const std::string& method, const In& in,
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

  auto st = WriteBusMessage(*msg);
  if (!st) {
    waitings_.erase(msg->id());
    return st;
  }

  // result->set_in(msg);
  // result->ResetWait();

  return OkStatus();
}

void BusClient::OnTransportConnection(const BaseConnectionPtr& conn) {
  switch (conn->state()) {
    case BaseConnection::kConnected:
      // bus_conn_.SayHelloToServer(); 上报我的姓名
      break;
    case BaseConnection::kDisconnected:
      // Transport断开了，善后，
      break;
  }
}

void BusClient::OnTransportReadCallback(const BaseConnectionPtr& conn, Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());

  MsgPtr msg(new Msg());
  auto ret = ctx->Parse(buf, msg.get());
  switch (ret) {
    case BusContext::kBad:
      // bus_conn_->ReportBadToServer()
      Stop();
      break;
    case BusContext::kGood:
      OnMsg(msg);
      break;
    case BusContext::kContinue:
      return;  // continue;
  }
}

void BusClient::OnMsg(const MsgPtr& msg) {
  // 服务端发过来的rpc请求，需要响应它
  if (msg->is_request()) {
    // 其他客户端的访问请求
    Msg resp;
    resp.set_id(msg->id());
    resp.set_caller(msg->caller());
    resp.set_request(false);

    OnMethodRequest(*msg, &resp);
    WriteBusMessage(resp);

    //   send_msg_func_(resp);
  } else {
    //    我的请求应答来了
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
}

Status BusClient::WriteBusMessage(const Msg& msg) {
  bbt::net::Buffer buffer;

  JsonPacker jp;

  std::string body;  // TODO 性能优化
  jp.Pack(msg, &body);

  MsgHeader hdr = {
      .magic = kMsgMagic,
      .length = (uint32_t)body.length() + 1,
  };

  Buffer tmp(sizeof(hdr) + body.length() + 1);
  tmp.Append((const char*)&hdr, sizeof(hdr));
  tmp.Append(body.data(), body.length() + 1);

  transport_->Send(tmp.Peek(), tmp.ReadableBytes());
  return OkStatus();
}

void BusClient::OnMethodRequest(const Msg& req, Msg* resp) {
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
