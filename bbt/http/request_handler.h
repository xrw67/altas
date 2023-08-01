#ifndef BBT_HTTP_REQUEST_HANDLER_H_
#define BBT_HTTP_REQUEST_HANDLER_H_

#include <string>
#include <unordered_map>
#include <functional>

namespace bbt {
namespace http {

struct Request;
struct Response;

class RequestHandler {
 public:
  typedef std::function<void(const Request&, Response*)> HandlefFunc;

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  RequestHandler();
  void HandleRequest(const Request& req, Response* rep);

  void set_handler(const std::string& path, const HandlefFunc& h);

 private:
  bool UrlDecode(const std::string& in, std::string& out);

  std::unordered_map<std::string, HandlefFunc> handlers_;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_HANDLER_H_
