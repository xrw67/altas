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
  server.Handle("/hello",
                [&](const bbt::http::Request& req, bbt::http::Response* resp) {
                  // Get
                  if (req.method == "GET") {
                    auto name = req.Param("name");
                    resp->status = bbt::http::Response::ok;
                    resp->WriteText("Hello, " + name);
                  }
                });

  server.Handle("/name",
                [&](const bbt::http::Request& req, bbt::http::Response* resp) {
                  // Post
                  if (req.method == "POST") {
                    // set post value
                    auto json = bbt::json::parse(req.content);
                    this_name = json["name"];

                    resp->status = bbt::http::Response::ok;
                    resp->WriteText("Post Ok");
                  }

                  // Get & name
                  if (req.method == "GET") {
                    resp->status = bbt::http::Response::ok;

                    bbt::json root;
                    root["name"] = this_name;
                    resp->WriteJson(root);
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