#include "bbt/bus/server.h"
#include "bbt/net/tcp/connection.h"

#include "bbt/base/str_util.h"

#include "bbt/bus/context.h"
#include "bbt/bus/service_manager.h"

namespace bbt {
namespace bus {

using bbt::net::_1;
using bbt::net::_2;
using bbt::net::Connection;

BusServer::BusServer(const std::string& name)
    : service_manager_(new ServiceManager(name)) {}

BusServer::~BusServer() {}

void BusServer::Stop() {
  // TODO: say baybay to all client
}

void BusServer::HandleConnection(const ConnectionPtr& conn) {
  switch (conn->state()) {
    case Connection::kConnected:
      conn->set_context(new BusContext());
      break;
    case Connection::kDisconnected:

      break;
  }
}

void BusServer::HandleRead(const ConnectionPtr& conn, Buffer* buf) {
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
    In in;
    for (auto i : *req) {
      in.set(i.first, i.second);
    }
    Out out;
    service_manager_->Serve(req->method(), in, &out);

    MsgPtr resp(new Msg());
    resp->set_id(req->id());
    resp->set_caller(req->caller());
    resp->set_request(false);
    for (auto i : out.params()) {
      resp->set_param(i.first, i.second);
    }
    SendMessageToConnection(req->src, resp);
  } else {
    auto it = connections_.find(req->caller());
    if (it != connections_.end()) {
      SendMessageToConnection(it->second, req);
    } else {
      // 客户端不存在，记录日志
      // ReplyMethodProviderNotFound(msg);
    }
  }
}

void BusServer::HandleResponseMessage(const MsgPtr& msg) {
  auto it = connections_.find(msg->caller());
  if (it != connections_.end()) {
    SendMessageToConnection(it->second, msg);
  } else {
    // 客户端丢失，记录日志
  }
}

}  // namespace bus
}  // namespace bbt