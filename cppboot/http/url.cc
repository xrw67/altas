#include "cppboot/http/url.h"

#include "cppboot/base/str_util.h"
#include "cppboot/base/fmt.h"

namespace cppboot {
namespace http {

namespace {

string_view ConsumeUtil(const char** start, const char* end, char util) {
  const char* p = *start;
  while (p != end && *p != util) {
    ++p;
  }
  auto result = string_view(*start, p - *start);
  *start = p;
  return result;
}

}  // namespace

Url::Url(string_view raw_url) noexcept {
  const char* p = raw_url.data();
  const char* end = raw_url.data() + raw_url.length();

  auto scheme_view = ConsumeUtil(&p, end, ':');
  if (scheme_view.empty()) {
    return;
  }
  scheme = scheme_view.str();

  p++;
  if (p[0] != '/' || p[1] != '/' || p[2] == '/') {
    return;
  }
  p += 2;

  auto host_view = ConsumeUtil(&p, end, '/');
  if (host_view.empty()) {
    return;
  }
  host = host_view.str();

  auto path_view = ConsumeUtil(&p, end, '?');
  raw_path = path_view.str();

  if (*p == '?') {
    p++;
    raw_query = std::string(p, end - p);
  }
}

}  // namespace http
}  // namespace cppboot