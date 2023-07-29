#ifndef BBT_HTTP_REQUEST_H_
#define BBT_HTTP_REQUEST_H_

#include <string>
#include <vector>

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
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_H_