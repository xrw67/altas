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
      auto format = req.Param("format");
      if (format == "text") {
        if (req.content == "xrw666")
          resp->WriteText(Response::ok, "PostText Ok");
        else
          resp->WriteText(Response::ok, "PostText Fail");
      } else if (format == "json") {
        auto json = bbt::json::parse(req.content);
        if (json["name"] == "xrw666")
          resp->WriteText(Response::ok, "PostJson Ok");
        else
          resp->WriteText(Response::ok, "PostJson Fail");
      } else if (format == "form") {
        bbt::http::FormData data;
        data.Parse(req.content);
        if (data.Get("key1") == "value1" && data.Get("key2") == "1001")
          resp->WriteText(Response::ok, "PostForm Ok");
        else
          resp->WriteText(Response::ok, "PostForm Fail");
      } else {
        resp->WriteText(Response::not_implemented, "invalid format");
      }
    }

    // Get
    if (req.method == "GET") {
      bbt::json root;
      root["name"] = "xrw";
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

  // Post
  {
    Response resp;
    auto st = bbt::http::Post("http://127.0.0.1:59999/name?format=text",
                              "xrw666", &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, Response::ok);
    ASSERT_EQ(resp.content, "PostText Ok");
  }

  // PostJson
  {
    Response resp;
    bbt::json data;
    data["name"] = "xrw666";
    auto st = bbt::http::PostJson("http://127.0.0.1:59999/name?format=json",
                                  data, &resp);
    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, Response::ok);
    ASSERT_EQ(resp.content, "PostJson Ok");
  }

  // PostForm
  {
    Response resp;
    bbt::http::FormData form;

    form.Set("key1", "value1");
    form.Set("key2", std::to_string(1001));

    st = bbt::http::PostForm("http://127.0.0.1:59999/name?format=form", form,
                             &resp);

    ASSERT_TRUE(st) << st.ToString();
    ASSERT_EQ(resp.status, Response::ok);
    ASSERT_EQ(resp.content, "PostForm Ok");
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

  server.Shutdown();
  t.join();
}

TEST(Http, HttpsServerAndClient) {}

}  // namespace