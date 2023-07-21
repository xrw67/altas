#include "bbt/net/http/response.h"

#include "gtest/gtest.h"

#include "bbt/net/buffer.h"

namespace {

const char* text =
    "HTTP/1.1 200 msg1\r\n"
    "Content-Length: 16\r\n"
    "Connection: Keep-Alive\r\n"
    "Content-Type: text/plain\r\n"
    "Host: \r\n"
    "Server: BBT\r\n"
    "\r\n"
    "I'am Body: hello";

TEST(HtttResponse, Setter) {
  bbt::HttpResponse resp(true);

  resp.set_status_code(bbt::HttpResponse::k200Ok);
  resp.set_status_message("msg1");
  resp.set_content_type("text/plain");
  resp.add_header("Server", "BBT");
  resp.add_header("Host", "");
  resp.set_body("I'am Body: hello");
  resp.set_close_connection(false);

  bbt::Buffer buffer;
  resp.AppendToBuffer(&buffer);

  ASSERT_STREQ((const char*)buffer.Peek(), text);
}

}  // namespace
