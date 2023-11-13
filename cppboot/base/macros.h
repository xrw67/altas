#ifndef CPPBOOT_BASE_MACROS_H_
#define CPPBOOT_BASE_MACROS_H_

#include <cassert>
#include <cstddef>

#include "cppboot/base/attributes.h"
#include "cppboot/base/config.h"
#include "cppboot/base/optimization.h"

// CPPBOOT_ARRAYSIZE()
//
// Returns the number of elements in an array as a compile-time constant, which
// can be used in defining new arrays. If you use this macro on a pointer by
// mistake, you will get a compile-time error.
#define CPPBOOT_ARRAYSIZE(array) \
  (sizeof(::cppboot::macros_internal::ArraySizeHelper(array)))

namespace cppboot {
namespace macros_internal {
// Note: this internal template function declaration is used by
// CPPBOOT_ARRAYSIZE. The function doesn't need a definition, as we only use its
// type.
template <typename T, size_t N>
auto ArraySizeHelper(const T (&array)[N]) -> char (&)[N];
}  // namespace macros_internal
}  // namespace cppboot

// CPPBOOT_BAD_CALL_IF()
//
// Used on a function overload to trap bad calls: any call that matches the
// overload will cause a compile-time error. This macro uses a clang-specific
// "enable_if" attribute, as described at
// https://clang.llvm.org/docs/AttributeReference.html#enable-if
//
// Overloads which use this macro should be bracketed by
// `#ifdef CPPBOOT_BAD_CALL_IF`.
//
// Example:
//
//   int isdigit(int c);
//   #ifdef CPPBOOT_BAD_CALL_IF
//   int isdigit(int c)
//     CPPBOOT_BAD_CALL_IF(c <= -1 || c > 255,
//                       "'c' must have the value of an unsigned char or EOF");
//   #endif // CPPBOOT_BAD_CALL_IF
#if CPPBOOT_HAVE_ATTRIBUTE(enable_if)
#define CPPBOOT_BAD_CALL_IF(expr, msg) \
  __attribute__((enable_if(expr, "Bad call trap"), unavailable(msg)))
#endif

// CPPBOOT_ASSERT()
//
// In C++11, `assert` can't be used portably within constexpr functions.
// CPPBOOT_ASSERT functions as a runtime assert but works in C++11 constexpr
// functions.  Example:
//
// constexpr double Divide(double a, double b) {
//   return CPPBOOT_ASSERT(b != 0), a / b;
// }
//
// This macro is inspired by
// https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/
#if defined(NDEBUG)
#define CPPBOOT_ASSERT(expr) \
  (false ? static_cast<void>(expr) : static_cast<void>(0))
#else
#define CPPBOOT_ASSERT(expr)                           \
  (CPPBOOT_PREDICT_TRUE((expr)) ? static_cast<void>(0) \
                                : [] { assert(false && #expr); }())  // NOLINT
#endif

// `CPPBOOT_INTERNAL_HARDENING_ABORT()` controls how
// `CPPBOOT_HARDENING_ASSERT()` aborts the program in release mode (when NDEBUG
// is defined). The implementation should abort the program as quickly as
// possible and ideally it should not be possible to ignore the abort request.
#if (CPPBOOT_HAVE_BUILTIN(__builtin_trap) &&         \
     CPPBOOT_HAVE_BUILTIN(__builtin_unreachable)) || \
    (defined(__GNUC__) && !defined(__clang__))
#define CPPBOOT_INTERNAL_HARDENING_ABORT() \
  do {                                     \
    __builtin_trap();                      \
    __builtin_unreachable();               \
  } while (false)
#else
#define CPPBOOT_INTERNAL_HARDENING_ABORT() abort()
#endif

// CPPBOOT_HARDENING_ASSERT()
//
// `CPPBOOT_HARDENING_ASSERT()` is like `CPPBOOT_ASSERT()`, but used to
// implement runtime assertions that should be enabled in hardened builds even
// when `NDEBUG` is defined.
//
// When `NDEBUG` is not defined, `CPPBOOT_HARDENING_ASSERT()` is identical to
// `CPPBOOT_ASSERT()`.
//
// See `CPPBOOT_OPTION_HARDENED` in `cppboot/base/options.h` for more
// information on hardened mode.
#if CPPBOOT_OPTION_HARDENED == 1 && defined(NDEBUG)
#define CPPBOOT_HARDENING_ASSERT(expr)                        \
  (CPPBOOT_PREDICT_TRUE((expr)) ? static_cast<void>(0) : [] { \
    CPPBOOT_INTERNAL_HARDENING_ABORT();                       \
  }())
#else
#define CPPBOOT_HARDENING_ASSERT(expr) CPPBOOT_ASSERT(expr)
#endif

#endif  // CPPBOOT_BASE_MACROS_H_
