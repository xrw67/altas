#include "gmock/gmock.h"

#include "bbt/base/fs.h"

#include "bbt/http/request.h"
#include "bbt/http/response.h"
#include "bbt/http/server/file_server.h"

namespace {

TEST(FileServer, Create) {
  bbt::http::FileServer fs("./root1");
  ASSERT_EQ(fs.root(), "./root1");

  fs.set_root("./root2");
  ASSERT_EQ(fs.root(), "./root2");
}

struct FileServerText : public ::testing::Test {
  bbt::http::FileServer fs;
  bbt::http::Request req;
  bbt::http::Response resp;

  void SetUp() {
    auto root = bbt::PathJoin(bbt::Dir(__FILE__), "testdata/");
    fs.set_root(root);
  }
};

TEST_F(FileServerText, should_serve_index_html) {
  req.subpath = "/dir1/dir2/";
  fs.ServeHttp(req, &resp);
  ASSERT_EQ(resp.status, resp.ok);
  ASSERT_EQ(resp.header("Content-Type"), "text/html");
  ASSERT_GT(resp.content.length(), 0);
  ASSERT_EQ(resp.content.length(),
            bbt::FileSize(fs.root() + "/dir1/dir2/index.html"));
}

TEST_F(FileServerText, should_serve_txt) {
  req.subpath = "/dir1/dir2/1.txt";
  fs.ServeHttp(req, &resp);
  ASSERT_EQ(resp.status, resp.ok);
  ASSERT_EQ(resp.header("Content-Type"), "text/plain");
  ASSERT_EQ(resp.content, "Test Txt File");
}

TEST_F(FileServerText, shoud_server_memory_file) {
  fs.AddFile("/memory/file.txt", "I'am file");

  req.subpath = "/memory/file.txt";
  fs.ServeHttp(req, &resp);
  ASSERT_EQ(resp.status, resp.ok);
  ASSERT_EQ(resp.header("Content-Type"), "text/plain");
  ASSERT_EQ(resp.content, "I'am file");
}

}  // namespace