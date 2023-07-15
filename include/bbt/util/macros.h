#ifndef BBT_UTIL_MACROS_H_
#define BBT_UTIL_MACROS_H_

#include <cassert>
#include <cstddef>

#include "bbt/util/attributes.h"
#include "bbt/util/config.h"
#include "bbt/util/optimization.h"

// BBT_ARRAYSIZE()
//
// Returns the number of elements in an array as a compile-time constant, which
// can be used in defining new arrays. If you use this macro on a pointer by
// mistake, you will get a compile-time error.
#define BBT_ARRAYSIZE(array) \
  (sizeof(::bbt::macros_internal::ArraySizeHelper(array)))

namespace bbt {
namespace macros_internal {
// Note: this internal template function declaration is used by BBT_ARRAYSIZE.
// The function doesn't need a definition, as we only use its type.
template <typename T, size_t N>
auto ArraySizeHelper(const T (&array)[N]) -> char (&)[N];
}  // namespace macros_internal
}  // namespace bbt

// BBT_BAD_CALL_IF()
//
// Used on a function overload to trap bad calls: any call that matches the
// overload will cause a compile-time error. This macro uses a clang-specific
// "enable_if" attribute, as described at
// https://clang.llvm.org/docs/AttributeReference.html#enable-if
//
// Overloads which use this macro should be bracketed by
// `#ifdef BBT_BAD_CALL_IF`.
//
// Example:
//
//   int isdigit(int c);
//   #ifdef BBT_BAD_CALL_IF
//   int isdigit(int c)
//     BBT_BAD_CALL_IF(c <= -1 || c > 255,
//                       "'c' must have the value of an unsigned char or EOF");
//   #endif // BBT_BAD_CALL_IF
#if BBT_HAVE_ATTRIBUTE(enable_if)
#define BBT_BAD_CALL_IF(expr, msg) \
  __attribute__((enable_if(expr, "Bad call trap"), unavailable(msg)))
#endif

// BBT_ASSERT()
//
// In C++11, `assert` can't be used portably within constexpr functions.
// BBT_ASSERT functions as a runtime assert but works in C++11 constexpr
// functions.  Example:
//
// constexpr double Divide(double a, double b) {
//   return BBT_ASSERT(b != 0), a / b;
// }
//
// This macro is inspired by
// https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/
#if defined(NDEBUG)
#define BBT_ASSERT(expr) \
  (false ? static_cast<void>(expr) : static_cast<void>(0))
#else
#define BBT_ASSERT(expr)                           \
  (BBT_PREDICT_TRUE((expr)) ? static_cast<void>(0) \
                            : [] { assert(false && #expr); }())  // NOLINT
#endif

// `BBT_INTERNAL_HARDENING_ABORT()` controls how `BBT_HARDENING_ASSERT()`
// aborts the program in release mode (when NDEBUG is defined). The
// implementation should abort the program as quickly as possible and ideally it
// should not be possible to ignore the abort request.
#if (BBT_HAVE_BUILTIN(__builtin_trap) &&         \
     BBT_HAVE_BUILTIN(__builtin_unreachable)) || \
    (defined(__GNUC__) && !defined(__clang__))
#define BBT_INTERNAL_HARDENING_ABORT() \
  do {                                 \
    __builtin_trap();                  \
    __builtin_unreachable();           \
  } while (false)
#else
#define BBT_INTERNAL_HARDENING_ABORT() abort()
#endif

// BBT_HARDENING_ASSERT()
//
// `BBT_HARDENING_ASSERT()` is like `BBT_ASSERT()`, but used to implement
// runtime assertions that should be enabled in hardened builds even when
// `NDEBUG` is defined.
//
// When `NDEBUG` is not defined, `BBT_HARDENING_ASSERT()` is identical to
// `BBT_ASSERT()`.
//
// See `BBT_OPTION_HARDENED` in `bbt/util/options.h` for more information on
// hardened mode.
#if BBT_OPTION_HARDENED == 1 && defined(NDEBUG)
#define BBT_HARDENING_ASSERT(expr)                 \
  (BBT_PREDICT_TRUE((expr)) ? static_cast<void>(0) \
                            : [] { BBT_INTERNAL_HARDENING_ABORT(); }())
#else
#define BBT_HARDENING_ASSERT(expr) BBT_ASSERT(expr)
#endif

#endif  // BBT_UTIL_MACROS_H_
