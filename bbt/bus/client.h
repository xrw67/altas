#ifndef BBT_BUS_CLIENT_H_
#define BBT_BUS_CLIENT_H_

#include <string>
#include <atomic>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/tcp_client.h"

#include "method.h"
#include "msg.h"
#include "connection.h"
#include "method_mux.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;
using bbt::net::TcpClient;
using bbt::net::TcpConnection;
using bbt::net::TcpConnectionPtr;

class BusClient {
 public:
  BusClient(const std::string& name, TcpClient& tcp);
  ~BusClient();

  // tcp操作
  Status Connect(const std::string& address, const std::string& port);
  void Stop();

  // conn操作
  Status AddMethod(const std::string& name, MethodFunc func);
  Status Call(const std::string& method, const In& in, Out* out);
  Status ACall(const std::string& method, const In& in, Result* result);

 private:
  //
  // TCP的回调
  //

  // call when connection state changed
  void OnConnection(const TcpConnectionPtr& conn);
  // call when connection read bytes
  void OnMessage(const TcpConnectionPtr& conn, Buffer* buf);

  //
  // BusConnection的回调
  //

  // 发送msg到远程机器上
  void WriteBusMessage(const MsgPtr& msg);

  TcpClient& tcp_;
  BusConnection conn_;
  MethodMux mux_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CLIENT_H_