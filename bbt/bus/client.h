#ifndef BBT_BUS_CLIENT_H_
#define BBT_BUS_CLIENT_H_

#include <string>
#include <memory.h>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/connection.h"
#include "bbt/bus/msg.h"
#include "bbt/bus/method.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;
using bbt::net::ConnPtr;
using bbt::net::Context;

class BusInvoker;
class BusService;

class BusClient {
 public:
  BusClient(const std::string& name, const ConnPtr& transport);
  ~BusClient();

  void Start();
  
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
  void OnTransportConnection(const ConnPtr& conn);

  // Call when transpot read bytes
  void OnTransportReadCallback(const ConnPtr& conn, Buffer* buf);

  void HandleMessage(const MsgPtr& msg);
  void HandleRequestMessage(const MsgPtr& msg);

  void ReportMyServiceToServer();

  std::unique_ptr<BusService> service_;
  std::unique_ptr<BusInvoker> invoker_;
  ConnPtr transport_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CLIENT_H_