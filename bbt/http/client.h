#ifndef BBT_HTTP_CLIENT_H_
#define BBT_HTTP_CLIENT_H_

#include <string>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/base/json.h"
#include "bbt/http/request.h"
#include "bbt/http/response.h"

namespace bbt {
namespace http {

// TODO: add https support

Status Get(const std::string& url, Response* resp);
Status Post(const std::string& url, const std::string& content, Response* resp);
//Status PostJson(const std::string& url, const json& content,Response *resp);
//Status PostForm(const std::string& url, const Param& content,Response *resp);


class Client {
 public:
  Client();
  ~Client();
  Status Do(const Request& req, Response* resp);

 private:
  asio::ip::tcp::resolver::results_type GetEndpointFromUrl(
      const bbt::http::Url& url);
  Status ReadResponse(Response* resp);

  asio::io_context io_context_;
  asio::ip::tcp::socket socket_;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_CLIENT_H_
