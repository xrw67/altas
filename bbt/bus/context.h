#ifndef BBT_BUS_CONTEXT_H_
#define BBT_BUS_CONTEXT_H_

#include "asio.hpp"
#include "bbt/net/base_connection.h"
#include "bbt/net/tcp_connection.h"

#include "msg.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;

class Msg;

class BusContext : public bbt::net::BaseContext {
 public:
  BusContext();

  enum Result { kContinue, kBad, kGood };

  Result Parse(Buffer* buf, Msg* msg);

 private:
  enum ParseState { kHeader, kBody };

  ParseState state_;
  MsgHeader header_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONTEXT_H_
