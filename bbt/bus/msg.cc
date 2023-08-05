#include "bbt/bus/msg.h"

#include "bbt/base/json.h"

namespace bbt {
namespace bus {

bool Msg::Has(const std::string& key) const {
  return !key.empty() && values_.find(key) != values_.end();
}

void Msg::Set(const std::string& key, const std::string& value) {
  if (key.empty()) return;
  values_[key] = value;
}

std::string Msg::Get(const std::string& key) const {
  if (key.empty()) return "";
  auto it = values_.find(key);
  return (it != values_.end()) ? it->second : "";
}

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
      param[i.first] = i.second;
    }
    root["param"] = param;
  }
  *result = root.dump();
}

Status JsonPacker::UnPack(const std::string& data, Msg* msg) {
  return UnimplementedError("");
}

}  // namespace bus
}  // namespace bbt