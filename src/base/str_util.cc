#include "bbt/base/str_util.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <limits>
#include <memory>

namespace bbt {

namespace {

int memcasecmp(const char* s1, const char* s2, size_t len) {
  const unsigned char* us1 = reinterpret_cast<const unsigned char*>(s1);
  const unsigned char* us2 = reinterpret_cast<const unsigned char*>(s2);

  for (size_t i = 0; i < len; i++) {
    const int diff =
        int{static_cast<unsigned char>(tolower(us1[i]))} -
        int{static_cast<unsigned char>(tolower(us2[i]))};
    if (diff != 0) return diff;
  }
  return 0;
}

}

// TODO: 性能
std::vector<std::string> StrSplit(const std::string& text, char delimiter) {
  size_t pos = 0;
  size_t new_pos = 0;

  std::vector<std::string> result;

  if (text.empty())
    return result;

  while (std::string::npos != pos) {
    pos = text.find_first_of(delimiter, new_pos);
    if (std::string::npos == pos) {  // 结束了
      result.push_back(text.substr(new_pos));
      break;
    } else {
      if (pos >= new_pos) {
        result.push_back(text.substr(new_pos, pos - new_pos));
      }
      new_pos = pos + 1;
    }
  }
  return result;
}

std::string StrTrimLeft(const std::string& s, const std::string& cutset) {
  size_t pos = s.find_first_not_of(cutset);
  return (pos == std::string::npos) ? s : s.substr(pos);
}

std::string StrTrimRight(const std::string& s, const std::string& cutset) {
  size_t pos = s.find_last_not_of(cutset);
  return (pos == std::string::npos) ? s : s.substr(0, pos + 1);
}

std::string StrTrim(const std::string& s, const std::string& cutset) {
  return StrTrimRight(StrTrimLeft(s, cutset), cutset);
}


bool EqualsIgnoreCase(string_view piece1,
                      string_view piece2) noexcept {
  return (piece1.size() == piece2.size() &&
          0 == memcasecmp(piece1.data(), piece2.data(),
                                                  piece1.size()));
}

bool StartsWithIgnoreCase(string_view text,
                          string_view prefix) noexcept {
  return (text.size() >= prefix.size()) &&
         EqualsIgnoreCase(text.substr(0, prefix.size()), prefix);
}

bool EndsWithIgnoreCase(string_view text,
                        string_view suffix) noexcept {
  return (text.size() >= suffix.size()) &&
         EqualsIgnoreCase(text.substr(text.size() - suffix.size()), suffix);
}



}  // namespace bbt
