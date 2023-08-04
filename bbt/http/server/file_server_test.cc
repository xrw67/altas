#include "gmock/gmock.h"

#include "bbt/http/server/file_server.h"
#include "bbt/base/fs.h"

namespace {

TEST(FileServer, Create) {
  bbt::http::FileServer fs("./root1");
  ASSERT_EQ(fs.root(), "./root1");

  fs.set_root("./root2");
  ASSERT_EQ(fs.root(), "./root2");
}

TEST(FileServer, should_serve_files) {
  auto testdata = bbt::PathJoin(bbt::Dir(__FILE__), "testdata");
}

}  // namespace