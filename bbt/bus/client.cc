#include "bbt/bus/client.h"

#include <exception>

#include "bbt/net/tcp/connection.h"
#include "bbt/bus/method.h"
#include "bbt/bus/msg.h"
#include "bbt/bus/context.h"
#include "bbt/bus/msg_packer.h"
#include "bbt/bus/session.h"

namespace bbt {
namespace bus {

using bbt::net::_1;
using bbt::net::_2;
using bbt::net::Connection;

BusClient::BusClient(const std::string& name, const ConnectionPtr& transport)
    : session_(new BusSession(name)), transport_(transport) {
  transport_->set_context(new BusContext());
  transport_->set_connection_callback(
      std::bind(&BusClient::OnTransportConnection, this, _1));
  transport_->set_read_callback(
      std::bind(&BusClient::OnTransportReadCallback, this, _1, _2));

  session_->set_msg_writer(std::bind(&SendMessageToConnection, transport_, _1));
}

BusClient::~BusClient() { Stop(); }

void BusClient::Stop() {
  // bus_conn_.SayGoodbyToSever();
  transport_->Stop();
}

Status BusClient::AddMethod(const std::string& name, MethodFunc func) {
  return session_->AddMethod(name, func);
}

Status BusClient::Call(const std::string& method, const In& in, Out* out) {
  return session_->Call(method, in, out);
}

Status BusClient::ACall(const std::string& method, const In& in,
                        Result* result) {
  return session_->ACall(method, in, result);
}

void BusClient::OnTransportConnection(const ConnectionPtr& conn) {
  switch (conn->state()) {
    case Connection::kConnected:
      // bus_conn_.SayHelloToServer(); 上报我的姓名
      break;
    case Connection::kDisconnected:
      // Transport断开了，善后，
      break;
  }
}

void BusClient::OnTransportReadCallback(const ConnectionPtr& conn,
                                        Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());

  MsgPtr msg(new Msg());
  auto ret = ctx->Parse(buf, msg.get());
  switch (ret) {
    case BusContext::kBad:
      // bus_conn_->ReportBadToServer()
      Stop();
      break;
    case BusContext::kGood:
      session_->HandleMessage(msg);
      break;
    case BusContext::kContinue:
      return;  // continue;
  }
}

}  // namespace bus
}  // namespace bbt
