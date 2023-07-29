#include "bbt/http/request_handler.h"

#include <sstream>

#include "bbt/base/log.h"
#include "bbt/http/request.h"
#include "bbt/http/response.h"

namespace bbt {
namespace http {

RequestHandler::RequestHandler() {}

void RequestHandler::HandleRequest(const Request& req, Response* rep) {
  BBT_LOG(DEBUG, ">>>>> New Requset <<<<<");
  BBT_LOG(DEBUG, "req.method: {}", req.method);
  BBT_LOG(DEBUG, "req.uri: {}", req.uri);
  for (auto header : req.headers) {
    BBT_LOG(DEBUG, "req.header: {}: {}", header.name, header.value);
  }

  rep->status = Response::ok;
  rep->content = "Hello World!";
}

bool RequestHandler::UrlDecode(const std::string& in, std::string& out) {
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '%') {
      if (i + 3 <= in.size()) {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value) {
          out += static_cast<char>(value);
          i += 2;
        } else {
          return false;
        }
      } else {
        return false;
      }
    } else if (in[i] == '+') {
      out += ' ';
    } else {
      out += in[i];
    }
  }
  return true;
}

}  // namespace http
}  // namespace bbt