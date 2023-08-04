#include "gmock/gmock.h"

#include "bbt/http/request.h"
#include "bbt/http/response.h"
#include "bbt/http/server/serve_mux.h"

namespace {

using bbt::http::Request;
using bbt::http::ServeMux;
using bbt::http::Response;

void func1(const Request& req, Response* resp) { resp->content = "func1"; }
void func2(const Request& req, Response* resp) { resp->content = "func2"; }
void func3(const Request& req, Response* resp) { resp->content = "func3"; }

struct RequestHandlerTest : public ::testing::Test {
  ServeMux h;
  Request req;
  Response resp;

  RequestHandlerTest() {}
  ~RequestHandlerTest() {}

  void SetUp() {
    h.set_handler("/a/", func2);
    h.set_handler("/a/b", func3);
    h.set_handler("/a/b/", func1);
    h.set_handler("/a/b/", func2);  // invalid
  }
};

TEST_F(RequestHandlerTest, shound_match_longest_path_prefix) {
  req.path = "/a/b/c";
  h.HandleRequest(req, &resp);
  ASSERT_EQ(resp.content, "func1");
}

TEST_F(RequestHandlerTest, should_match_specific_path) {
  req.path = "/a/b";
  h.HandleRequest(req, &resp);
  ASSERT_EQ(resp.content, "func3");
}

TEST_F(RequestHandlerTest, example3) {
  req.path = "/a/";
  h.HandleRequest(req, &resp);
  ASSERT_EQ(resp.content, "func2");
}

TEST_F(RequestHandlerTest, should_match_404) {
  req.path = "/";
  h.HandleRequest(req, &resp);
  ASSERT_EQ(resp.status, Response::not_found);
}

}  // namespace