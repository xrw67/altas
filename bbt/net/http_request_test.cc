#include "bbt/net/http_request.h"
#include "gtest/gtest.h"

using bbt::HttpRequest;

namespace {

TEST(HtttRequest, DefaultConstructor) {
  HttpRequest req;

  ASSERT_EQ(req.method(), HttpRequest::kInvalid);
  ASSERT_EQ(req.version(), HttpRequest::kUnknown);
  ASSERT_EQ(req.path(), "");
  ASSERT_EQ(req.query(), "");
  ASSERT_TRUE(req.headers().empty());
}

TEST(HttpRequest, SetMethod) {
  HttpRequest req;

  ASSERT_TRUE(req.set_method("GET"));
  ASSERT_EQ(req.method(), HttpRequest::kGet);
  ASSERT_TRUE(req.set_method("POST"));
  ASSERT_EQ(req.method(), HttpRequest::kPost);
  ASSERT_TRUE(req.set_method("HEAD"));
  ASSERT_EQ(req.method(), HttpRequest::kHead);
  ASSERT_TRUE(req.set_method("PUT"));
  ASSERT_EQ(req.method(), HttpRequest::kPut);
  ASSERT_TRUE(req.set_method("DELETE"));
  ASSERT_EQ(req.method(), HttpRequest::kDelete);
  ASSERT_FALSE(req.set_method("PING"));
  ASSERT_EQ(req.method(), HttpRequest::kInvalid);
}

TEST(HttpRequest, SetHeader) {
  HttpRequest req;

  req.set_header("  Content-Type  ", "   text/plain  ");
  ASSERT_EQ(req.header("Content-Type"), "text/plain");
  ASSERT_EQ(req.header("ping"), "");

}

TEST(HttpRequest, SetterOther) {
  HttpRequest req;

  req.set_version(HttpRequest::kHttp10);
  ASSERT_EQ(req.version(), HttpRequest::kHttp10);
  req.set_version(HttpRequest::kHttp11);
  ASSERT_EQ(req.version(), HttpRequest::kHttp11);


  req.set_path("/path/to/index.html");
  ASSERT_EQ(req.path(), "/path/to/index.html");

  req.set_query("id=123");
  ASSERT_EQ(req.query(), "id=123");
}

}  // namespace