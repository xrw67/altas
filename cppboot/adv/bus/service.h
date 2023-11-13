#ifndef CPPBOOT_ADV_BUS_SERVICE_H_
#define CPPBOOT_ADV_BUS_SERVICE_H_

#include <string>
#include <map>

#include "cppboot/adv/bus/method.h"

namespace cppboot {

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

}  // namespace cppboot

#endif  // CPPBOOT_ADV_BUS_SERVICE_H_