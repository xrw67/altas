#ifndef CPPBOOT_NET_HTTP_CLIENT_H_
#define CPPBOOT_NET_HTTP_CLIENT_H_

#include <string>

#include "asio.hpp"

#include "cppboot/base/status.h"
#include "cppboot/base/json.h"
#include "cppboot/net/http/request.h"
#include "cppboot/net/http/response.h"

namespace cppboot {
namespace http {

// TODO: add https support

Status Get(const std::string& url, Response* resp);
Status Post(const std::string& url, const std::string& content, Response* resp);
Status PostJson(const std::string& url, const json& data, Response* resp);
Status PostForm(const std::string& url, const FormData& data, Response* resp);

class Client {
 public:
  Client();
  ~Client();
  Status Do(const Request& req, Response* resp);

 private:
  asio::ip::tcp::resolver::results_type GetEndpointFromUrl(
      const cppboot::http::Url& url);
  Status ReadResponse(Response* resp);

  asio::io_context io_context_;
  asio::ip::tcp::socket socket_;
};

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_NET_HTTP_CLIENT_H_
