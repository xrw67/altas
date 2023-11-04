#ifndef CPPBOOT_BUS_SERVICE_H_
#define CPPBOOT_BUS_SERVICE_H_

#include <string>
#include <map>

#include "cppboot/bus/method.h"

namespace cppboot {
namespace bus {

class Msg;

class BusService {
 public:
  explicit BusService(const std::string& name) : name_(name) {}

  bool AddMethod(const std::string& name, MethodFunc func);
  void ServeMsg(const Msg& req, Msg* resp);

  std::string name() const { return name_; }

 private:
  std::string name_;

  // std::mutex
  std::map<std::string, MethodFunc> methods_;
};

}  // namespace bus
}  // namespace cppboot

#endif  // CPPBOOT_BUS_SERVICE_H_