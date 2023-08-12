#include "bbt/bus/server.h"
#include "bbt/net/tcp_connection.h"

#include "bbt/bus/context.h"

namespace bbt {
namespace bus {

using bbt::net::_1;
using bbt::net::_2;
using bbt::net::Connection;

//
// BusServer
//
BusServer::BusServer(const std::string& name) : name_(name) {}

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
      HandleBusMsg(conn, msg);
      break;
    case BusContext::kContinue:
      return;  // 继续接收数据
  }
}

void BusServer::HandleBusMsg(const ConnectionPtr&, const MsgPtr& msg) {
  // 分发给具体的Client的服务来处理
}

}  // namespace bus
}  // namespace bbt