#ifndef BBT_NET_HTTP_REQUEST_H_
#define BBT_NET_HTTP_REQUEST_H_

#include <map>
#include <string>

#include "bbt/util/status.h"

namespace bbt {

class Buffer;
class HttpRequest final {
 public:
  typedef std::map<std::string, std::string> HeaderMap;

  enum Method { kInvalid, kGet, kPost, kHead, kPut, kDelete };
  enum Version { kUnknown, kHttp10, kHttp11 };

  HttpRequest() noexcept
      : method_(HttpRequest::kInvalid), version_(HttpRequest::kUnknown) {}

  Status Parse(Buffer& buffer) noexcept;

  Method method() const noexcept { return method_; }
  bool set_method(std::string method) {
    method_ = (method == "GET")      ? kGet
              : (method == "POST")   ? kPost
              : (method == "HEAD")   ? kHead
              : (method == "PUT")    ? kPut
              : (method == "DELETE") ? kDelete
                                     : kInvalid;
    return method_ != kInvalid;
  }

  Version version() const noexcept { return version_; }
  void set_version(Version version) noexcept { version_ = version; }

  std::string path() const noexcept { return path_; }
  void set_path(std::string path) noexcept { path_ = path; }

  std::string query() const noexcept { return query_; }
  void set_query(std::string query) noexcept { query_ = query; }

  std::string header(std::string field) const noexcept {
    auto it = headers_.find(field);
    return (it != headers_.end()) ? it->second : "";
  }
  const HeaderMap& headers() const noexcept { return headers_; }
  void set_header(std::string field, std::string value) noexcept {
    headers_[field] = value;
  }

  std::string body() const noexcept { return body_; }
  void set_body(std::string body) noexcept { body_ = body; }

 private:
  Method method_;
  Version version_;
  std::string path_;
  std::string query_;
  HeaderMap headers_;
  std::string body_;
};

}  // namespace bbt

#endif  // BBT_NET_HTTP_REQUEST_H_
