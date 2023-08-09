#ifndef BBT_BUS_CLIENT_H_
#define BBT_BUS_CLIENT_H_

#include <string>
#include <atomic>
#include <mutex>
#include <functional>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/base_connection.h"

#include "method.h"
#include "msg.h"

namespace bbt {
namespace bus {

using bbt::net::BaseConnectionPtr;
using bbt::net::BaseContext;
using bbt::net::Buffer;

class BusClient {
 public:
  BusClient(const std::string& name, const BaseConnectionPtr& transport);
  ~BusClient();

  // 主动执行关闭，会传递到Transport的Close
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
  void OnTransportConnection(const BaseConnectionPtr& conn);

  // Call when transpot read bytes
  void OnTransportReadCallback(const BaseConnectionPtr& conn, Buffer* buf);

  // call when receive bus message coming
  void OnMsg(const MsgPtr& msg);

  void OnMethodRequest(const Msg&req, Msg *resp);

  // 发送msg到远程机器上
  Status WriteBusMessage(const Msg& msg);

  MsgId NextMsgId() noexcept { return next_id_.fetch_add(1); }

  std::string name_;
  BaseConnectionPtr transport_;

  std::atomic<MsgId> next_id_;

  /// 等待结果的列表
  std::mutex mutex_;
  std::map<MsgId, Result*> waitings_;  // TODO: result的析构函数要weakup再析构

  std::map<std::string, MethodFunc> methods_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CLIENT_H_