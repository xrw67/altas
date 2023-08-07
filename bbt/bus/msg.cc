#include "bbt/bus/msg.h"

namespace bbt {
namespace bus {

bool Msg::has_param(const std::string& key) const {
  return !key.empty() && values_.find(key) != values_.end();
}

void Msg::set_param(const std::string& key, const std::string& value) {
  if (key.empty()) return;
  values_[key] = value;
}

std::string Msg::param(const std::string& key) const {
  if (key.empty()) return "";
  auto it = values_.find(key);
  return (it != values_.end()) ? it->second : "";
}

}  // namespace bus
}  // namespace bbt