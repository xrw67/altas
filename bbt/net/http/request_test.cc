#include "bbt/net/http/request.h"

#include "gtest/gtest.h"

#include "bbt/net/buffer.h"

namespace {

TEST(HtttRequest, DefaultConstructor) {
  bbt::HttpRequest req;

  ASSERT_EQ(req.method(), bbt::HttpRequest::kInvalid);
  ASSERT_EQ(req.version(), bbt::HttpRequest::kUnknown);
  ASSERT_EQ(req.path(), "");
  ASSERT_EQ(req.query(), "");
  ASSERT_TRUE(req.headers().empty());
}

TEST(HttpRequest, SetMethod) {
  bbt::HttpRequest req;

  ASSERT_TRUE(req.set_method("GET"));
  ASSERT_EQ(req.method(), bbt::HttpRequest::kGet);
  ASSERT_TRUE(req.set_method("POST"));
  ASSERT_EQ(req.method(), bbt::HttpRequest::kPost);
  ASSERT_TRUE(req.set_method("HEAD"));
  ASSERT_EQ(req.method(), bbt::HttpRequest::kHead);
  ASSERT_TRUE(req.set_method("PUT"));
  ASSERT_EQ(req.method(), bbt::HttpRequest::kPut);
  ASSERT_TRUE(req.set_method("DELETE"));
  ASSERT_EQ(req.method(), bbt::HttpRequest::kDelete);
  ASSERT_FALSE(req.set_method("PING"));
  ASSERT_EQ(req.method(), bbt::HttpRequest::kInvalid);
}

TEST(HttpRequest, SetHeader) {
  bbt::HttpRequest req;

  req.set_header("Content-Type", "text/plain");
  ASSERT_EQ(req.header("Content-Type"), "text/plain");
  ASSERT_EQ(req.header("ping"), "");
}

TEST(HttpRequest, SetterOther) {
  bbt::HttpRequest req;

  req.set_version(bbt::HttpRequest::kHttp10);
  ASSERT_EQ(req.version(), bbt::HttpRequest::kHttp10);
  req.set_version(bbt::HttpRequest::kHttp11);
  ASSERT_EQ(req.version(), bbt::HttpRequest::kHttp11);

  req.set_path("/path/to/index.html");
  ASSERT_EQ(req.path(), "/path/to/index.html");

  req.set_query("id=123");
  ASSERT_EQ(req.query(), "id=123");
}

TEST(HttpRequest, Parse) {
  const char* text =
      "GET /index.html?key1=value1&key2 HTTP/1.1\r\n"
      "Host: www.hello.com\r\n"
      "User-Agent:\r\n"
      "Accept-Encoding: \r\n"
      "\r\n"
      "I'am Body: hello";

  bbt::Buffer buffer;
  buffer.Append(text);

  bbt::HttpRequest request;
  auto st = request.Parse(buffer);
  ASSERT_TRUE(st) << st.ToString();
  ASSERT_EQ(request.method(), bbt::HttpRequest::kGet);
  ASSERT_EQ(request.path(), "/index.html");
  ASSERT_EQ(request.query(), "key1=value1&key2");
  ASSERT_EQ(request.version(), bbt::HttpRequest::kHttp11);
  ASSERT_EQ(request.header("Host"), "www.hello.com");
  ASSERT_EQ(request.header("User-Agent"), "");
  ASSERT_EQ(request.header("Accept-Encoding"), "");
  ASSERT_EQ(request.header("Accept-Encoding"), "");

  request.ParseForm();
  ASSERT_EQ(request.form("key1"), "value1");
  ASSERT_EQ(request.form("key2"), "");

  ASSERT_EQ(request.body(), "I'am Body: hello");
}

}  // namespace