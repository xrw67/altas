#include "cppboot/base/str_util.h"

#include "gmock/gmock.h"

namespace {

using ::testing::ElementsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

TEST(StrUtil, StrCat) {
  std::string result;

  result = cppboot::StrCat("", "");
  EXPECT_EQ(result, "");

  result = cppboot::StrCat("", "Hello");
  EXPECT_EQ(result, "Hello");

  result = cppboot::StrCat("Hello", "World");
  EXPECT_EQ(result, "HelloWorld");

  result = cppboot::StrCat("", "", "");
  EXPECT_EQ(result, "");

  result = cppboot::StrCat("Hello", "My", "World");
  EXPECT_EQ(result, "HelloMyWorld");

  result = cppboot::StrCat("Hello", "My", "Funny", "World");
  EXPECT_EQ(result, "HelloMyFunnyWorld");
}

TEST(StrUtil, Stricmp) {
  ASSERT_EQ(cppboot::Stricmp("Hello", "HELLO"), 0);
  ASSERT_GT(cppboot::Stricmp("Hello1", "HELLO"), 0);
  ASSERT_LT(cppboot::Stricmp("Hello", "HELLO1"), 0);

  ASSERT_EQ(cppboot::Stricmp(std::string("Hello"), std::string("HELLO")), 0);
  ASSERT_GT(cppboot::Stricmp(std::string("Hello1"), std::string("HELLO")), 0);
  ASSERT_LT(cppboot::Stricmp(std::string("Hello"), std::string("HELLO1")), 0);
}

TEST(StrUtil, StrSplit) {
  {
    std::vector<std::string> v = cppboot::StrSplit("", ',');
    ASSERT_EQ(v.size(), 0);
  }

  {
    std::vector<std::string> v = cppboot::StrSplit("a,b,c", ',');
    ASSERT_THAT(v, ElementsAre("a", "b", "c"));
  }

  {
    // Leading and trailing empty substrings.
    std::vector<std::string> v = cppboot::StrSplit(",a,b,c,", ',');
    ASSERT_THAT(v, ElementsAre("", "a", "b", "c", ""));
  }

  {
    // Splits on a delimiter that is not found.
    std::vector<std::string> v = cppboot::StrSplit("abc", ',');
    ASSERT_THAT(v, ElementsAre("abc"));
  }

  {
    std::string embedded_nulls("a\0b\0c", 5);
    std::vector<std::string> v = cppboot::StrSplit(embedded_nulls, '\0');
    ASSERT_THAT(v, ElementsAre("a", "b", "c"));
  }
}

TEST(StrUtil, StrTrim) {
  std::string s = "  \r\n  \t \r\n Hello \r\t\n \t ";
  ASSERT_EQ(cppboot::StrTrim(s, " \r\n\t"), "Hello");
  ASSERT_EQ(cppboot::StrTrim(s), "Hello");
  ASSERT_EQ(cppboot::StrTrim(s, " "), "\r\n  \t \r\n Hello \r\t\n \t");
}

TEST(StrUtil, CanAppendStringView) {
  std::string hello("Hello");
  cppboot::string_view world(" World");

  StrAppend(hello, world);
  ASSERT_EQ(hello, "Hello World");
}

//
// Match
//

TEST(MatchTest, StartsWith) {
  const std::string s1("123\0abc", 7);
  const cppboot::string_view a("foobar");
  const cppboot::string_view b(s1);
  const cppboot::string_view e;
  EXPECT_TRUE(cppboot::StartsWith(a, a));
  EXPECT_TRUE(cppboot::StartsWith(a, "foo"));
  EXPECT_TRUE(cppboot::StartsWith(a, e));
  EXPECT_TRUE(cppboot::StartsWith(b, s1));
  EXPECT_TRUE(cppboot::StartsWith(b, b));
  EXPECT_TRUE(cppboot::StartsWith(b, e));
  EXPECT_TRUE(cppboot::StartsWith(e, ""));
  EXPECT_FALSE(cppboot::StartsWith(a, b));
  EXPECT_FALSE(cppboot::StartsWith(b, a));
  EXPECT_FALSE(cppboot::StartsWith(e, a));
}

TEST(MatchTest, EndsWith) {
  const std::string s1("123\0abc", 7);
  const cppboot::string_view a("foobar");
  const cppboot::string_view b(s1);
  const cppboot::string_view e;
  EXPECT_TRUE(cppboot::EndsWith(a, a));
  EXPECT_TRUE(cppboot::EndsWith(a, "bar"));
  EXPECT_TRUE(cppboot::EndsWith(a, e));
  EXPECT_TRUE(cppboot::EndsWith(b, s1));
  EXPECT_TRUE(cppboot::EndsWith(b, b));
  EXPECT_TRUE(cppboot::EndsWith(b, e));
  EXPECT_TRUE(cppboot::EndsWith(e, ""));
  EXPECT_FALSE(cppboot::EndsWith(a, b));
  EXPECT_FALSE(cppboot::EndsWith(b, a));
  EXPECT_FALSE(cppboot::EndsWith(e, a));
}

TEST(MatchTest, Contains) {
  cppboot::string_view a("abcdefg");
  cppboot::string_view b("abcd");
  cppboot::string_view c("efg");
  cppboot::string_view d("gh");
  EXPECT_TRUE(cppboot::StrContains(a, a));
  EXPECT_TRUE(cppboot::StrContains(a, b));
  EXPECT_TRUE(cppboot::StrContains(a, c));
  EXPECT_FALSE(cppboot::StrContains(a, d));
  EXPECT_TRUE(cppboot::StrContains("", ""));
  EXPECT_TRUE(cppboot::StrContains("abc", ""));
  EXPECT_FALSE(cppboot::StrContains("", "a"));
}

TEST(MatchTest, ContainsChar) {
  cppboot::string_view a("abcdefg");
  cppboot::string_view b("abcd");
  EXPECT_TRUE(cppboot::StrContains(a, 'a'));
  EXPECT_TRUE(cppboot::StrContains(a, 'b'));
  EXPECT_TRUE(cppboot::StrContains(a, 'e'));
  EXPECT_FALSE(cppboot::StrContains(a, 'h'));

  EXPECT_TRUE(cppboot::StrContains(b, 'a'));
  EXPECT_TRUE(cppboot::StrContains(b, 'b'));
  EXPECT_FALSE(cppboot::StrContains(b, 'e'));
  EXPECT_FALSE(cppboot::StrContains(b, 'h'));

  EXPECT_FALSE(cppboot::StrContains("", 'a'));
  EXPECT_FALSE(cppboot::StrContains("", 'a'));
}

TEST(MatchTest, ContainsNull) {
  const std::string s = "foo";
  const char* cs = "foo";
  const cppboot::string_view sv("foo");
  const cppboot::string_view sv2("foo\0bar", 4);
  EXPECT_EQ(s, "foo");
  EXPECT_EQ(sv, "foo");
  EXPECT_NE(sv2, "foo");
  EXPECT_TRUE(cppboot::EndsWith(s, sv));
  EXPECT_TRUE(cppboot::StartsWith(cs, sv));
  EXPECT_TRUE(cppboot::StrContains(cs, sv));
  EXPECT_FALSE(cppboot::StrContains(cs, sv2));
}

TEST(MatchTest, EqualsIgnoreCase) {
  std::string text = "the";
  cppboot::string_view data(text);

  EXPECT_TRUE(cppboot::EqualsIgnoreCase(data, "The"));
  EXPECT_TRUE(cppboot::EqualsIgnoreCase(data, "THE"));
  EXPECT_TRUE(cppboot::EqualsIgnoreCase(data, "the"));
  EXPECT_FALSE(cppboot::EqualsIgnoreCase(data, "Quick"));
  EXPECT_FALSE(cppboot::EqualsIgnoreCase(data, "then"));
}

TEST(MatchTest, StartsWithIgnoreCase) {
  EXPECT_TRUE(cppboot::StartsWithIgnoreCase("foo", "foo"));
  EXPECT_TRUE(cppboot::StartsWithIgnoreCase("foo", "Fo"));
  EXPECT_TRUE(cppboot::StartsWithIgnoreCase("foo", ""));
  EXPECT_FALSE(cppboot::StartsWithIgnoreCase("foo", "fooo"));
  EXPECT_FALSE(cppboot::StartsWithIgnoreCase("", "fo"));
}

TEST(MatchTest, EndsWithIgnoreCase) {
  EXPECT_TRUE(cppboot::EndsWithIgnoreCase("foo", "foo"));
  EXPECT_TRUE(cppboot::EndsWithIgnoreCase("foo", "Oo"));
  EXPECT_TRUE(cppboot::EndsWithIgnoreCase("foo", ""));
  EXPECT_FALSE(cppboot::EndsWithIgnoreCase("foo", "fooo"));
  EXPECT_FALSE(cppboot::EndsWithIgnoreCase("", "fo"));
}

}  // namespace
