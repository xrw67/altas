#include "bbt/bus/server.h"
#include "bbt/net/tcp/connection.h"

#include "bbt/base/str_util.h"
#include "bbt/base/log.h"
#include "bbt/bus/context.h"
#include "bbt/bus/service.h"
#include "bbt/bus/router.h"

namespace bbt {
namespace bus {

using bbt::net::_1;
using bbt::net::_2;
using bbt::net::Connection;

BusServer::BusServer(const std::string& name)
    : local_service_(new BusService(name)), router_(new BusRouter()) {
  local_service_->AddMethod(
      "RegisterService",
      std::bind(&BusServer::HandleRegisterMethod, this, _1, _2));
}

BusServer::~BusServer() {}

void BusServer::Stop() {
  // TODO: say baybay to all client
}

void BusServer::HandleConnection(const ConnectionPtr& conn) {
  switch (conn->state()) {
    case Connection::kConnected:
      conn->set_context(new BusContext());
      break;
    case Connection::kDisconnected: {
      BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());
      if (ctx) {
        router_->Remove(ctx->name());
      }
      break;
    }
  }
}

void BusServer::OnReceive(const ConnectionPtr& conn, Buffer* buf) {
  BusContext* ctx = reinterpret_cast<BusContext*>(conn->context());

  MsgPtr msg(new Msg());
  auto ret = ctx->Parse(buf, msg.get());
  switch (ret) {
    case BusContext::kBad:
      Stop();  // tcp。stop , conn.stop ??
      break;
    case BusContext::kGood:
      msg->src = conn;
      HandleMessage(msg);
      break;
    case BusContext::kContinue:
      return;  // 继续接收数据
  }
}

void BusServer::HandleMessage(const MsgPtr& msg) {
  if (msg->is_request()) {
    HandleRequestMessage(msg);
  } else {
    HandleResponseMessage(msg);
  }
}

void BusServer::HandleRequestMessage(const MsgPtr& req) {
  // 分发给具体的Client的服务来处理
  const auto& provider = req->method_provider();
  if (provider.empty() || provider == "SvcMgr") {
    MsgPtr resp(new Msg());
    local_service_->ServeMsg(*req, resp.get());
    SendMessageToConnection(req->src, resp);
  } else {
    auto c = router_->Find(provider);
    if (c) {
      SendMessageToConnection(c, req);
    } else {
      // 客户端不存在，记录日志
      // ReplyMethodProviderNotFound(msg);
    }
  }
}

void BusServer::HandleResponseMessage(const MsgPtr& msg) {
  auto c = router_->Find(msg->caller());
  if (c) {
    SendMessageToConnection(c, msg);
  } else {
    // 客户端丢失，记录日志
  }
}

void BusServer::HandleRegisterMethod(const In& in, Out* out) {
  auto name = in.get("ServiceName");
  router_->Add(name, in.conn);
  BBT_LOG(DEBUG, "handle {}, name is {}", "RegisterMethod", name);
}

}  // namespace bus
}  // namespace bbt