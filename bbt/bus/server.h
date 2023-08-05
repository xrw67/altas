#ifndef BBT_BUS_SERVER_H_
#define BBT_BUS_SERVER_H_

#include "asio.hpp"

#include "bbt/base/status.h"

namespace bbt {
namespace bus {

class Server {
 public:
  Server(asio::io_context& io) {}

  Status Listen(const std::string& address, const std::string& port) {
    return OkStatus();
  }
  void Serve() {}
  void Shutdown() {}
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_SERVER_H_