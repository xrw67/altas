#ifndef BBT_HTTP_URL_H_
#define BBT_HTTP_URL_H_

#include <string>
#include "bbt/base/status.h"
#include "bbt/base/string_view.h"

namespace bbt {
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
}  // namespace bbt

#endif  // BBT_HTTP_URL_H_
