#include "cppboot/base/fs.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "gmock/gmock.h"
#include "cppboot/base/str_util.h"
#include "cppboot/base/sys.h"

namespace cppboot {

using ::testing::ElementsAre;

TEST(Fs, PathJoin) {
  ASSERT_EQ(cppboot::PathJoin("/a", "b"), "/a/b");
  ASSERT_EQ(cppboot::PathJoin("/a", "/b"), "/a/b");
  ASSERT_EQ(cppboot::PathJoin("/a///", "///b//"), "/a/b");
  ASSERT_EQ(cppboot::PathJoin("/a///", "///b/c/"), "/a/b/c");
  ASSERT_EQ(cppboot::PathJoin("/a///", "///b///c/"), "/a/b///c");

  ASSERT_EQ(cppboot::PathJoin("/a///", "///b//", "/c/"), "/a/b/c");

  ASSERT_EQ(cppboot::PathJoin("/a///", "///b//", "///c", "//d/e//"), "/a/b/c/d/e");
  ASSERT_EQ(cppboot::PathJoin("/a///", "///b//", "///c", "//d/e//", "fff"),
            "/a/b/c/d/e/fff");
}

TEST(Fs, PathSplit) {
  ASSERT_TRUE(PathSplit("").empty());

  ASSERT_THAT(PathSplit("/"), ElementsAre("/"));
  ASSERT_THAT(PathSplit("/path"), ElementsAre("/", "path"));
  ASSERT_THAT(PathSplit("/path/"), ElementsAre("/", "path"));
  ASSERT_THAT(PathSplit("/path/to"), ElementsAre("/", "path", "to"));
  ASSERT_THAT(PathSplit("///path///to////"), ElementsAre("/", "path", "to"));

  ASSERT_THAT(PathSplit("path"), ElementsAre("path"));
  ASSERT_THAT(PathSplit("path/to"), ElementsAre("path", "to"));
}

TEST(Fs, RealPath) {
  ASSERT_EQ(GetCurrentDir(), RealPath("."));
  ASSERT_EQ("", RealPath("/path/to/file"));
}

TEST(Fs, Readlink) {
  std::string src = GetTempPath("cppboot_test_fs_readlink_src");
  std::string dst = GetTempPath("cppboot_test_fs_readlink_dst");

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
  std::string path = GetTempPath("cppboot_test_read_and_write_file");

  ::remove(path.c_str());

  ASSERT_TRUE(WriteFile(path, "1").ok());
  ASSERT_EQ(ReadFile(path), "1");

  ::remove(path.c_str());
}

TEST(Fs, DirAndBasename) {
  {
    std::string path = "/path/to/file";
    ASSERT_EQ(cppboot::to_string(cppboot::Dir(path)), "/path/to");
    ASSERT_EQ(cppboot::to_string(cppboot::Basename(path)), "file");
  }

  {
    std::string path = "/path/to/";
    ASSERT_EQ(cppboot::to_string(cppboot::Dir(path)), "/path/to");
    ASSERT_EQ(cppboot::to_string(cppboot::Basename(path)), "");
  }

  {
    std::string path = "file";
    ASSERT_EQ(cppboot::to_string(cppboot::Dir(path)), "");
    ASSERT_EQ(cppboot::to_string(cppboot::Basename(path)), "file");
  }
}

TEST(Fs, RemoveBadPath) {
  ASSERT_EQ("OK", Remove("").ToString());
  ASSERT_EQ("OK", RemoveAll("").ToString());

  ASSERT_EQ("OK", Remove("/path/to/noexist").ToString());
  ASSERT_EQ("OK", RemoveAll("/path/to/noexist").ToString());
}

TEST(Fs, MkdirAndRemove) {
  std::string dir = GetTempPath("cppboot_fs_test_Px3X");
  std::string subdir = PathJoin(dir, "1", "2", "3", "4");

  ASSERT_EQ("OK", RemoveAll(dir).ToString());
  ASSERT_EQ("OK", MkdirAll(subdir.c_str()).ToString());
  ASSERT_EQ("OK", MkdirAll(subdir.c_str()).ToString()) << "mkdir again";
  ASSERT_TRUE(IsFileExist(subdir));
  ASSERT_EQ("OK", RemoveAll(dir).ToString());
  ASSERT_FALSE(IsFileExist(dir));
}

TEST(Fs, Link) {
  std::string dir = GetTempPath("cppboot_fs_test_Pa3X");
  std::string src = PathJoin(dir, "1");
  std::string dst1 = PathJoin(dir, "2");
  std::string dst2 = PathJoin(dir, "3");

  ASSERT_EQ("OK", RemoveAll(dir).ToString());
  ASSERT_EQ("OK", MkdirAll(dir).ToString());

  ASSERT_EQ("OK", WriteFile(src, "1").ToString());
  ASSERT_EQ("OK", Hardlink(src, dst1).ToString());
  ASSERT_EQ("OK", Symlink(src, dst2).ToString());
  ASSERT_EQ("1", ReadFile(dst1));
  ASSERT_EQ("1", ReadFile(dst2));

  ASSERT_EQ("OK", RemoveAll(dir).ToString());
}

}  // namespace cppboot
