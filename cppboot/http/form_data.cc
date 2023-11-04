#include "cppboot/http/form_data.h"

#include <sstream>

namespace cppboot {
namespace http {

namespace {

// TODO: depulicate
std::string GetNextToken(const char** next, const char* end, char quota) {
  const char* p = *next;
  const char* start = p;

  if (p >= end) return std::string();

  while (p != end && *p != quota) p++;
  std::string token(start, p - start);

  *next = ++p;
  return token;
}

}  // namespace

void FormData::Set(const char* key, const std::string& value) noexcept {
  if (key && *key) values_[key] = value;
}

std::string FormData::Get(const char* key) const noexcept {
  if (key && *key) {
    auto it = values_.find(key);
    if (it != values_.end()) return it->second;
  }
  return {};
}

Status FormData::Parse(string_view query) noexcept {
  const char* next = query.data();
  const char* end = query.data() + query.length();

  for (;;) {
    auto param = GetNextToken(&next, end, '&');
    if (param.empty()) {
      break;
    }
    auto sep = param.find('=');
    if (sep == param.npos) {
      values_[param] = "";
    } else {
      values_[param.substr(0, sep)] = param.substr(sep + 1);
    }
  }
  return OkStatus();
}

std::string FormData::Encode() const noexcept {
  std::stringstream ss;
  bool add_delimiter = false;

  for (auto i : values_) {
    if (add_delimiter)
      ss << "&";
    else
      add_delimiter = true;

    ss << i.first << "=" << i.second;
  }
  return ss.str();
}

}  // namespace http
}  // namespace cppboot