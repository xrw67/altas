#ifndef BBT_HTTP_REQUEST_HANDLER_H_
#define BBT_HTTP_REQUEST_HANDLER_H_

#include <string>

namespace bbt {
namespace http {

struct Request;
struct Reply;

class RequestHandler {
 public:
  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  RequestHandler();
  void HandleRequest(const Request& req, Reply* rep);

 private:
  bool UrlDecode(const std::string& in, std::string& out);
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_HANDLER_H_
