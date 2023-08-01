#ifndef BBT_HTTP_REQUEST_H_
#define BBT_HTTP_REQUEST_H_

#include <string>
#include <vector>
#include <map>

#include "header.h"

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

  std::string Param(const std::string& key) const noexcept {
    auto it = params.find(key);
    return (it != params.end()) ? it->second : "";
  }
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_H_