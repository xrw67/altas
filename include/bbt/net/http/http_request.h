#ifndef BBT_NET_HTTP_REQUEST_H_
#define BBT_NET_HTTP_REQUEST_H_

#include <map>

#include "bbt/util/string_view.h"
#include "bbt/util/str_util.h"

namespace bbt {

class HttpRequest final {
 public:
  typedef std::map<string_view, string_view> HeaderMap;

  enum Method { kInvalid, kGet, kPost, kHead, kPut, kDelete };
  enum Version { kUnknown, kHttp10, kHttp11 };

  HttpRequest() noexcept : method_(kInvalid), version_(kUnknown) {}

  Method method() const noexcept;
  bool set_method(string_view method);
  Version version() const noexcept;
  void set_version(Version version) noexcept;
  string_view path() const noexcept;
  void set_path(string_view path) noexcept;
  string_view query() const noexcept;
  void set_query(string_view query) noexcept;

  string_view header(string_view field) const noexcept;
  const HeaderMap& headers() const noexcept;
  void set_header(string_view field, string_view value) noexcept;

  string_view body() const noexcept;
  void set_body(string_view body) noexcept;

 private:
  Method method_;
  Version version_;
  string_view path_;
  string_view query_;
  HeaderMap headers_;
  string_view body_;
};

inline HttpRequest::Method HttpRequest::method() const noexcept {
  return method_;
}

inline bool HttpRequest::set_method(string_view method) {
  method_ = (method == "GET")      ? kGet
            : (method == "POST")   ? kPost
            : (method == "HEAD")   ? kHead
            : (method == "PUT")    ? kPut
            : (method == "DELETE") ? kDelete
                                   : kInvalid;
  return method_ != kInvalid;
}

inline HttpRequest::Version HttpRequest::version() const noexcept {
  return version_;
}

inline void HttpRequest::set_version(Version version) noexcept {
  version_ = version;
}

inline string_view HttpRequest::path() const noexcept { return path_; }
inline void HttpRequest::set_path(string_view path) noexcept { path_ = path; }
inline string_view HttpRequest::query() const noexcept { return query_; }
inline void HttpRequest::set_query(string_view query) noexcept {
  query_ = query;
}

inline void HttpRequest::set_header(string_view field,
                                    string_view value) noexcept {
  headers_[StrTrim(field)] = StrTrim(value);
}

inline string_view HttpRequest::header(string_view field) const noexcept {
  auto it = headers_.find(field);
  return (it != headers_.end()) ? it->second : string_view();
}

inline const HttpRequest::HeaderMap& HttpRequest::headers() const noexcept {
  return headers_;
}

inline string_view HttpRequest::body() const noexcept { return body_; }

void HttpRequest::set_body(string_view body) noexcept { body_ = body; }

}  // namespace bbt

#endif  // BBT_NET_HTTP_REQUEST_H_
