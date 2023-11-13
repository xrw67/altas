#ifndef CPPBOOT_NET_HTTP_URL_H_
#define CPPBOOT_NET_HTTP_URL_H_

#include <string>
#include "cppboot/base/status.h"
#include "cppboot/base/string_view.h"

namespace cppboot {
namespace http {

// [scheme:][//[userinfo@]host][/]path[?query][#fragment]

struct Url {
  std::string scheme;
  std::string host;
  std::string raw_path;
  std::string raw_query;

  explicit Url(string_view raw_url) noexcept;

  bool IsValid() const noexcept { return !scheme.empty(); }
};

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_NET_HTTP_URL_H_
