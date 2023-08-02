#include "bbt/http/client.h"

#include <string>
#include <sstream>
#include <iostream>

#include "asio.hpp"
#include "bbt/base/fmt.h"
#include "bbt/base/log.h"
#include "bbt/http/response.h"
#include "bbt/http/url.h"

namespace bbt {
namespace http {

using asio::ip::tcp;

Status Get(const std::string& url, Response* resp) {
  Request req("GET", url);
  Client client;
  return client.Do(req, resp);
}

Status Post(const std::string& url, const std::string& content,
            Response* resp) {
  Request req("POST", url);
  req.content = content;
  req.set_header("Content-Length", std::to_string(req.content.size()));
  req.set_header("Content-Type", "text/plain");

  Client client;
  return client.Do(req, resp);
}

Status PostJson(const std::string& url, const json& data, Response* resp) {
  Request req("POST", url);
  req.content = data.dump();
  req.set_header("Content-Length", std::to_string(req.content.size()));
  req.set_header("Content-Type", "application/json");

  Client client;
  return client.Do(req, resp);
}

Status PostForm(const std::string& url, const FormData& data, Response* resp) {
  Request req("POST", url);
  req.content = data.Encode();
  req.set_header("Content-Length", std::to_string(req.content.size()));
  req.set_header("Content-Type", "text/html");

  Client client;
  return client.Do(req, resp);
}

Client::Client() : socket_(io_context_) {}

Client::~Client() {}

Status Client::Do(const Request& req, Response* resp) {
  auto endpoints = GetEndpointFromUrl(req.url);

  // Try each endpoint until we successfully establish a connection.
  asio::connect(socket_, endpoints);
  asio::streambuf request;
  req.to_buffers(&request);
  asio::write(socket_, request);

  return ReadResponse(resp);
}

tcp::resolver::results_type Client::GetEndpointFromUrl(
    const bbt::http::Url& url) {
  // Get a list of endpoints corresponding to the server name.
  tcp::resolver resolver(io_context_);
  tcp::resolver::results_type endpoints;

  auto colon = url.host.find_last_of(":");
  if (colon > 0) {
    auto host = url.host.substr(0, colon);
    if (host.empty()) host = "0.0.0.0";
    return resolver.resolve(host, url.host.substr(colon + 1));
  }
  return resolver.resolve(url.host, url.scheme);
}

Status Client::ReadResponse(Response* resp) {
  try {
    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    asio::streambuf response;
    asio::read_until(socket_, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
      return bbt::InvalidArgumentError("Invalid response");
    }

    if (status_code != 200) {
      return bbt::InvalidArgumentError(
          bbt::format("Response returned with status code {}", status_code));
    }

    Response reply;
    reply.status = Response::ok;
    // Read the response headers, which are terminated by a blank line.
    asio::read_until(socket_, response, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r") {
      BBT_LOG(DEBUG, "{}", header);
    }

    std::stringstream content;

    // Write whatever content we already have to output.
    if (response.size() > 0) {
      content << &response;
    }

    // Read until EOF, writing data to output as we go.
    asio::error_code error;
    while (asio::read(socket_, response, asio::transfer_at_least(1), error)) {
      content << &response;
    }
    if (error != asio::error::eof) {
      throw asio::system_error(error);
    }

    resp->status = bbt::http::Response::ok;
    resp->content = content.str();
  } catch (std::exception& e) {
    return bbt::CancelledError(e.what());
  }

  return bbt::OkStatus();
}

}  // namespace http
}  // namespace bbt