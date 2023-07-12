#include "bbt/base/fs.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "gmock/gmock.h"

namespace bbt {

TEST(Fs, PathJoin) {
  ASSERT_EQ(bbt::PathJoin("/a", "b"), "/a/b");
  ASSERT_EQ(bbt::PathJoin("/a", "/b"), "/a/b");
  ASSERT_EQ(bbt::PathJoin("/a///", "///b//"), "/a/b");
  ASSERT_EQ(bbt::PathJoin("/a///", "///b/c/"), "/a/b/c");
  ASSERT_EQ(bbt::PathJoin("/a///", "///b///c/"), "/a/b///c");

  ASSERT_EQ(bbt::PathJoin("/a///", "///b//", "/c/"), "/a/b/c");

  ASSERT_EQ(bbt::PathJoin("/a///", "///b//", "///c", "//d/e//"), "/a/b/c/d/e");
}

TEST(Fs, Readlink) {
  std::string src = GetTempPath("bbt_test_fs_readlink_src");
  std::string dst = GetTempPath("bbt_test_fs_readlink_dst");

  ::remove(dst.c_str());
  ::remove(src.c_str());

  ASSERT_TRUE(WriteFile(src, "1").ok());
  ASSERT_EQ(::symlink(src.c_str(), dst.c_str()), 0) << strerror(errno);
  ASSERT_EQ(Readlink(dst), src);
  ASSERT_EQ(Readlink(src), "");

  ::remove(dst.c_str());
  ::remove(src.c_str());
}

TEST(Fs, ReadAndWriteFile) {
  std::string path = GetTempPath("bbt_test_read_and_write_file");

  ::remove(path.c_str());

  ASSERT_TRUE(WriteFile(path, "1").ok());
  ASSERT_EQ(ReadFile(path), "1");

  ::remove(path.c_str());
}

}  // namespace bbt