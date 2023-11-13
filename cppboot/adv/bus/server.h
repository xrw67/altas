#ifndef CPPBOOT_ADV_BUS_SERVER_H_
#define CPPBOOT_ADV_BUS_SERVER_H_

#include <set>
#include <memory>
#include <unordered_map>

#include "asio.hpp"
#include "cppboot/base/status.h"
#include "cppboot/net/tcp/server.h"
#include "cppboot/adv/bus/msg.h"
#include "cppboot/adv/bus/method.h"

namespace cppboot {

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

}  // namespace cppboot

#endif  // CPPBOOT_ADV_BUS_SERVER_H_