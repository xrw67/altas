#include "gmock/gmock.h"

#include <vector>

#include "cppboot/base/fmt.h"

namespace {

TEST(Fmt, BasicFormat) {
  std::string s = cppboot::format("The answer is {}.", 42);
  ASSERT_EQ(s, "The answer is 42.");

  s = cppboot::format("I'd rather be {1} than {0}.", "right", "happy");
  ASSERT_EQ(s, "I'd rather be happy than right.");
}

TEST(Fmt, PrintRange) {
  std::vector<int> v = {1, 2, 3};
  ASSERT_EQ(cppboot::format("{}", v), "[1, 2, 3]");
}

TEST(Fmt, ByPosition) {
  ASSERT_EQ(cppboot::format("{0}, {1}, {2}", 'a', 'b', 'c'), "a, b, c");
  ASSERT_EQ(cppboot::format("{}, {}, {}", 'a', 'b', 'c'), "a, b, c");
  ASSERT_EQ(cppboot::format("{2}, {1}, {0}", 'a', 'b', 'c'), "c, b, a");
  ASSERT_EQ(cppboot::format("{0}{1}{0}", "abra", "cad"), "abracadabra");
}

TEST(Fmt, SpecialWidth) {
  ASSERT_EQ(cppboot::format("{:<30}", "left aligned"),
            "left aligned                  ");
  ASSERT_EQ(cppboot::format("{:>30}", "right aligned"),
            "                 right aligned");
  ASSERT_EQ(cppboot::format("{:^30}", "centered"),
            "           centered           ");
  ASSERT_EQ(cppboot::format("{:*^30}", "centered"),
            "***********centered***********");

  ASSERT_EQ(cppboot::format("{:<{}}", "left aligned", 30),
            "left aligned                  ")
      << "Dynamic width";
  ASSERT_EQ(cppboot::format("{:.{}f}", 3.14, 1), "3.1") << "Dynamic precision";
}

TEST(Fmt, Hex) {
  ASSERT_EQ(cppboot::format("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42),
            "int: 42;  hex: 2a;  oct: 52; bin: 101010");
  ASSERT_EQ(
      cppboot::format("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42),
      "int: 42;  hex: 0x2a;  oct: 052;  bin: 0b101010");
  ASSERT_EQ(cppboot::format("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42),
            "int: 42;  hex: 2a;  oct: 52; bin: 101010");
  ASSERT_EQ(
      cppboot::format("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42),
      "int: 42;  hex: 0x2a;  oct: 052;  bin: 0b101010");

  ASSERT_EQ(cppboot::format("{:#04x}", 0), "0x00");
}

TEST(Fmt, Chrono) {
  auto t = tm();
  t.tm_year = 2010 - 1900;
  t.tm_mon = 7;
  t.tm_mday = 4;
  t.tm_hour = 12;
  t.tm_min = 15;
  t.tm_sec = 58;
  ASSERT_EQ(cppboot::format("{:%Y-%m-%d %H:%M:%S}", t), "2010-08-04 12:15:58");
}

TEST(Fmt, SaftyFormat) {
  EXPECT_THROW(
      {
        try {
          auto s = cppboot::format("The answer is {:d}", "forty-two");
        } catch (const cppboot::format_error& e) {
          EXPECT_STREQ("invalid format specifier", e.what());
          throw;
        }
      },
      cppboot::format_error);
}

TEST(Fmt, string_view) {
  cppboot::string_view sv("World");
  ASSERT_EQ(cppboot::format("Hello {}", sv), "Hello World");
}
TEST(Fmt, Status) {
  auto st = cppboot::NotFoundError("msg1");
  ASSERT_EQ(format("{}", st), "NOT_FOUND: msg1");
}

}  // namespace

struct __test_point {
  double x, y;
};

template <>
struct fmt::formatter<__test_point> {
  auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
    return ctx.begin();
  }

  // Formats the __test_point p using the parsed format specification
  // (presentation) stored in this formatter.
  auto format(const __test_point& p, format_context& ctx) const
      -> format_context::iterator {
    // ctx.out() is an output iterator to write to.
    return fmt::format_to(ctx.out(), "({:.1f}, {:.1f})", p.x, p.y);
  }
};

namespace {
TEST(Fmt, UserDefinedThype) {
  __test_point p = {1, 2};
  std::string s = fmt::format("{}", p);
  ASSERT_EQ(s, "(1.0, 2.0)");
}

}  // namespace