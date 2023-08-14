#ifndef BBT_BUS_SERVER_H_
#define BBT_BUS_SERVER_H_

#include <set>
#include <memory>
#include <unordered_map>

#include "asio.hpp"
#include "bbt/base/status.h"
#include "bbt/net/tcp/server.h"
#include "bbt/bus/msg.h"
#include "bbt/bus/method.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;
using bbt::net::ConnectionPtr;

class BusService;
class BusRouter;

class BusServer {
 public:
  explicit BusServer(const std::string& name);
  ~BusServer();

  void Stop();

  // call when connection state changed
  void HandleConnection(const ConnectionPtr& conn);
  // call when connection read bytes
  void OnReceive(const ConnectionPtr& conn, Buffer* buf);

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
}  // namespace bbt

#endif  // BBT_BUS_SERVER_H_