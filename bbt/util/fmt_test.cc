#include "gmock/gmock.h"

#include <vector>

#include "bbt/util/fmt.h"

namespace {

TEST(Fmt, BasicFormat) {
  std::string s = bbt::format("The answer is {}.", 42);
  ASSERT_EQ(s, "The answer is 42.");

  s = bbt::format("I'd rather be {1} than {0}.", "right", "happy");
  ASSERT_EQ(s, "I'd rather be happy than right.");

  s = bbt::format(std::locale("en_US.UTF-8"), "{:L}", 1234567890);
  ASSERT_EQ(s, "1,234,567,890");
}

TEST(Fmt, PrintRange) {
  std::vector<int> v = {1, 2, 3};
  ASSERT_EQ(bbt::format("{}", v), "[1, 2, 3]");
}

TEST(Fmt, ByPosition) {
  ASSERT_EQ(bbt::format("{0}, {1}, {2}", 'a', 'b', 'c'), "a, b, c");
  ASSERT_EQ(bbt::format("{}, {}, {}", 'a', 'b', 'c'), "a, b, c");
  ASSERT_EQ(bbt::format("{2}, {1}, {0}", 'a', 'b', 'c'), "c, b, a");
  ASSERT_EQ(bbt::format("{0}{1}{0}", "abra", "cad"), "abracadabra");
}

TEST(Fmt, SpecialWidth) {
  ASSERT_EQ(bbt::format("{:<30}", "left aligned"),
            "left aligned                  ");
  ASSERT_EQ(bbt::format("{:>30}", "right aligned"),
            "                 right aligned");
  ASSERT_EQ(bbt::format("{:^30}", "centered"),
            "           centered           ");
  ASSERT_EQ(bbt::format("{:*^30}", "centered"),
            "***********centered***********");

  ASSERT_EQ(bbt::format("{:<{}}", "left aligned", 30),
            "left aligned                  ")
      << "Dynamic width";
  ASSERT_EQ(bbt::format("{:.{}f}", 3.14, 1), "3.1") << "Dynamic precision";
}

TEST(Fmt, Hex) {
  ASSERT_EQ(bbt::format("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42),
            "int: 42;  hex: 2a;  oct: 52; bin: 101010");
  ASSERT_EQ(
      bbt::format("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42),
      "int: 42;  hex: 0x2a;  oct: 052;  bin: 0b101010");
  ASSERT_EQ(bbt::format("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42),
            "int: 42;  hex: 2a;  oct: 52; bin: 101010");
  ASSERT_EQ(
      bbt::format("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42),
      "int: 42;  hex: 0x2a;  oct: 052;  bin: 0b101010");

  ASSERT_EQ(bbt::format("{:#04x}", 0), "0x00");
}

TEST(Fmt, Chrono) {
  auto t = tm();
  t.tm_year = 2010 - 1900;
  t.tm_mon = 7;
  t.tm_mday = 4;
  t.tm_hour = 12;
  t.tm_min = 15;
  t.tm_sec = 58;
  ASSERT_EQ(bbt::format("{:%Y-%m-%d %H:%M:%S}", t), "2010-08-04 12:15:58");
}

TEST(Fmt, SaftyFormat) {
  EXPECT_THROW(
      {
        try {
          auto s = bbt::format("The answer is {:d}", "forty-two");
        } catch (const bbt::format_error& e) {
          EXPECT_STREQ("invalid format specifier", e.what());
          throw;
        }
      },
      bbt::format_error);
}

TEST(Fmt, string_view) {
  bbt::string_view sv("World");
  ASSERT_EQ(bbt::format("Hello {}", sv), "Hello World");
}
TEST(Fmt, Status) {
  auto st = bbt::NotFoundError("msg1");
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