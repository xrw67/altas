#ifndef BBT_BUS_CLIENT_H_
#define BBT_BUS_CLIENT_H_

#include <string>
#include <map>
#include <atomic>

#include "asio.hpp"

#include "bbt/base/status.h"

#include "callbacks.h"
#include "method.h"

namespace bbt {
namespace bus {

class TcpClient {
 public:
  TcpClient(asio::io_context& ioctx);

  Status Connect(const std::string& address, const std::string& port);
  asio::io_context& io_context_;
  asio::ip::tcp::socket socket_; // Temp
};

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

  void HandleMsg(const MsgPtr& msg);

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