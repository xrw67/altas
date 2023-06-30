#include "bbt/base/str_util.h"

#include "gmock/gmock.h"

namespace {
using ::testing::ElementsAre;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;

TEST(StrUtil, StrSplit) {
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

}  // namespace
