#include "bbt/net/http/http_request.h"

#include <cctype>

#include "bbt/util/fmt.h"
#include "bbt/util/str_util.h"

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

HttpRequest::HttpRequest() noexcept
    : method_(HttpRequest::kInvalid), version_(HttpRequest::kUnknown) {}

HttpRequest::~HttpRequest() {}

Status HttpRequest::Parse(HttpRequest& request, string_view content) noexcept {
  const char* next = content.data();
  const char* end = next + content.size();

  auto method = GetNextToken(&next, end, ' ');
  if (!request.set_method(method))
    return InvalidArgumentError(format("invalid method: {}", method));

  auto path = GetNextToken(&next, end, ' ');
  request.set_path(path);

  auto version = GetNextToken(&next, end, '\r');
  if (version == string_view("HTTP/1.1"))
    request.set_version(HttpRequest::kHttp11);
  else if (version == string_view("HTTP/1.0"))
    request.set_version(HttpRequest::kHttp10);

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
    request.set_header(field, StrTrim(value));
    next++;
  }

  request.set_body(StrTrimLeft(string_view(next, end - next)));
  return OkStatus();
}

HttpRequest::Method HttpRequest::method() const noexcept { return method_; }

bool HttpRequest::set_method(string_view method) {
  method_ = (method == "GET")      ? kGet
            : (method == "POST")   ? kPost
            : (method == "HEAD")   ? kHead
            : (method == "PUT")    ? kPut
            : (method == "DELETE") ? kDelete
                                   : kInvalid;
  return method_ != kInvalid;
}

HttpRequest::Version HttpRequest::version() const noexcept { return version_; }

void HttpRequest::set_version(Version version) noexcept { version_ = version; }

string_view HttpRequest::path() const noexcept { return path_; }
void HttpRequest::set_path(string_view path) noexcept { path_ = path; }
string_view HttpRequest::query() const noexcept { return query_; }
void HttpRequest::set_query(string_view query) noexcept { query_ = query; }

void HttpRequest::set_header(string_view field, string_view value) noexcept {
  headers_[StrTrim(field)] = StrTrim(value);
}

string_view HttpRequest::header(string_view field) const noexcept {
  auto it = headers_.find(field);
  return (it != headers_.end()) ? it->second : string_view();
}

const HttpRequest::HeaderMap& HttpRequest::headers() const noexcept {
  return headers_;
}

string_view HttpRequest::body() const noexcept { return body_; }

void HttpRequest::set_body(string_view body) noexcept { body_ = body; }

}  // namespace bbt