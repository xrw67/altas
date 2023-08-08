#ifndef BBT_BUS_CLIENT_H_
#define BBT_BUS_CLIENT_H_

#include <string>
#include <map>
#include <atomic>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/tcp_client.h"

#include "method.h"
#include "msg.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;
using bbt::net::TcpClient;
using bbt::net::Connection;
using bbt::net::ConnectionPtr;

class Client {
 public:
  Client(const std::string& name, asio::io_context& io);
  ~Client();

  Status Connect(const std::string& address, const std::string& port);
  void Close();

  Status RegisterMethod(const std::string& name, MethodFunc func);

  Status Call(const std::string& method, const In& in, Out* out);
  Status ACall(const std::string& method, const In& in, Result* result);

 private:
  MsgId NextMsgId() noexcept;

  // call when connection state changed
  void OnConnection(const ConnectionPtr& conn);
  // call when connection read bytes
  void OnMessage(const ConnectionPtr& conn, Buffer* buf);
  // call when receive bus message coming
  void OnBusMsg(const ConnectionPtr& conn, const MsgPtr& msg);

  std::string name_;
  TcpClient tcp_;
  ConnectionPtr conn_;

  std::map<std::string, MethodFunc> methods_;

  std::atomic<MsgId> next_id_;

  /// 等待结果的列表
  std::map<MsgId, Result*> waitings_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CLIENT_H_