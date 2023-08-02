#ifndef BBT_HTTP_REQUEST_H_
#define BBT_HTTP_REQUEST_H_

#include <string>
#include <vector>
#include <map>

#include "asio.hpp"
#include "header.h"
#include "form_data.h"
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
  FormData params;
  std::string host;
  Url url;

  Request();
  Request(const std::string& method, const std::string& raw_url);
  ~Request();

  std::string Param(const char* key) const noexcept { return params.Get(key); }

  void to_buffers(asio::streambuf* buf) const noexcept;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_H_