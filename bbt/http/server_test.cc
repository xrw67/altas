#include "gmock/gmock.h"

#include <thread>

#include "asio.hpp"

#include "bbt/base/fs.h"
#include "bbt/base/fmt.h"
#include "bbt/base/json.h"
#include "bbt/base/str_util.h"
#include "bbt/http/client.h"
#include "bbt/http/server.h"
#include "bbt/http/request.h"
#include "bbt/http/response.h"

namespace {

TEST(Http, HttpServerAndClient) {
  std::string this_name;

  bbt::http::Server server;
  server.Handle(
      "/", [&](const bbt::http::Request& req, bbt::http::Response* resp) {
        // Get
        if (req.method == "GET" && bbt::StartsWith(req.path, "/hello")) {
          auto name = req.Param("name");
          resp->status = bbt::http::Response::ok;
          resp->content = "Hello, " + name;

          resp->headers.resize(2);
          resp->headers[0].name = "Content-Length";
          resp->headers[0].value = std::to_string(resp->content.size());
          resp->headers[1].name = "Content-Type";
          resp->headers[1].value = "text/html";
        }

        // Post
        if (req.method == "POST" && bbt::StartsWith(req.path, "/name")) {
          // set post value
          auto json = bbt::json::parse(req.content);
          this_name = json["name"];

          resp->status = bbt::http::Response::ok;
          resp->content = "Post Ok";

          resp->headers.resize(2);
          resp->headers[0].name = "Content-Length";
          resp->headers[0].value = std::to_string(resp->content.size());
          resp->headers[1].name = "Content-Type";
          resp->headers[1].value = "text/html";
        }

        // Get & name
        if (req.method == "GET" && bbt::StartsWith(req.path, "/name")) {
          resp->status = bbt::http::Response::ok;

          bbt::json content;
          content["name"] = this_name;
          resp->content = content.dump();

          resp->headers.resize(2);
          resp->headers[0].name = "Content-Length";
          resp->headers[0].value = std::to_string(resp->content.size());
          resp->headers[1].name = "Content-Type";
          resp->headers[1].value = "text/html";
        }
      });

  auto st = server.Listen("127.0.0.1", "59999");
  ASSERT_TRUE(st) << st.ToString();

  std::thread t([&]() { server.Serve(); });

  // Query
  {
    bbt::http::Response resp;
    auto st = bbt::http::Get("http://127.0.0.1:59999/hello?name=xrw", &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, bbt::http::Response::ok);
    ASSERT_EQ(resp.content, "Hello, xrw");
  }

  this_name.clear();

  // Post
  {
    bbt::http::Response resp;
    bbt::json content;
    content["name"] = "xrw";
    auto st =
        bbt::http::Post("http://127.0.0.1:59999/name", content.dump(), &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, bbt::http::Response::ok);
    ASSERT_EQ(resp.content, "Post Ok");
  }

  // Get
  {
    bbt::http::Response resp;
    st = bbt::http::Get("http://127.0.0.1:59999/name", &resp);
    bbt::json j = bbt::json::parse(resp.content);
    ASSERT_EQ(j["name"], "xrw");
  }

  server.Shutdown();
  t.join();
}

TEST(Http, HttpsServerAndClient) {}

}  // namespace