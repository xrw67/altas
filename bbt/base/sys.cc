#include "bbt/base/sys.h"

#ifdef WIN32
#include <direct.h>
#else
#include <time.h>
#include <sys/types.h>
#endif

#ifdef __linux__
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
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

int64_t GetNanoClock() {
  struct timespec ts;
  if (::clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
    return ts.tv_sec * 1000000000 + ts.tv_nsec;
  }
  throw std::runtime_error("CLOCK_MONOTONIC failed");
}

}  // namespace bbt
