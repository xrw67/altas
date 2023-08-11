#ifndef BBT_BUS_SERVER_H_
#define BBT_BUS_SERVER_H_

#include <set>

#include "asio.hpp"
#include "bbt/base/status.h"
#include "bbt/net/tcp_server.h"
#include "bbt/bus/msg.h"

namespace bbt {

namespace bus {

using bbt::net::ConnectionPtr;
using bbt::net::Buffer;

class BusServer {
 public:
  explicit BusServer(const std::string& name);
  ~BusServer();

  void Stop();

  // call when connection state changed
  void HandleConnection(const ConnectionPtr& conn);
  // call when connection read bytes
  void HandleRead(const ConnectionPtr& conn, Buffer* buf);

 protected:
  // call when receive bus message coming
  void HandleBusMsg(const ConnectionPtr& conn, const MsgPtr& msg);

  /// BusServer name, used by multi server enviroment;
  std::string name_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_SERVER_H_