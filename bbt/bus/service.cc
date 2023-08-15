#include "bbt/bus/service.h"
#include "bbt/bus/msg.h"

namespace bbt {
namespace bus {

bool BusService::AddMethod(const std::string& name, MethodFunc func) {
  if (methods_.find(name) == methods_.end()) {
    methods_[name] = func;
    return true;
  }

  return false;
}

void BusService::ServeMsg(const Msg& req, Msg* resp) {
  auto it = methods_.find(req.method());
  if (it != methods_.end()) {
    In in;
    in.conn = req.src;
    for (auto i : req.params()) {
      in.set(i.first, i.second);
    }
    Out out;
    it->second(in, &out);

    for (auto i : out.params()) {
      resp->set_param(i.first, i.second);
    }
  } else {
    resp->set_param("return", "not found");
  }

  resp->set_id(req.id());
  resp->set_caller(req.caller());
  resp->set_request(false);
}

}  // namespace bus
}  // namespace bbt