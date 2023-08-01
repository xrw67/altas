#include "gmock/gmock.h"

#include "bbt/http/url.h"

TEST(Url, Default) {
  bbt::http::Url url("");
  ASSERT_FALSE(url.IsValid());
}

TEST(Url, Copy) {
  bbt::http::Url url1("http://bing.com/search?q=dotnet");
  bbt::http::Url url2 = url1;

  ASSERT_EQ(url1.scheme, "http");
  ASSERT_EQ(url2.scheme, "http");
}

TEST(Url, ParseWithoutQuery) {
  bbt::http::Url url("http://127.0.0.1:59999/name");
  ASSERT_TRUE(url.IsValid());

  ASSERT_EQ(url.scheme, "http");
  ASSERT_EQ(url.host, "127.0.0.1:59999");
  ASSERT_EQ(url.raw_path, "/name");
  ASSERT_EQ(url.raw_query, "");
}

TEST(Url, ParseHttp) {
  bbt::http::Url url("http://bing.com/search?q=dotnet");
  ASSERT_TRUE(url.IsValid());

  ASSERT_EQ(url.scheme, "http");
  ASSERT_EQ(url.host, "bing.com");
  ASSERT_EQ(url.raw_path, "/search");
  ASSERT_EQ(url.raw_query, "q=dotnet");
}

TEST(Url, ParseHttps) {
  bbt::http::Url url("https://bing.com:9000/search?q=dotnet");
  ASSERT_TRUE(url.IsValid());

  ASSERT_EQ(url.scheme, "https");
  ASSERT_EQ(url.host, "bing.com:9000");
  ASSERT_EQ(url.raw_path, "/search");
  ASSERT_EQ(url.raw_query, "q=dotnet");
}

#if 0
TEST(Url, DISABLED_Parse) {
    bbt::http::Url url;
    
    auto st = url.Parse("http://bing.com/search?q=dotnet");
    ASSERT_TRUE(st) << st.ToString();

    ASSERT_EQ(url.schema, "http");
    ASSERT_EQ(url.host, "bing.com");
    ASSERT_EQ(url.path, "search");
    ASSERT_EQ(url.raw_query, "q=dotnet");
    auto q = url.Query();
    ASSERT_EQ(q["q"], "dotnet");
}

TEST(Url, DISABLES_ParseWithEncode) {
        bbt::http::Url url;
    
    auto st = url.Parse("https://example.com/foo\%2fbar");
    ASSERT_TRUE(st) << st.ToString();

    ASSERT_EQ(url.path, "/foo/bar");
    ASSERT_EQ(url.raw_path, "/foo\%2fbar")
    ASSERT_EQ(url.String(), "https://example.com/foo\%2fbar");
}
#endif