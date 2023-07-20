#include "gtest/gtest.h"

#include "bbt/net/buffer.h"
#include "bbt/net/http/request.h"
#include "bbt/net/http/response.h"
#include "bbt/net/http/server.h"

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
    }
  });
  svr.Start();

{
  bbt::Buffer buffer;
  bbt::HttpRequest request;
  bbt::HttpResponse response(true);

  buffer.Append("GET /math/add?a=100&b=200 HTTP/1.1\r\n");
  ASSERT_TRUE(request.Parse(buffer));
  svr.Call(request, &response);
  ASSERT_EQ(response.status_code(), bbt::HttpResponse::k200Ok);
  ASSERT_EQ(response.body(), "300");
}

{
  bbt::Buffer buffer;
  bbt::HttpRequest request;
  bbt::HttpResponse response(true);

  buffer.Append("GET /math/dec?a=100&b=200 HTTP/1.1\r\n");
  ASSERT_TRUE(request.Parse(buffer));
  svr.Call(request, &response);
  ASSERT_EQ(response.status_code(), bbt::HttpResponse::k404NotFound);
}

}

}  // namespace