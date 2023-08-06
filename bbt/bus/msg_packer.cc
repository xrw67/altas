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

  const auto& data = msg.data();
  if (!data.empty()) {
    bbt::json param;
    for (auto i : data) {
      bbt::json p;
      p["t"] = i.first;
      p["v"] = i.second;
      param.push_back(p);
    }
    root["param"] = param;
  }
  *result = root.dump();
}

Status JsonPacker::Unpack(const std::string& data, Msg* msg) {
  try {
    auto root = bbt::json::parse(data);
    msg->set_id(root["id"]);
    msg->set_method(root["method"]);

    auto& param = root["param"];
    for (auto i: param) {
      msg->Set(i["t"], i["v"]);
    }

  } catch (std::exception& e) {
    return InvalidArgumentError(e.what());
  }
  return OkStatus();
}

}  // namespace bus
}  // namespace bbt