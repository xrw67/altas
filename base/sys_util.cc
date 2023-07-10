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

uint64_t GetTid() { return (uint64_t)::GetCurrentThreadId(); }
uint64_t GetPid() { return (uint64_t)::GetCurrentProcessId(); }

std::string GetAppPath() {
  char pathbuf[PATH_MAX] = {};
  DWORD n = ::GetModuleFileName(NULL, pathbuf, sizeof(pathbuf));
  return std::string(buffer, n);
}

#endif

#ifdef __linux__
uint64_t GetTid() { return (uint64_t)::syscall(__NR_gettid); }
uint64_t GetPid() { return (uint64_t)::syscall(__NR_getpid); }
std::string GetAppPath() { return Readlink("/proc/self/exe"); }

#endif

#ifdef __APPLE__
#include <unistd.h>
#include <pthread.h>
#include <libproc.h>

uint64_t GetTid() {
  uint64_t tid = 0;
  return (uint32_t)pthread_threadid_np(NULL, &tid);
  return tid;
}

uint64_t GetPid() { return (uint64_t)getpid(); }
std::string GetAppPath() {
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidpath(GetTid(), pathbuf, sizeof(pathbuf)) != 0) {
    return "";
  } else {
    return pathbuf;
  }
}

#endif

std::string GetCurrentDir() {
  char pathbuf[PATH_MAX] = {0};
  getcwd(pathbuf, sizeof(pathbuf));
  return pathbuf;
}

}  // namespace bbt
