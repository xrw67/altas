#include "bbt/base/sys_util.h"

#ifdef WIN32
#include <direct.h>
#endif  // WIN32

#ifdef __linux__
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#endif  // __linux__

#include "bbt/base/fs.h"

namespace bbt {

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifdef WIN32

uint32_t GetTid() { return (uint32_t)::GetCurrentThreadId(); }
uint32_t GetPid() { return (uint32_t)::GetCurrentProcessId(); }

std::string GetAppPath() {
  char pathbuf[PATH_MAX] = {};
  DWORD n = ::GetModuleFileName(NULL, pathbuf, sizeof(pathbuf));
  return std::string(buffer, n);
}

#else

uint32_t GetTid() { return (uint32_t)::syscall(__NR_gettid); }
uint32_t GetPid() { return (uint32_t)::syscall(__NR_getpid); }
std::string GetAppPath() { return Readlink("/proc/self/exe"); }

#endif

std::string GetCurrentDir() {
  char pathbuf[PATH_MAX] = {0};
  getcwd(pathbuf, sizeof(pathbuf));
  return pathbuf;
}

}  // namespace bbt
