#ifndef BBT_HTTP_REQUEST_H_
#define BBT_HTTP_REQUEST_H_

#include <string>
#include <vector>
#include <map>

#include "asio.hpp"
#include "header.h"
#include "bbt/http/url.h"

namespace bbt {
namespace http {

/// A request received from a client.

struct Request {
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<Header> headers;

  std::string content;

  std::string path;
  std::map<std::string, std::string> params;

  std::string host;
  Url url;

  Request();
  Request(const std::string& method, const std::string& raw_url);
  ~Request();

  std::string Param(const std::string& key) const noexcept {
    auto it = params.find(key);
    return (it != params.end()) ? it->second : "";
  }

  void to_buffers(asio::streambuf* buf) const noexcept;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_H_