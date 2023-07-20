#ifndef BBT_NET_HTTP_HTTP_SERVER_H_
#define BBT_NET_HTTP_HTTP_SERVER_H_

#include <functional>

namespace bbt {

class HttpRequest;
class HttpResponse;

class HttpServer {
 public:
  typedef std::function<void(HttpRequest&, HttpResponse*)> HttpCallback;

  void set_http_callback(const HttpCallback& cb) { http_callback_ = cb; }

  virtual void Start() noexcept = 0;

 protected:
  HttpCallback http_callback_;
};

}  // namespace bbt

#endif  // BBT_NET_HTTP_HTTP_SERVER_H_
