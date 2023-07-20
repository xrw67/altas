#include "bbt/net/http/response.h"

#include "gtest/gtest.h"

#include "bbt/net/buffer.h"

namespace {

TEST(HtttResponse, Setter) {
  bbt::HttpResponse resp(true);

  resp.set_status_code(bbt::HttpResponse::k200Ok);
  resp.set_status_message("msg1");
  resp.set_content_type("text/plain");
  resp.add_header("User-Agent", "");
  resp.add_header("Host", "www.hello.com");
  resp.set_body("I'am Body: hello");
  resp.set_close_connection(false);

  const char* result =
      "HTTP/1.1 200 msg1\r\n"
      "Content-Length: 16\r\n"
      "Connection: Keep-Alive\r\n"
      "Content-Type: text/plain\r\n"
      "Host: www.hello.com\r\n"
      "User-Agent: \r\n"
      "\r\n"
      "I'am Body: hello";

  bbt::Buffer buffer;
  resp.AppendToBuffer(&buffer);

  ASSERT_STREQ((const char*)buffer.Peek(), result);
}

}  // namespace
