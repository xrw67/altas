#ifndef BBT_NET_HTTP_RESPONSE_H_
#define BBT_NET_HTTP_RESPONSE_H_

#include <map>
#include <string>

#include "bbt/util/status.h"

namespace bbt {

class Buffer;
class HttpResponse {
 public:
  enum HttpStatusCode {
    kUnknown,
    k200Ok = 200,
    k301MovedPermanently = 301,
    k400BadRequest = 400,
    k404NotFound = 404,
  };

  explicit HttpResponse(bool close)
      : status_code_(kUnknown), close_connection_(close) {}

  HttpStatusCode status_code() const noexcept { return status_code_; }
  void set_status_code(HttpStatusCode code) noexcept { status_code_ = code; }

  std::string status_message() const noexcept { return status_message_; }
  void set_status_message(const std::string& message) noexcept {
    status_message_ = message;
  }

  void set_close_connection(bool on) noexcept { close_connection_ = on; }

  void set_content_type(const std::string& content_type) {
    add_header("Content-Type", content_type);
  }

  void add_header(const std::string& key, const std::string& value) {
    headers_[key] = value;
  }

  std::string body() const noexcept { return body_; }
  void set_body(const std::string& body) { body_ = body; }

  void AppendToBuffer(Buffer* output) const;

 private:
  std::map<std::string, std::string> headers_;
  HttpStatusCode status_code_;
  std::string status_message_;
  bool close_connection_;
  std::string body_;
};

}  // namespace bbt

#endif  // BBT_NET_HTTP_RESPONSE_H_
