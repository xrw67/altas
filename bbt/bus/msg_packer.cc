#include "bbt/bus/msg_packer.h"

#include <exception>

#include "bbt/base/json.h"
#include "bbt/bus/msg.h"

namespace bbt {
namespace bus {

//
// JsonPacker
//

void JsonPacker::Pack(const Msg& msg, std::string* result) {
  bbt::json root;
  root["id"] = msg.id();
  root["method"] = msg.method();
  root["is_req"] = msg.is_request();
  root["caller"] = msg.caller();
  root["method_provider"] = msg.method_provider();
  bbt::json param;
  for (auto i : msg) {
    bbt::json p;
    p["t"] = i.first;
    p["v"] = i.second;
    param.push_back(p);
  }
  root["param"] = param;

  *result = root.dump();
}

Status JsonPacker::Unpack(const std::string& data, Msg* msg) {
  try {
    auto root = bbt::json::parse(data);
    msg->set_id(root["id"]);
    msg->set_method(root["method"]);
    msg->set_request(root["is_req"]);
    msg->set_caller(root["caller"]);
    msg->set_method_provider(root["method_provider"]);
    auto& param = root["param"];
    for (auto i : param) {
      msg->set_param(i["t"], i["v"]);
    }

  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

}  // namespace bus
}  // namespace bbt