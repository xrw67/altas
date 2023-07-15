#include "bbt/util/fs.h"

#include <cstring>
#include <stdarg.h>
#include <algorithm>
#include <errno.h>
#include <fstream>  // std::ifstream
#include <set>
#include <sstream>  // std::stringstream
#include <string>

#ifdef WIN32
#include <direct.h>
#define getcwd _getcwd
#define PATH_MAX 4096
#else
#include <dirent.h>  // opendir(), readdir(), closedir()
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "bbt/util/str_util.h"

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

std::string PathJoin(string_view a, string_view b, string_view c, string_view d,
                     string_view e) {
  std::string result;
  StrAppend(result, StrTrimRight(a, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(b, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(c, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(d, kPathSeparator));
  result.append(kPathSeparator);
  StrAppend(result, StrTrim(e, kPathSeparator));
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

bool AppendFile(const std::string& filename, const std::string& data) {
  if (filename.empty() || data.empty()) return false;

#ifdef __linux__
  int n = 0;
  int fd = ::open(filename.c_str(), O_WRONLY | O_APPEND, 0);
  if (fd >= 0) {
    n = ::write(fd, data.c_str(), data.length());
    ::close(fd);
  }
  return (n > 0);
#endif
  return false;
}

std::string ReadFile(string_view filename) {
  if (!filename.empty()) {
    std::ifstream t(filename.data());
    if (t.is_open()) {
      std::stringstream buf;
      buf << t.rdbuf();
      return buf.str();
    }
  }
  return std::string();
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

bool IsFileExist(const std::string& filename) {
  return ::access(filename.c_str(), F_OK) == 0;
}

bool IsDir(const std::string& path) {
  struct stat sb = {0};
  return (::lstat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode));
}

std::vector<std::string> GetDirectoryChildren(const std::string& path) {
  std::vector<std::string> children;

#ifdef WIN32
  WIN32_FIND_DATA FileData;
  HANDLE hFind;

  hFind = FindFirstFile(path.c_str(), &FileData);
  if (hFind != INVALID_HANDLE_VALUE) {
    while (FindNextFile(hFind, &FileData)) {
      children.push_back(FileData.cFileName);
    }
    FindClose(hFind);
  }
#else

  DIR* dir;
  struct dirent* ent;

  dir = opendir(path.c_str());
  if (dir) {
    while ((ent = readdir(dir)) != NULL) {
      if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        continue;
      children.push_back(ent->d_name);
    }
    closedir(dir);
  }

#endif

  return children;
}

}  // namespace bbt