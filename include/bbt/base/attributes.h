#ifndef BBT_BASE_ATTRIBUTES_H_
#define BBT_BASE_ATTRIBUTES_H_

#include "bbt/base/config.h"

// BBT_HAVE_ATTRIBUTE
//
// A function-like feature checking macro that is a wrapper around
// `__has_attribute`, which is defined by GCC 5+ and Clang and evaluates to a
// nonzero constant integer if the attribute is supported or 0 if not.
//
// It evaluates to zero if `__has_attribute` is not defined by the compiler.
//
// GCC: https://gcc.gnu.org/gcc-5/changes.html
// Clang: https://clang.llvm.org/docs/LanguageExtensions.html
#ifdef __has_attribute
#define BBT_HAVE_ATTRIBUTE(x) __has_attribute(x)
#else
#define BBT_HAVE_ATTRIBUTE(x) 0
#endif

// BBT_HAVE_CPP_ATTRIBUTE
//
// A function-like feature checking macro that accepts C++11 style attributes.
// It's a wrapper around `__has_cpp_attribute`, defined by ISO C++ SD-6
// (https://en.cppreference.com/w/cpp/experimental/feature_test). If we don't
// find `__has_cpp_attribute`, will evaluate to 0.
#if defined(__cplusplus) && defined(__has_cpp_attribute)
// NOTE: requiring __cplusplus above should not be necessary, but
// works around https://bugs.llvm.org/show_bug.cgi?id=23435.
#define BBT_HAVE_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define BBT_HAVE_CPP_ATTRIBUTE(x) 0
#endif

// BBT_MUST_USE_RESULT
//
// Tells the compiler to warn about unused results.
//
// For code or headers that are assured to only build with C++17 and up, prefer
// just using the standard `[[nodiscard]]` directly over this macro.
//
// When annotating a function, it must appear as the first part of the
// declaration or definition. The compiler will warn if the return value from
// such a function is unused:
//
//   BBT_MUST_USE_RESULT Sprocket* AllocateSprocket();
//   AllocateSprocket();  // Triggers a warning.
//
// When annotating a class, it is equivalent to annotating every function which
// returns an instance.
//
//   class BBT_MUST_USE_RESULT Sprocket {};
//   Sprocket();  // Triggers a warning.
//
//   Sprocket MakeSprocket();
//   MakeSprocket();  // Triggers a warning.
//
// Note that references and pointers are not instances:
//
//   Sprocket* SprocketPointer();
//   SprocketPointer();  // Does *not* trigger a warning.
//
// BBT_MUST_USE_RESULT allows using cast-to-void to suppress the unused result
// warning. For that, warn_unused_result is used only for clang but not for gcc.
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66425
//
// Note: past advice was to place the macro after the argument list.
//
// TODO(b/176172494): Use BBT_HAVE_CPP_ATTRIBUTE(nodiscard) when all code is
// compliant with the stricter [[nodiscard]].
#if defined(__clang__) && BBT_HAVE_ATTRIBUTE(warn_unused_result)
#define BBT_MUST_USE_RESULT __attribute__((warn_unused_result))
#else
#define BBT_MUST_USE_RESULT
#endif

// BBT_ATTRIBUTE_LIFETIME_BOUND indicates that a resource owned by a function
// parameter or implicit object parameter is retained by the return value of the
// annotated function (or, for a parameter of a constructor, in the value of the
// constructed object). This attribute causes warnings to be produced if a
// temporary object does not live long enough.
//
// When applied to a reference parameter, the referenced object is assumed to be
// retained by the return value of the function. When applied to a non-reference
// parameter (for example, a pointer or a class type), all temporaries
// referenced by the parameter are assumed to be retained by the return value of
// the function.
//
// See also the upstream documentation:
// https://clang.llvm.org/docs/AttributeReference.html#lifetimebound
#if BBT_HAVE_CPP_ATTRIBUTE(clang::lifetimebound)
#define BBT_ATTRIBUTE_LIFETIME_BOUND [[clang::lifetimebound]]
#elif BBT_HAVE_ATTRIBUTE(lifetimebound)
#define BBT_ATTRIBUTE_LIFETIME_BOUND __attribute__((lifetimebound))
#else
#define BBT_ATTRIBUTE_LIFETIME_BOUND
#endif

#endif  // BBT_BASE_ATTRIBUTES_H_