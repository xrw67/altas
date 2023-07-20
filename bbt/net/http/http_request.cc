#include "bbt/net/http/http_request.h"

#include <cctype>

#include "bbt/util/fmt.h"
#include "bbt/util/str_util.h"
#include "bbt/net/buffer.h"

namespace bbt {

namespace {

string_view GetNextToken(const char** next, const char* end, char quota) {
  const char* p = *next;
  const char* start = p;

  while (p != end && *p != quota) p++;
  string_view token(start, p - start);

  *next = ++p;
  return token;
}

}  // namespace

Status HttpRequest::Parse(Buffer& buffer) noexcept {
  const char* next = (const char*)buffer.Peek();
  const char* end = next + buffer.ReadableBytes();

  auto method = GetNextToken(&next, end, ' ');
  if (!set_method(method.str()))
    return InvalidArgumentError(format("invalid method: {}", method));

  auto path = GetNextToken(&next, end, ' ');
  set_path(path.str());

  auto version = GetNextToken(&next, end, '\r');
  if (version == string_view("HTTP/1.1"))
    set_version(HttpRequest::kHttp11);
  else if (version == string_view("HTTP/1.0"))
    set_version(HttpRequest::kHttp10);

  next++;
  // Headers

  for (;;) {
    auto header = GetNextToken(&next, end, '\r');
    if (header.empty()) {
      break;
    }

    auto pos = header.find_first_of(':');
    if (pos == string_view::npos)
      return InvalidArgumentError(format("invalid Header: {}", header));

    auto field = header.substr(0, pos);
    auto value =
        (pos + 1 < header.size()) ? header.substr(pos + 2) : string_view();
    set_header(field.str(), StrTrim(value).str());
    next++;
  }

  set_body(StrTrimLeft(string_view(next, end - next)).str());
  return OkStatus();
}

}  // namespace bbt