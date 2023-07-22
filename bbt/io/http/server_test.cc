#include "gtest/gtest.h"

#include "bbt/io/buffer.h"
#include "bbt/io/http/request.h"
#include "bbt/io/http/response.h"
#include "bbt/io/http/server.h"

namespace {

struct MockHttpServer : public bbt::HttpServer {
  void Start() noexcept {}

  void Call(bbt::HttpRequest& req, bbt::HttpResponse* resp) {
    http_callback_(req, resp);
  }
};

TEST(HttpServer, nothing) {
  MockHttpServer svr;
  svr.set_http_callback([](bbt::HttpRequest& req, bbt::HttpResponse* resp) {
    if (req.path() == "/math/add") {
      req.ParseForm();

      int a = std::stoi(req.form("a"));
      int b = std::stoi(req.form("b"));
      int sum = a + b;

      resp->set_status_code(bbt::HttpResponse::k200Ok);
      resp->set_body(std::to_string(sum));
    } else {
      resp->set_status_code(bbt::HttpResponse::k404NotFound);
      resp->set_status_message("Not Found");
      resp->set_close_connection(true);
    }
  });
  svr.Start();

#define CHECK(req_text, resp_code, resp_body)     \
  do {                                            \
    bbt::Buffer buffer;                           \
    bbt::HttpRequest request;                     \
    bbt::HttpResponse response(true);             \
    buffer.Append(req_text);                      \
    ASSERT_TRUE(request.Parse(buffer));           \
    svr.Call(request, &response);                 \
    ASSERT_EQ(response.status_code(), resp_code); \
    ASSERT_EQ(response.body(), resp_body);        \
  } while (0)

  CHECK("GET /math/add?a=100&b=200 HTTP/1.1\r\n", bbt::HttpResponse::k200Ok,
        "300");

  CHECK("GET /math/dec?a=100&b=200 HTTP/1.1\r\n",
        bbt::HttpResponse::k404NotFound, "");

}  // namespace

}  // namespace