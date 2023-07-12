#include "bbt/base/fs.h"

#include <cstring>
#include <fstream>  // std::ifstream
#include <sstream>  // std::stringstream

#include "bbt/base/str_util.h"

namespace bbt {

#ifdef WIN32
static const char* kPathSeparator = "\\";
#else
static const char* kPathSeparator = "/";
#endif

std::string PathJoin(string_view a, string_view b) {
  std::string result;
  StrAppend(result, StrTrimRight(a, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(b, kPathSeparator));
  return result;
}

std::string PathJoin(string_view a, string_view b, string_view c) {
  std::string result;
  StrAppend(result, StrTrimRight(a, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(b, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(c, kPathSeparator));
  return result;
}

std::string PathJoin(string_view a, string_view b, string_view c,
                     string_view d) {
  std::string result;
  StrAppend(result, StrTrimRight(a, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(b, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(c, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(d, kPathSeparator));
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

  ofile << content;
  ofile.close();
  return OkStatus();
}

std::string ReadFile(string_view filename) {
  if (filename.empty()) return std::string();

  std::ifstream t(filename.data());
  std::stringstream buf;
  buf << t.rdbuf();
  return buf.str();
}

string_view Dir(string_view path) {
  size_t pos = path.find_last_of("/\\");
  if (pos != path.npos)
    return path.substr(0, pos);
  else
    return string_view();
}

string_view Basename(string_view path) {
  const char* slash = std::strrchr(path.data(), '/');
  if (slash) {
    return slash + 1;
  } else {
    slash = std::strrchr(path.data(), '\\');
    if (slash) return slash + 1;
  }
  return path;
}

}  // namespace bbt