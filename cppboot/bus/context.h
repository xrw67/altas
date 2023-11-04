#ifndef CPPBOOT_BUS_CONTEXT_H_
#define CPPBOOT_BUS_CONTEXT_H_

#include <string>

#include "asio.hpp"

#include "cppboot/net/connection.h"
#include "cppboot/net/tcp/connection.h"

#include "msg.h"

namespace cppboot {
namespace bus {

using cppboot::net::Buffer;

class Msg;

class BusContext : public cppboot::net::Context {
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
}  // namespace cppboot

#endif  // CPPBOOT_BUS_CONTEXT_H_
