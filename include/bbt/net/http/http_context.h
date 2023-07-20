#ifndef BBT_NET_HTTP_CONTEXT_H_
#define BBT_NET_HTTP_CONTEXT_H_

#include "bbt/util/status.h"
#include "bbt/util/string_view.h"

#include "bbt/net/http/http_request.h"

namespace bbt {

class HttpContext {
 public:
  HttpRequest& request() noexcept;

  Status ParseRequest(string_view payload) noexcept;

 private:
  HttpRequest request_;
};

inline HttpRequest& HttpContext::request() noexcept { return request_; }

}  // namespace bbt

#endif  // BBT_NET_HTTP_CONTEXT_H_
