#include "bbt/util/str_util.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <iterator>
#include <limits>
#include <memory>

namespace bbt {

namespace {

const char* kSpaceChars = " \t\r\n";

int memcasecmp(const char* s1, const char* s2, size_t len) {
  const unsigned char* us1 = reinterpret_cast<const unsigned char*>(s1);
  const unsigned char* us2 = reinterpret_cast<const unsigned char*>(s2);

  for (size_t i = 0; i < len; i++) {
    const int diff = int{static_cast<unsigned char>(tolower(us1[i]))} -
                     int{static_cast<unsigned char>(tolower(us2[i]))};
    if (diff != 0) return diff;
  }
  return 0;
}

}  // namespace

std::string StrCat(const std::string& a, const std::string& b) {
  std::string result;
  result.append(a);
  result.append(b);
  return result;
}

std::string StrCat(const std::string& a, const std::string& b,
                   const std::string& c) {
  std::string result;
  result.append(a);
  result.append(b);
  result.append(c);
  return result;
}

std::string StrCat(const std::string& a, const std::string& b,
                   const std::string& c, const std::string& d) {
  std::string result;
  result.append(a);
  result.append(b);
  result.append(c);
  result.append(d);
  return result;
}

// TODO: 性能
std::vector<std::string> StrSplit(string_view text, char delimiter) {
  size_t pos = 0;
  size_t new_pos = 0;

  std::vector<std::string> result;

  if (text.empty()) return result;

  while (std::string::npos != pos) {
    pos = text.find_first_of(delimiter, new_pos);
    if (std::string::npos == pos) {  // 结束了
      result.push_back(to_string(text.substr(new_pos)));
      break;
    } else {
      if (pos >= new_pos) {
        result.push_back(to_string(text.substr(new_pos, pos - new_pos)));
      }
      new_pos = pos + 1;
    }
  }
  return result;
}

std::string StrPrintf(const char* fmt, ...) {
  va_list ap;
  std::vector<char> buf;
  int buf_size = 1024;  // 预留的长度
  int n = 0;

  while (1) {
    buf.resize(buf_size);
    va_start(ap, fmt);
    n = ::vsnprintf(&buf[0], buf_size, fmt, ap);
    va_end(ap);

    if (n < 0)
      return std::string();
    else if (n >= buf_size)
      buf_size = n + 1;  // for null char
    else
      break;
  }

  return std::string(&buf[0], n);
}

void StrAppend(std::string& s, string_view other) {
  s.append(other.data(), other.size());
}

std::string to_string(string_view s) { return std::string(s.data(), s.size()); }

int Stricmp(const char* s1, const char* s2) {
  int n;

  while (*s1 || *s2) {
    n = tolower(*s1++) - tolower(*s2++);
    if (n == 0) continue;
    return n;
  }
  return 0;
}

int Stricmp(const std::string& s1, const std::string& s2) {
  return Stricmp(s1.c_str(), s2.c_str());
}

string_view StrTrimLeft(string_view s, string_view cutset) {
  if (cutset.empty()) cutset = kSpaceChars;
  string_view::size_type pos = s.find_first_not_of(cutset);
  return (pos == string_view::npos) ? s : s.substr(pos);
}

string_view StrTrimRight(string_view s, string_view cutset) {
  if (cutset.empty()) cutset = kSpaceChars;
  size_t pos = s.find_last_not_of(cutset);
  return (pos == std::string::npos) ? s : s.substr(0, pos + 1);
}

string_view StrTrim(string_view s, string_view cutset) {
  return StrTrimRight(StrTrimLeft(s, cutset), cutset);
}

bool EqualsIgnoreCase(string_view piece1, string_view piece2) noexcept {
  return (piece1.size() == piece2.size() &&
          0 == memcasecmp(piece1.data(), piece2.data(), piece1.size()));
}

bool StartsWithIgnoreCase(string_view text, string_view prefix) noexcept {
  return (text.size() >= prefix.size()) &&
         EqualsIgnoreCase(text.substr(0, prefix.size()), prefix);
}

bool EndsWithIgnoreCase(string_view text, string_view suffix) noexcept {
  return (text.size() >= suffix.size()) &&
         EqualsIgnoreCase(text.substr(text.size() - suffix.size()), suffix);
}

std::vector<std::string> StrFields(const std::string& s) {
  std::vector<std::string> fields;
  size_t start, end;

  // 跳过开头的空格
  start = s.find_first_not_of(kSpaceChars);
  if (start == std::string::npos) return {};

  while ((end = s.find_first_of(kSpaceChars, start)) != std::string::npos) {
    std::string cell(s.substr(start, end - start));
    if (!cell.empty()) fields.push_back(cell);
    start = s.find_first_not_of(kSpaceChars, end);
  }
  if (start < s.length()) {
    std::string cell(StrTrimRight(s.substr(start), kSpaceChars));
    if (!cell.empty()) fields.push_back(cell);
  }
  return fields;
}

std::string StrReplace(std::string& str, const std::string& from,
                       const std::string& to) {
  std::string::size_type pos = 0;
  while ((pos = str.find(from, pos)) != std::string::npos) {
    str.replace(pos, from.length(), to);
    pos += to.length();
  }
  return str;
}
}  // namespace bbt
