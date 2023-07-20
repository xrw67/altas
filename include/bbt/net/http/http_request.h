#ifndef BBT_NET_HTTP_REQUEST_H_
#define BBT_NET_HTTP_REQUEST_H_

#include <map>

#include "bbt/util/status.h"
#include "bbt/util/string_view.h"


namespace bbt {

class HttpRequest final {
 public:
  typedef std::map<string_view, string_view> HeaderMap;

  enum Method { kInvalid, kGet, kPost, kHead, kPut, kDelete };
  enum Version { kUnknown, kHttp10, kHttp11 };

  HttpRequest() noexcept;
  ~HttpRequest() noexcept;

  static Status Parse(HttpRequest& request, string_view content) noexcept;

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

}  // namespace bbt

#endif  // BBT_NET_HTTP_REQUEST_H_
