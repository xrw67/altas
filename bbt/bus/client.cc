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

BusClient::BusClient(const std::string& name, TcpClient& tcp)
    : tcp_(tcp), conn_(name, tcp.io_context()) {
  conn_.set_send_msg_func(std::bind(&BusClient::WriteBusMessage, this, _1));

  tcp_.set_connection_context(new BusContext());
  tcp_.set_connection_callback(std::bind(&BusClient::OnConnection, this, _1));
  tcp_.set_message_callback(std::bind(&BusClient::OnMessage, this, _1, _2));
}

BusClient::~BusClient() { Stop(); }

Status BusClient::Connect(const std::string& address, const std::string& port) {
  return tcp_.Connect(address, port);
}

void BusClient::Stop() {
  // conn_.SayGoodbyToSever();
  tcp_.Stop();
}

Status BusClient::AddMethod(const std::string& name, MethodFunc func) {
   if (mux_.methods_.find(name) != mux_.methods_.end()) return AlreadyExistsError(name);

  auto st = conn_.AddMethod(name, func);
  if (st) {
    // ADD LocalServiceList
    // TODO: mutex
    mux_.methods_[name] = func;
  }
  return st;
}

Status BusClient::Call(const std::string& method, const In& in, Out* out) {
  return conn_.Call(method, in, out);
}

Status BusClient::ACall(const std::string& method, const In& in,
                        Result* result) {
  return conn_.ACall(method, in, result);
}

void BusClient::OnConnection(const TcpConnectionPtr& conn) {
  switch (conn->state()) {
    case TcpConnection::kConnected:
      // conn_.SayHelloToServer(); 上报我的姓名
      break;
    case TcpConnection::kDisconnected:
      Stop();
      break;
  }
}

void BusClient::OnMessage(const TcpConnectionPtr& conn, Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());

  MsgPtr msg(new Msg());
  auto ret = ctx->Parse(buf, msg.get());
  switch (ret) {
    case BusContext::kBad:
      // conn_->ReportBadToServer()
      Stop();
      break;
    case BusContext::kGood:
      conn_.OnMsg(msg);
      break;
    case BusContext::kContinue:
      return;  // continue;
  }
}

void BusClient::WriteBusMessage(const MsgPtr& msg) {
  bbt::net::Buffer buffer;

  JsonPacker jp;

  std::string body;  // TODO 性能优化
  jp.Pack(*msg, &body);

  MessageHeader hdr = {
      .magic = kMsgMagic,
      .length = (uint32_t)body.length() + 1,
  };

  Buffer tmp(sizeof(hdr) + body.length() + 1);
  tmp.Append((const char*)&hdr, sizeof(hdr));
  tmp.Append(body.data(), body.length() + 1);

  tcp_.Send(tmp.Peek(), tmp.ReadableBytes());
}

}  // namespace bus
}  // namespace bbt
