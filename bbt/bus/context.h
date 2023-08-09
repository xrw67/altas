#ifndef BBT_BUS_CONTEXT_H_
#define BBT_BUS_CONTEXT_H_

#include "asio.hpp"
#include "bbt/net/tcp_connection.h"

#include "msg.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;

class Msg;

// 作为TcpConnect的上下文对象 ，每个上下文必须有一个，避免多线程冲突
class BusContext : public bbt::net::TcpConnection::Context {
 public:
  BusContext();

  enum Result { kContinue, kBad, kGood };

  Result Parse(Buffer* buf, Msg* msg);

 private:
  enum ParseState { kHeader, kBody };

  ParseState state_;
  MessageHeader header_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONTEXT_H_
