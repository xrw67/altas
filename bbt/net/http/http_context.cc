#include "bbt/net/http/http_context.h"

#include <cctype>

#include "bbt/util/fmt.h"

namespace bbt {

inline bool IsSpearator(char c, const char* separator) {
  while (separator && *separator) {
    if (c == *separator) return true;
    separator++;
  }
  return false;
}

string_view GetNextToken(const char** next, const char* end,
                         const char* separator) {
  const char* token;
  const char* start = *next;

  while (start != end && IsSpearator(*start, separator)) {
    start++;
  }

  token = start;

  while (start != end && !IsSpearator(*start, separator)) {
    start++;
  }

  *next = start;
  return string_view(token, start - token);
}

Status HttpContext::ParseRequest(string_view payload) noexcept {
  const char* next = payload.data();
  const char* end = next + payload.size();

  auto method = GetNextToken(&next, end, " ");
  if (!request_.set_method(method))
    return InvalidArgumentError(format("invalid method: {}", method));

  auto path = GetNextToken(&next, end, " ");
  request_.set_path(path);

  auto version = GetNextToken(&next, end, " \r\n");

  if (version == string_view("HTTP/1.1"))
    request_.set_version(HttpRequest::kHttp11);
  else if (version == string_view("HTTP/1.0"))
    request_.set_version(HttpRequest::kHttp10);

  // Headers

  for (;;) {
    auto header = GetNextToken(&next, end, "\r\n");
    if (header.empty()) break;

    auto pos = header.find_first_of(':');
    if (pos == string_view::npos)
      return InvalidArgumentError(format("invalid Header: {}", header));

    auto field = header.substr(0, pos);
    auto value =
        (pos + 1 < header.size()) ? header.substr(pos + 2) : string_view();
    request_.set_header(field, StrTrim(value));
  }

  request_.set_body(StrTrimLeft(string_view(next, end - next)));
  return OkStatus();
}

}  // namespace bbt