#ifndef THEFOX_BASE_STRING_VIEW_H_
#define THEFOX_BASE_STRING_VIEW_H_

#include <cstddef>
#include <cstring>
#include <string>

namespace thefox {

class string_view {
 public:
  using value_type = char;
  using pointer = char*;
  using const_pointer = const char*;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  constexpr string_view() noexcept : ptr_(nullptr), length_(0) {}

  constexpr string_view(const char* str)
      : ptr_(str), length_(str ? std::strlen(str) : 0) {}
  string_view(const std::string& str) : string_view(str.data(), str.size()) {}

  // TODO: 健壮的代码需要检查len是否大于kMaxSize，并产生panic
  constexpr string_view(const char* data, size_type len)
      : ptr_(data), length_(len) {}

  constexpr const_pointer data() const noexcept { return ptr_; }

  // Returns the number of characters in the `string_view`.
  constexpr size_type size() const noexcept { return length_; }

  // Returns the number of characters in the `string_view`. Alias for `size()`.
  constexpr size_type length() const noexcept { return size(); }

  // Checks if the `string_view` is empty (refers to no characters).
  constexpr bool empty() const noexcept { return length_ == 0; }

  // Swaps this `string_view` with another `string_view`.
  void swap(string_view& s) noexcept {
    auto t = *this;
    *this = s;
    s = t;
  }

 private:
  const char* ptr_;
  size_t length_;
};

// This large function is defined inline so that in a fairly common case where
// one of the arguments is a literal, the compiler can elide a lot of the
// following comparisons.
constexpr bool operator==(string_view x, string_view y) noexcept {
  return x.size() == y.size() &&
         (x.empty() || std::memcmp(x.data(), y.data(), x.size()) == 0);
}

constexpr bool operator!=(string_view x, string_view y) noexcept {
  return !(x == y);
}

}  // namespace thefox

#endif  // THEFOX_BASE_STRING_VIEW_H_
