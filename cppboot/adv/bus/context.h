#ifndef CPPBOOT_ADV_BUS_CONTEXT_H_
#define CPPBOOT_ADV_BUS_CONTEXT_H_

#include <string>

#include "asio.hpp"

#include "cppboot/net/connection.h"
#include "cppboot/net/tcp/connection.h"

#include "msg.h"

namespace cppboot {

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

}  // namespace cppboot

#endif  // CPPBOOT_ADV_BUS_CONTEXT_H_
