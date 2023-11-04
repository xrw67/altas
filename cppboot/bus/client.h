#ifndef CPPBOOT_BUS_CLIENT_H_
#define CPPBOOT_BUS_CLIENT_H_

#include <string>
#include <memory.h>

#include "asio.hpp"

#include "cppboot/base/status.h"
#include "cppboot/net/connection.h"
#include "cppboot/bus/msg.h"
#include "cppboot/bus/method.h"

namespace cppboot {
namespace bus {

using cppboot::net::Buffer;
using cppboot::net::ConnPtr;
using cppboot::net::Context;

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
}  // namespace cppboot

#endif  // CPPBOOT_BUS_CLIENT_H_