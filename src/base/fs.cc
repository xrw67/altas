#include "bbt/base/fs.h"
#include "bbt/base/str_util.h"

namespace bbt {

#ifdef WIN32
static const char* kPathSeparator = "\\";
#else
static const char* kPathSeparator = "/";
#endif

std::string PathJoin(const std::string& a, const std::string& b) {
  std::string result;
  result.append(StrTrimRight(a, kPathSeparator));
  result.append(kPathSeparator);
  result.append(StrTrim(b, kPathSeparator));
  return result;
}

std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c) {
  std::string result;
  result.append(StrTrimRight(a, kPathSeparator));
  result.append(kPathSeparator);
  result.append(StrTrim(b, kPathSeparator));
  result.append(kPathSeparator);
  result.append(StrTrim(c, kPathSeparator));
  return result;
}

std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c, const std::string& d) {
  std::string result;
  result.append(StrTrimRight(a, kPathSeparator));
  result.append(kPathSeparator);
  result.append(StrTrim(b, kPathSeparator));
  result.append(kPathSeparator);
  result.append(StrTrim(c, kPathSeparator));
  result.append(kPathSeparator);
  result.append(StrTrim(d, kPathSeparator));
  return result;
}

}  // namespace bbt