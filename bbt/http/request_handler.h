#ifndef BBT_HTTP_REQUEST_HANDLER_H_
#define BBT_HTTP_REQUEST_HANDLER_H_

#include <string>
#include <functional>

namespace bbt {
namespace http {

struct Request;
struct Response;

class RequestHandler {
 public:
  typedef std::function<void(const Request&, Response*)> Handler;

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  RequestHandler();
  void HandleRequest(const Request& req, Response* rep);

  void set_handler(const Handler& h) { handler_ = h; }

 private:
  bool UrlDecode(const std::string& in, std::string& out);

  Handler handler_;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_HANDLER_H_
