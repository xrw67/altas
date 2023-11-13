#ifndef CPPBOOT_NET_HTTP_REQUEST_HANDLER_H_
#define CPPBOOT_NET_HTTP_REQUEST_HANDLER_H_

#include <string>
#include <list>
#include <unordered_map>
#include <functional>

namespace cppboot {
namespace http {

struct Request;
struct Response;

class ServeMux {
 public:
  typedef std::function<void(const Request&, Response*)> Func;

  ServeMux(const ServeMux&) = delete;
  ServeMux& operator=(const ServeMux&) = delete;

  ServeMux();
  void ServeHttp(Request& req, Response* rep);

  void set_handler(const std::string& path, const Func& h);

 private:
  struct FuncEntry {
    std::string pattern;
    Func fn;
  };

  std::unordered_map<std::string, FuncEntry> funcs_;
  std::list<FuncEntry> prefix_funcs_;
};

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_NET_HTTP_REQUEST_HANDLER_H_
