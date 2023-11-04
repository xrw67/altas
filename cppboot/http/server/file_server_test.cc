#include "gmock/gmock.h"

#include "cppboot/base/fs.h"

#include "cppboot/http/request.h"
#include "cppboot/http/response.h"
#include "cppboot/http/server/file_server.h"

namespace {

TEST(FileServer, Create) {
  cppboot::http::FileServer fs("./root1");
  ASSERT_EQ(fs.root(), "./root1");

  fs.set_root("./root2");
  ASSERT_EQ(fs.root(), "./root2");
}

struct FileServerText : public ::testing::Test {
  cppboot::http::FileServer fs;
  cppboot::http::Request req;
  cppboot::http::Response resp;

  void SetUp() {
    auto root = cppboot::PathJoin(cppboot::Dir(__FILE__), "testdata/");
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
            cppboot::FileSize(fs.root() + "/dir1/dir2/index.html"));
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