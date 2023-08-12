#ifndef BBT_BUS_SERVICE_MANAGER_H_
#define BBT_BUS_SERVICE_MANAGER_H_

#include <unordered_map>

#include "bbt/net/callbacks.h"


namespace bbt {
namespace bus {

class ServiceManager {
 public:
  ServiceManager(const std::string& name) : name_(name) {}
  void Serve(const std::string method, const In& in, Out* out){}

  void HandleRegisterMethod(const In& in, Out* out) {}

  void HandleUnegisterMethod(const In& in, Out* out) {}

  std::string name_;

};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_SERVICE_MANAGER_H_