#include "bbt/base/fs.h"

#include "gmock/gmock.h"

namespace {

TEST(PathUtil, PathJoin) {
  ASSERT_EQ(bbt::PathJoin("/a", "b"), "/a/b");
  ASSERT_EQ(bbt::PathJoin("/a", "/b"), "/a/b");
  ASSERT_EQ(bbt::PathJoin("/a///", "///b//"), "/a/b");
  ASSERT_EQ(bbt::PathJoin("/a///", "///b/c/"), "/a/b/c");
  ASSERT_EQ(bbt::PathJoin("/a///", "///b///c/"), "/a/b///c");

  ASSERT_EQ(bbt::PathJoin("/a///", "///b//", "/c/"), "/a/b/c");

  ASSERT_EQ(bbt::PathJoin("/a///", "///b//", "///c", "//d/e//"), "/a/b/c/d/e");
}

}  // namespace