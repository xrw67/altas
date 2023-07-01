#include "bbt/base/str_util.h"

#include "gmock/gmock.h"

namespace {

using ::testing::ElementsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

TEST(StrUtil, StrSplit) {
  {
    std::vector<std::string> v = bbt::StrSplit("", ',');
    ASSERT_EQ(v.size(), 0);
  }

  {
    std::vector<std::string> v = bbt::StrSplit("a,b,c", ',');
    ASSERT_THAT(v, ElementsAre("a", "b", "c"));
  }

  {
    // Leading and trailing empty substrings.
    std::vector<std::string> v = bbt::StrSplit(",a,b,c,", ',');
    ASSERT_THAT(v, ElementsAre("", "a", "b", "c", ""));
  }

  {
    // Splits on a delimiter that is not found.
    std::vector<std::string> v = bbt::StrSplit("abc", ',');
    ASSERT_THAT(v, ElementsAre("abc"));
  }

  {
    std::string embedded_nulls("a\0b\0c", 5);
    std::vector<std::string> v = bbt::StrSplit(embedded_nulls, '\0');
    ASSERT_THAT(v, ElementsAre("a", "b", "c"));
  }
}

TEST(StrUtil, StrTrim) {
  std::string s = "  \r\n  \t \r\n Hello \r\t\n \t ";
  ASSERT_EQ(bbt::StrTrim(s, " \r\n\t"), "Hello");
}

//
// Match
//


TEST(MatchTest, StartsWith) {
  const std::string s1("123\0abc", 7);
  const bbt::string_view a("foobar");
  const bbt::string_view b(s1);
  const bbt::string_view e;
  EXPECT_TRUE(bbt::StartsWith(a, a));
  EXPECT_TRUE(bbt::StartsWith(a, "foo"));
  EXPECT_TRUE(bbt::StartsWith(a, e));
  EXPECT_TRUE(bbt::StartsWith(b, s1));
  EXPECT_TRUE(bbt::StartsWith(b, b));
  EXPECT_TRUE(bbt::StartsWith(b, e));
  EXPECT_TRUE(bbt::StartsWith(e, ""));
  EXPECT_FALSE(bbt::StartsWith(a, b));
  EXPECT_FALSE(bbt::StartsWith(b, a));
  EXPECT_FALSE(bbt::StartsWith(e, a));
}

TEST(MatchTest, EndsWith) {
  const std::string s1("123\0abc", 7);
  const bbt::string_view a("foobar");
  const bbt::string_view b(s1);
  const bbt::string_view e;
  EXPECT_TRUE(bbt::EndsWith(a, a));
  EXPECT_TRUE(bbt::EndsWith(a, "bar"));
  EXPECT_TRUE(bbt::EndsWith(a, e));
  EXPECT_TRUE(bbt::EndsWith(b, s1));
  EXPECT_TRUE(bbt::EndsWith(b, b));
  EXPECT_TRUE(bbt::EndsWith(b, e));
  EXPECT_TRUE(bbt::EndsWith(e, ""));
  EXPECT_FALSE(bbt::EndsWith(a, b));
  EXPECT_FALSE(bbt::EndsWith(b, a));
  EXPECT_FALSE(bbt::EndsWith(e, a));
}

TEST(MatchTest, Contains) {
  bbt::string_view a("abcdefg");
  bbt::string_view b("abcd");
  bbt::string_view c("efg");
  bbt::string_view d("gh");
  EXPECT_TRUE(bbt::StrContains(a, a));
  EXPECT_TRUE(bbt::StrContains(a, b));
  EXPECT_TRUE(bbt::StrContains(a, c));
  EXPECT_FALSE(bbt::StrContains(a, d));
  EXPECT_TRUE(bbt::StrContains("", ""));
  EXPECT_TRUE(bbt::StrContains("abc", ""));
  EXPECT_FALSE(bbt::StrContains("", "a"));
}

TEST(MatchTest, ContainsChar) {
  bbt::string_view a("abcdefg");
  bbt::string_view b("abcd");
  EXPECT_TRUE(bbt::StrContains(a, 'a'));
  EXPECT_TRUE(bbt::StrContains(a, 'b'));
  EXPECT_TRUE(bbt::StrContains(a, 'e'));
  EXPECT_FALSE(bbt::StrContains(a, 'h'));

  EXPECT_TRUE(bbt::StrContains(b, 'a'));
  EXPECT_TRUE(bbt::StrContains(b, 'b'));
  EXPECT_FALSE(bbt::StrContains(b, 'e'));
  EXPECT_FALSE(bbt::StrContains(b, 'h'));

  EXPECT_FALSE(bbt::StrContains("", 'a'));
  EXPECT_FALSE(bbt::StrContains("", 'a'));
}

TEST(MatchTest, ContainsNull) {
  const std::string s = "foo";
  const char* cs = "foo";
  const bbt::string_view sv("foo");
  const bbt::string_view sv2("foo\0bar", 4);
  EXPECT_EQ(s, "foo");
  EXPECT_EQ(sv, "foo");
  EXPECT_NE(sv2, "foo");
  EXPECT_TRUE(bbt::EndsWith(s, sv));
  EXPECT_TRUE(bbt::StartsWith(cs, sv));
  EXPECT_TRUE(bbt::StrContains(cs, sv));
  EXPECT_FALSE(bbt::StrContains(cs, sv2));
}

TEST(MatchTest, EqualsIgnoreCase) {
  std::string text = "the";
  bbt::string_view data(text);

  EXPECT_TRUE(bbt::EqualsIgnoreCase(data, "The"));
  EXPECT_TRUE(bbt::EqualsIgnoreCase(data, "THE"));
  EXPECT_TRUE(bbt::EqualsIgnoreCase(data, "the"));
  EXPECT_FALSE(bbt::EqualsIgnoreCase(data, "Quick"));
  EXPECT_FALSE(bbt::EqualsIgnoreCase(data, "then"));
}

TEST(MatchTest, StartsWithIgnoreCase) {
  EXPECT_TRUE(bbt::StartsWithIgnoreCase("foo", "foo"));
  EXPECT_TRUE(bbt::StartsWithIgnoreCase("foo", "Fo"));
  EXPECT_TRUE(bbt::StartsWithIgnoreCase("foo", ""));
  EXPECT_FALSE(bbt::StartsWithIgnoreCase("foo", "fooo"));
  EXPECT_FALSE(bbt::StartsWithIgnoreCase("", "fo"));
}

TEST(MatchTest, EndsWithIgnoreCase) {
  EXPECT_TRUE(bbt::EndsWithIgnoreCase("foo", "foo"));
  EXPECT_TRUE(bbt::EndsWithIgnoreCase("foo", "Oo"));
  EXPECT_TRUE(bbt::EndsWithIgnoreCase("foo", ""));
  EXPECT_FALSE(bbt::EndsWithIgnoreCase("foo", "fooo"));
  EXPECT_FALSE(bbt::EndsWithIgnoreCase("", "fo"));
}



}  // namespace
