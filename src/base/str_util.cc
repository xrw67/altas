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

// TODO: 性能
std::vector<std::string> StrSplit(const std::string& text, char delimiter) {
  size_t pos = 0;
  size_t new_pos = 0;

  std::vector<std::string> result;

  while (std::string::npos != pos) {
    pos = text.find_first_of(delimiter, new_pos);
    if (std::string::npos == pos) {  // 结束了
      if (pos > new_pos) {
        result.push_back(text.substr(new_pos, pos - new_pos));
      }
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

}  // namespace bbt
