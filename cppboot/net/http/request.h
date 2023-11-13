#ifndef CPPBOOT_NET_HTTP_REQUEST_H_
#define CPPBOOT_NET_HTTP_REQUEST_H_

#include <string>
#include <vector>
#include <map>

#include "asio.hpp"
#include "header.h"
#include "form_data.h"
#include "cppboot/net/http/url.h"

namespace cppboot {
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
  std::string subpath;

  FormData params;
  std::string host;
  Url url;

  Request();
  Request(const std::string& method, const std::string& raw_url);
  ~Request();

  void set_header(const std::string& name, const std::string& value) noexcept;

  std::string Param(const char* key) const noexcept { return params.Get(key); }

  void to_buffers(asio::streambuf* buf) const noexcept;
};

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_NET_HTTP_REQUEST_H_