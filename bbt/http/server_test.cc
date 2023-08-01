#include "gmock/gmock.h"

#include <thread>

#include "asio.hpp"

#include "bbt/base/fmt.h"
#include "bbt/http/client.h"
#include "bbt/http/server.h"
#include "bbt/http/request.h"
#include "bbt/http/response.h"

namespace {

TEST(Http, ServerAndClient) {
  bbt::http::Server server;
  server.Handle("/",
                [](const bbt::http::Request& req, bbt::http::Response* resp) {
                  auto name = req.Param("name");
                  resp->status = bbt::http::Response::ok;
                  resp->content = "Hello, " + name;

                  resp->headers.resize(2);
                  resp->headers[0].name = "Content-Length";
                  resp->headers[0].value = std::to_string(resp->content.size());
                  resp->headers[1].name = "Content-Type";
                  resp->headers[1].value = "text/html";
                });

  auto st = server.Listen("127.0.0.1", "59999");
  ASSERT_TRUE(st) << st.ToString();

  std::thread t([&]() { server.Serve(); });
  t.detach();
  
  {
    bbt::http::Response resp;
    auto st = bbt::http::Get("http://127.0.0.1:59999/hello?name=xrw", &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, bbt::http::Response::ok);
    ASSERT_EQ(resp.content, "Hello, xrw");
  }

  {
      // POST
  }

  {
    // DELETE
  }

  server.Shutdown();
}

}  // namespace