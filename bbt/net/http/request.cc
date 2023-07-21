#include "bbt/net/http/request.h"

#include <cctype>

#include "bbt/base/fmt.h"
#include "bbt/base/str_util.h"
#include "bbt/net/buffer.h"

namespace bbt {

namespace {

std::string GetNextToken(const char** next, const char* end, char quota) {
  const char* p = *next;
  const char* start = p;

  if (p >= end) return std::string();

  while (p != end && *p != quota) p++;
  std::string token(start, p - start);

  *next = ++p;
  return token;
}

}  // namespace

Status HttpRequest::Parse(Buffer& buffer) noexcept {
  const char* next = (const char*)buffer.Peek();
  const char* end = next + buffer.ReadableBytes();

  auto method = GetNextToken(&next, end, ' ');
  if (!set_method(method))
    return InvalidArgumentError(format("invalid method: {}", method));

  auto path = GetNextToken(&next, end, ' ');
  auto question = path.find('?');
  if (question != path.npos) {
    set_path(path.substr(0, question));
    set_query(path.substr(question + 1));
  } else {
    set_path(path);
  }

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
    set_header(field, StrTrim(value).str());
    next++;
  }

  set_body(StrTrimLeft(string_view(next, end - next)).str());
  return OkStatus();
}

void HttpRequest::ParseForm() noexcept {
  const char* next = query_.c_str();
  const char* end = next + query_.length();

  for (;;) {
    auto param = GetNextToken(&next, end, '&');
    if (param.empty()) {
      break;
    }
    auto sep = param.find('=');
    if (sep == param.npos) {
      set_form(param, "");
    } else {
      set_form(param.substr(0, sep), param.substr(sep + 1));
    }
  }
}

}  // namespace bbt