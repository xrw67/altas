#include "gmock/gmock.h"

#include "cppboot/http/request.h"
#include "cppboot/http/response.h"
#include "cppboot/http/server/serve_mux.h"

namespace {

using cppboot::http::Request;
using cppboot::http::Response;
using cppboot::http::ServeMux;

void func1(const Request& req, Response* resp) { resp->content = "func1"; }
void func2(const Request& req, Response* resp) { resp->content = "func2"; }
void func3(const Request& req, Response* resp) { resp->content = "func3"; }

struct ServeMuxTest : public ::testing::Test {
  ServeMux h;
  Request req;
  Response resp;

  ServeMuxTest() {}
  ~ServeMuxTest() {}

  void SetUp() {
    h.set_handler("/a/", func2);
    h.set_handler("/a/b", func3);
    h.set_handler("/a/b/", func1);
    h.set_handler("/a/b/", func2);  // invalid
  }
};

TEST_F(ServeMuxTest, shound_match_longest_path_prefix) {
  req.path = "/a/b/c";
  h.ServeHttp(req, &resp);
  ASSERT_EQ(resp.content, "func1");
}

TEST_F(ServeMuxTest, should_match_specific_path) {
  req.path = "/a/b";
  h.ServeHttp(req, &resp);
  ASSERT_EQ(resp.content, "func3");
}

TEST_F(ServeMuxTest, example3) {
  req.path = "/a/";
  h.ServeHttp(req, &resp);
  ASSERT_EQ(resp.content, "func2");
}

TEST_F(ServeMuxTest, should_match_404) {
  req.path = "/";
  h.ServeHttp(req, &resp);
  ASSERT_EQ(resp.status, Response::not_found);
}

}  // namespace