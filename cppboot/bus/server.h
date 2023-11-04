#ifndef CPPBOOT_BUS_SERVER_H_
#define CPPBOOT_BUS_SERVER_H_

#include <set>
#include <memory>
#include <unordered_map>

#include "asio.hpp"
#include "cppboot/base/status.h"
#include "cppboot/net/tcp/server.h"
#include "cppboot/bus/msg.h"
#include "cppboot/bus/method.h"

namespace cppboot {
namespace bus {

using cppboot::net::Buffer;
using cppboot::net::ConnPtr;

class BusService;
class BusRouter;

class BusServer {
 public:
  explicit BusServer(const std::string& name);
  ~BusServer();

  void Stop();

  // call when connection state changed
  void HandleConnection(const ConnPtr& conn);
  // call when connection read bytes
  void OnReceive(const ConnPtr& conn, Buffer* buf);

 protected:
  // call when receive bus message coming
  void HandleMessage(const MsgPtr& msg);
  void HandleRequestMessage(const MsgPtr& msg);
  void HandleResponseMessage(const MsgPtr& msg);

  // Service
  void HandleRegisterMethod(const In& in, Out* out);

  std::unique_ptr<BusService> local_service_;  // 服务器本身提供的服务
  std::unique_ptr<BusRouter> router_;
};

}  // namespace bus
}  // namespace cppboot

#endif  // CPPBOOT_BUS_SERVER_H_