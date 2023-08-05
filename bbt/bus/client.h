#ifndef BBT_BUS_CLIENT_H_
#define BBT_BUS_CLIENT_H_

#include <string>
#include "asio.hpp"
#include "bbt/base/status.h"
#include "bbt/bus/method.h"

namespace bbt {
namespace bus {

class Client {
 public:
  Client(asio::io_context& io) {}

  Status Connect(const std::string& address, const std::string& port) {
    return OkStatus();
  }
  Status RegisterMethod(const std::string& name, MethodFunc func) {
    return OkStatus();
  }
  Status Call(const std::string& method, const In& in, Out* out) {
    return OkStatus();
  }
  Status ACall(const std::string& method, const In& in, Result* result) {
    return OkStatus();
  }
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CLIENT_H_