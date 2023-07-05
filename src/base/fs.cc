#include "bbt/base/fs.h"

#include <fstream>

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

std::string GetTempPath(const std::string& name) {
#ifdef WIN32
  char buffer[PATH_MAX] = {};
  ::GetTempPath(sizeof(buffer), buffer);
  return PathJoin(buffer, name);
#else
  return "/tmp/" + name;
#endif
}

std::string Readlink(const std::string& linkpath) {
  int n;
  char buffer[PATH_MAX];

  n = ::readlink(linkpath.c_str(), buffer, sizeof(buffer) - 1);
  if (n > 0) {
    buffer[n] = 0;
    return buffer;
  }
  return "";
}

Status WriteFile(const std::string& filename, const std::string& content) {
  if (filename.empty()) return InvalidArgumentError("no filename");

  std::ofstream ofile(filename);
  if (!ofile.is_open()) return CancelledError("create file failed");

  ofile << content << std::endl;
  ofile.close();
  return OkStatus();
}

}  // namespace bbt