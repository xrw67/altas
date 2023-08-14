#ifndef BBT_BUS_CONTEXT_H_
#define BBT_BUS_CONTEXT_H_

#include <string>

#include "asio.hpp"

#include "bbt/net/connection.h"
#include "bbt/net/tcp/connection.h"

#include "msg.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;

class Msg;

class BusContext : public bbt::net::Context {
 public:
  BusContext();

  enum Result { kContinue, kBad, kGood };

  void set_name(const std::string& name) { name_ = name; }
  std::string name() const noexcept { return name_; }
  Result Parse(Buffer* buf, Msg* msg);

 private:
  enum ParseState { kHeader, kBody };

  std::string name_;
  ParseState state_;
  MsgHeader header_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONTEXT_H_
