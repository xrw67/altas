#ifndef CPPBOOT_HTTP_SERVER_FILE_SERVER_H_
#define CPPBOOT_HTTP_SERVER_FILE_SERVER_H_

#include <string>
#include <map>

namespace cppboot {
namespace http {

struct Request;
struct Response;

class FileServer final {
 public:
  FileServer() noexcept {}
  FileServer(const std::string& root) noexcept : root_(root) {}
  ~FileServer() noexcept {}

  void ServeHttp(const Request& req, Response* rep);

  void AddFile(const std::string& path, const std::string& content);

  std::string root() const noexcept { return root_; }
  void set_root(const std::string& root) noexcept { root_ = root; }

 private:
  std::string root_;

  std::map<std::string, std::string> files_;
};

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_HTTP_SERVER_FILE_SERVER_H_
