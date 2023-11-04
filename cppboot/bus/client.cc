#include "cppboot/bus/client.h"

#include <exception>

#include "cppboot/net/tcp/connection.h"
#include "cppboot/bus/method.h"
#include "cppboot/bus/msg.h"
#include "cppboot/bus/context.h"
#include "cppboot/bus/msg_packer.h"
#include "cppboot/bus/invoker.h"
#include "cppboot/bus/service.h"

namespace cppboot {
namespace bus {

using cppboot::net::_1;
using cppboot::net::_2;
using cppboot::net::Conn;

BusClient::BusClient(const std::string& name, const ConnPtr& transport)
    : service_(new BusService(name)),
      invoker_(new BusInvoker(name)),
      transport_(transport) {
  transport_->set_context(new BusContext());
  transport_->set_conn_callback(
      std::bind(&BusClient::OnTransportConnection, this, _1));
  transport_->set_receive_callback(
      std::bind(&BusClient::OnTransportReadCallback, this, _1, _2));

  invoker_->set_msg_writer(std::bind(&SendMessageToConnection, transport_, _1));
}

BusClient::~BusClient() { Stop(); }

void BusClient::Start() {
  ReportMyServiceToServer();  // ???
}

void BusClient::Stop() {
  // invoker_.SayGoodbyToSever();
  transport_->Stop();
}

Status BusClient::AddMethod(const std::string& name, MethodFunc func) {
  if (!service_->AddMethod(name, func)) return AlreadyExistsError(name);

  In in;
  in.set("MethodName", name);

  return invoker_->Call("SvcMgr/AddMethod", in, NULL);
}

Status BusClient::Call(const std::string& method, const In& in, Out* out) {
  return invoker_->Call(method, in, out);
}

Status BusClient::ACall(const std::string& method, const In& in,
                        Result* result) {
  return invoker_->ACall(method, in, result);
}

void BusClient::OnTransportConnection(const ConnPtr& conn) {
  switch (conn->state()) {
    case Conn::kConnected:
      break;
    case Conn::kDisconnected:
      // Transport断开了，善后，
      break;
  }
}

void BusClient::OnTransportReadCallback(const ConnPtr& conn, Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());

  MsgPtr msg(new Msg());
  auto ret = ctx->Parse(buf, msg.get());
  switch (ret) {
    case BusContext::kBad:
      // invoker_->ReportBadToServer()
      Stop();
      break;
    case BusContext::kGood:
      HandleMessage(msg);
      break;
    case BusContext::kContinue:
      return;  // continue;
  }
}

void BusClient::HandleMessage(const MsgPtr& msg) {
  // 服务端发过来的rpc请求，需要响应它
  if (msg->is_request()) {
    HandleRequestMessage(msg);
  } else {
    invoker_->HandleResponseMessage(msg);
  }
}

void BusClient::HandleRequestMessage(const MsgPtr& msg) {
  // 其他客户端的访问请求
  MsgPtr resp(new Msg());
  service_->ServeMsg(*msg, resp.get());
  SendMessageToConnection(transport_, resp);
}

void BusClient::ReportMyServiceToServer() {
  In in;
  in.set("ServiceName", service_->name());
  invoker_->Call("RegisterService", in, NULL);
}

}  // namespace bus
}  // namespace cppboot
