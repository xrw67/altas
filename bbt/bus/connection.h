#ifndef BBT_BUS_CONNECTION_H_
#define BBT_BUS_CONNECTION_H_

#include <map>
#include <string>
#include <atomic>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/tcp_connection.h"
#include "bbt/bus/msg.h"

#include "bbt/bus/method.h"

namespace bbt {
namespace bus {

typedef std::function<void(const MsgPtr&)> SendMsgFunc;

class BusConnection {
 public:
  /// 等待结果的列表
  std::map<MsgId, Result*> waitings_;

  BusConnection(const std::string& name, asio::io_context& io)
      : name_(name), io_context_(io) {}

  Status AddMethod(const std::string& name, MethodFunc func);

  Status Call(const std::string& method, const In& in, Out* out);
  Status ACall(const std::string& method, const In& in, Result* result);

  // call when receive bus message coming
  void OnMsg(const MsgPtr& msg);

  void set_send_msg_func(const SendMsgFunc& w) { send_msg_func_ = w; }

 private:
  MsgId NextMsgId() noexcept { return next_id_.fetch_add(1); }

  std::string name_;
  asio::io_context& io_context_;  // TODO： 用于执行定时器、队列任务等等
  std::atomic<MsgId> next_id_;
  SendMsgFunc send_msg_func_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONNECTION_H_