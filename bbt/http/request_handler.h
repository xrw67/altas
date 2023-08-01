#ifndef BBT_HTTP_REQUEST_HANDLER_H_
#define BBT_HTTP_REQUEST_HANDLER_H_

#include <string>
#include <list>
#include <unordered_map>
#include <functional>

namespace bbt {
namespace http {

struct Request;
struct Response;

class RequestHandler {
 public:
  typedef std::function<void(const Request&, Response*)> Func;

  RequestHandler(const RequestHandler&) = delete;
  RequestHandler& operator=(const RequestHandler&) = delete;

  RequestHandler();
  void HandleRequest(const Request& req, Response* rep);

  void set_handler(const std::string& path, const Func& h);

 private:
  bool UrlDecode(const std::string& in, std::string& out);

  struct FuncEntry {
    std::string pattern;
    Func fn;
  };

  std::unordered_map<std::string, FuncEntry> funcs_;
  std::list<FuncEntry> prefix_funcs_;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_REQUEST_HANDLER_H_
