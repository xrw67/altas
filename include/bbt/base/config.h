#ifndef BBT_BASE_CONFIG_H_
#define BBT_BASE_CONFIG_H_

// Included for the __GLIBC__ macro (or similar macros on other systems).
#include <limits.h>

#ifdef __cplusplus
// Included for __GLIBCXX__, _LIBCPP_VERSION
#include <cstddef>
#endif  // __cplusplus

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>  // 放到最前面

// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4521
#pragma warning(disable : 4521)
// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4522
#pragma warning(disable : 4522)

#pragma warning(disable : 4819)
#pragma warning(disable : 4996)
#define getcwd _getcwd
#define snprintf _snprintf_s

#else

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#endif

#if defined(_MSC_VER) && _MSC_VER < 1700
#define PRId32 d
#define PRIu32 u
#define PRIx32 x
#define PRId64 lld
#define PRIu64 llu
#define PRIx64 llx
#else
#include <inttypes.h>
#endif


//
// BBT_EXPORT
//
#ifndef BBT_EXPORT

#if defined(_WIN32)

#if defined(BBT_COMPILE_LIBRARY)
#define BBT_EXPORT __declspec(dllexport)
#else
#define BBT_EXPORT __declspec(dllimport)
#endif  // defined(BBT_COMPILE_LIBRARY)

#else  // defined(_WIN32)
#if defined(BBT_COMPILE_LIBRARY)
#define BBT_EXPORT __attribute__((visibility("default")))
#else
#define BBT_EXPORT
#endif
#endif  // defined(_WIN32)
#endif  // BBT_EXPORT



#endif  // BBT_BASE_CONFIG_H_