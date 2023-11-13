#include "cppboot/base/fs.h"

#include <cstring>
#include <stdarg.h>
#include <algorithm>
#include <errno.h>
#include <fstream>  // std::ifstream
#include <set>
#include <sstream>  // std::stringstream
#include <string>

#include <stdlib.h>

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

#include "cppboot/base/fmt.h"
#include "cppboot/base/str_util.h"

namespace cppboot {

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

std::vector<std::string> PathSplit(string_view path) {
  if (path.empty()) return {};

  std::vector<std::string> result;
  if (path[0] == '/') result.push_back("/");

  auto vec = StrSplit(path, kPathSeparator[0]);
  for (auto& i : vec) {
    if (i.empty()) continue;
    result.push_back(i);
  }
  return result;
}

std::string RealPath(string_view path) {
  char buffer[PATH_MAX];
  if (::realpath(path.data(), buffer) != NULL) {
    return buffer;
  }
  return "";
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

size_t FileSize(const std::string& path) {
  struct stat st;
  return !stat(path.c_str(), &st) ? st.st_size : 0;
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

Status Remove(const std::string& path) {
  int rc = ::remove(path.c_str());
  if (rc < 0 && errno != ENOENT) {
    return ErrnoToStatus(errno, format("failed to delete: {}", path));
  }
  return OkStatus();
}

Status RemoveAll(const std::string& path) {
  auto subpaths = GetDirectoryChildren(path);
  for (auto& i : subpaths) {
    auto st = RemoveAll(PathJoin(path, i));
    if (!st.ok()) return st;
  }
  return Remove(path);
}

Status Mkdir(const std::string& path, mode_t mode) {
  int rc = ::mkdir(path.c_str(), mode);
  if (rc < 0 && errno != EEXIST) {
    return ErrnoToStatus(errno, format("failed to mkdir: {}", path));
  }
  return OkStatus();
}

Status MkdirAll(const std::string& path, mode_t mode) {
  std::string buf = path;

  for (auto i = buf.find_first_of(kPathSeparator, 1); i != std::string::npos;
       i = buf.find_first_of(kPathSeparator, i + 1)) {
    buf[i] = '\0';
    auto st = Mkdir(buf);
    buf[i] = kPathSeparator[0];
    if (!st.ok()) return st;
  }

  return Mkdir(buf);
}

Status Hardlink(const std::string& src, const std::string& dst) {
  return (link(src.c_str(), dst.c_str()) < 0)
             ? ErrnoToStatus(errno, format("failed to link {} to {}", src, dst))
             : OkStatus();
}

Status Symlink(const std::string& src, const std::string& dst) {
  return (symlink(src.c_str(), dst.c_str()) < 0)
             ? ErrnoToStatus(errno,
                             format("failed to symlink {} to ", src, dst))
             : OkStatus();
}

Status CopyFile(const std::string& src, const std::string& dst) {
  size_t nread, nwrite;
  char buffer[32 * 1024];

  FILE* fsrc = fopen(src.c_str(), "r");
  if (!fsrc) return ErrnoToStatus(errno, format("failed to open src {}", src));

  FILE* fdst = fopen(dst.c_str(), "w");
  if (!fdst) {
    fclose(fsrc);
    return ErrnoToStatus(errno, format("failed to create dst {}", src));
  }

  Status st;

  for (;;) {
    nread = fread(buffer, 1, sizeof(buffer), fsrc);
    if (nread == 0) {
      int err = ferror(fsrc);
      if (err != 0) st = ErrnoToStatus(err, format("failed to read src file"));
      break;
    }

    nwrite = fwrite(buffer, 1, nread, fdst);
    if (nwrite != nread) {
      st = ErrnoToStatus(ferror(fdst),
                         format("failed to write dst file with {}", nread));
      break;
    }
  }

  fclose(fsrc);
  fclose(fdst);
  return st;
}

Status Rename(const std::string& src, const std::string& dst) {
  return (rename(src.c_str(), dst.c_str()) < 0)
             ? ErrnoToStatus(errno,
                             format("failed to rename {} to {}", src, dst))
             : OkStatus();
}

}  // namespace cppboot
