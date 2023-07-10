#ifndef BBT_BASE_CONFIG_H_
#define BBT_BASE_CONFIG_H_

// Included for the __GLIBC__ macro (or similar macros on other systems).
#include <limits.h>

#ifdef __cplusplus
// Included for __GLIBCXX__, _LIBCPP_VERSION
#include <cstddef>
#endif  // __cplusplus

// BBT_INTERNAL_CPLUSPLUS_LANG
//
// MSVC does not set the value of __cplusplus correctly, but instead uses
// _MSVC_LANG as a stand-in.
// https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
//
// However, there are reports that MSVC even sets _MSVC_LANG incorrectly at
// times, for example:
// https://github.com/microsoft/vscode-cpptools/issues/1770
// https://reviews.llvm.org/D70996
//
// For this reason, this symbol is considered INTERNAL and code outside of
// Abseil must not use it.
#if defined(_MSVC_LANG)
#define BBT_INTERNAL_CPLUSPLUS_LANG _MSVC_LANG
#elif defined(__cplusplus)
#define BBT_INTERNAL_CPLUSPLUS_LANG __cplusplus
#endif

#if defined(__APPLE__)
// Included for TARGET_OS_IPHONE, __IPHONE_OS_VERSION_MIN_REQUIRED,
// __IPHONE_8_0.
#include <Availability.h>
#include <TargetConditionals.h>
#endif

// -----------------------------------------------------------------------------
// Compiler Feature Checks
// -----------------------------------------------------------------------------

// BBT_HAVE_BUILTIN()
//
// Checks whether the compiler supports a Clang Feature Checking Macro, and if
// so, checks whether it supports the provided builtin function "x" where x
// is one of the functions noted in
// https://clang.llvm.org/docs/LanguageExtensions.html
//
// Note: Use this macro to avoid an extra level of #ifdef __has_builtin check.
// http://releases.llvm.org/3.3/tools/clang/docs/LanguageExtensions.html
#ifdef __has_builtin
#define BBT_HAVE_BUILTIN(x) __has_builtin(x)
#else
#define BBT_HAVE_BUILTIN(x) 0
#endif

#ifdef __has_feature
#define BBT_HAVE_FEATURE(f) __has_feature(f)
#else
#define BBT_HAVE_FEATURE(f) 0
#endif

// Portable check for GCC minimum version:
// https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define BBT_INTERNAL_HAVE_MIN_GNUC_VERSION(x, y) \
  (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#define BBT_INTERNAL_HAVE_MIN_GNUC_VERSION(x, y) 0
#endif

#if defined(__clang__) && defined(__clang_major__) && defined(__clang_minor__)
#define BBT_INTERNAL_HAVE_MIN_CLANG_VERSION(x, y) \
  (__clang_major__ > (x) || __clang_major__ == (x) && __clang_minor__ >= (y))
#else
#define BBT_INTERNAL_HAVE_MIN_CLANG_VERSION(x, y) 0
#endif




// -----------------------------------------------------------------------------
// Platform Feature Checks
// -----------------------------------------------------------------------------

// Currently supported operating systems and associated preprocessor
// symbols:
//
//   Linux and Linux-derived           __linux__
//   Android                           __ANDROID__ (implies __linux__)
//   Linux (non-Android)               __linux__ && !__ANDROID__
//   Darwin (macOS and iOS)            __APPLE__
//   Akaros (http://akaros.org)        __ros__
//   Windows                           _WIN32
//   NaCL                              __native_client__
//   AsmJS                             __asmjs__
//   WebAssembly                       __wasm__
//   Fuchsia                           __Fuchsia__
//
// Note that since Android defines both __ANDROID__ and __linux__, one
// may probe for either Linux or Android by simply testing for __linux__.



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