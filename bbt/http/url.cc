#include "bbt/http/url.h"

#include "bbt/base/str_util.h"
#include "bbt/base/fmt.h"

namespace bbt {
namespace http {

namespace {

string_view ConsumeUtil(const char** start, char util) {
  const char* p = *start;
  while (*p != '\0' && *p != util) {
    ++p;
  }
  auto result = string_view(*start, p - *start);
  *start = p + 1;
  return result;
}

}  // namespace

Url::Url(string_view raw_url) noexcept {
  const char* p = raw_url.data();
  auto scheme_view = ConsumeUtil(&p, ':');
  if (scheme_view.empty()) {
    return;
  }

  if (p[0] != '/' || p[1] != '/' || p[2] == '/') {
    return;
  }
  p += 2;

  auto host_view = ConsumeUtil(&p, '/');
  if (host_view.empty()) {
    return;
  }

  auto path_view = ConsumeUtil(&p, '?');
  scheme = scheme_view.str();
  host = host_view.str();
  raw_path = "/" + path_view.str();
  raw_query = p;
}

}  // namespace http
}  // namespace bbt