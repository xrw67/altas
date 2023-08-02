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

using bbt::http::Request;
using bbt::http::Response;

TEST(Http, HttpServerAndClient) {
  std::string this_name;

  bbt::http::Server server;
  server.Handle("/hello", [&](const Request& req, Response* resp) {
    // Get
    if (req.method == "GET") {
      auto name = req.Param("name");
      resp->WriteText(Response::ok, "Hello, " + name);
    }
  });

  server.Handle("/name", [&](const Request& req, Response* resp) {
    // Post
    if (req.method == "POST") {
      auto json = bbt::json::parse(req.content);
      this_name = json["name"];
      resp->WriteText(Response::ok, "Post Ok");
    }

    // Get
    if (req.method == "GET") {
      bbt::json root;
      root["name"] = this_name;
      resp->WriteJson(Response::ok, root);
    }

    if (req.method == "DELETE") {
      resp->WriteText(Response::ok, "Delete Ok");
    }
  });

  server.Handle("/name/", [&](const Request& req, Response* resp) {
    if (req.method == "DELETE") {
      resp->WriteText(Response::ok, "Delete Ok");
    }
  });

  auto st = server.Listen("127.0.0.1", "59999");
  ASSERT_TRUE(st) << st.ToString();

  std::thread t([&]() { server.Serve(); });

  // Query
  {
    Response resp;
    auto st = bbt::http::Get("http://127.0.0.1:59999/hello?name=xrw", &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, Response::ok);
    ASSERT_EQ(resp.content, "Hello, xrw");
  }

  this_name.clear();

  // Post
  {
    Response resp;
    bbt::json content;
    content["name"] = "xrw";
    auto st =
        bbt::http::Post("http://127.0.0.1:59999/name", content.dump(), &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, Response::ok);
    ASSERT_EQ(resp.content, "Post Ok");
  }

  // Get
  {
    Response resp;
    st = bbt::http::Get("http://127.0.0.1:59999/name", &resp);
    bbt::json j = bbt::json::parse(resp.content);
    ASSERT_EQ(j["name"], "xrw");
  }

  // Delete
  {
    Request req("DELETE", "http://127.0.0.1:59999/name/xrw");
    bbt::http::Client client;
    Response resp;

    st = client.Do(req, &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, Response::ok);
    ASSERT_EQ(resp.content, "Delete Ok");
  }

  // PostForm
  // {
  //   Response resp;
  //   bbt::http::FormData values;

  //   values["key1"] = "value1";
  //   values["key2"] = 1001;

  //   st = bbt::http::PostForm("http://127.0.0.1:59999/name?format=form", values,
  //                            &resp);

  //   st = client.Do(req, &resp);
  //   ASSERT_TRUE(st) << st.ToString();
  //   ASSERT_EQ(resp.status, Response::ok);

  //   ASSERT_EQ(resp.content, "PostForm Ok");
  // }

  server.Shutdown();
  t.join();
}

TEST(Http, HttpsServerAndClient) {}

}  // namespace