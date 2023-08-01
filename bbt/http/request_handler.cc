#include "bbt/http/request_handler.h"

#include <sstream>

#include "bbt/base/log.h"
#include "bbt/base/str_util.h"
#include "bbt/http/request.h"
#include "bbt/http/response.h"

namespace bbt {
namespace http {

RequestHandler::RequestHandler() {}

void RequestHandler::HandleRequest(const Request& req, Response* resp) {
  for (auto& i : handlers_) {
    if (StartsWithIgnoreCase(req.path, i.first)) {
      i.second(req, resp);
      return;
    }
  }
  resp->status = Response::not_found;
  resp->WriteText("Not found");
}

void RequestHandler::set_handler(const std::string& path, const HandlefFunc& func) {
  handlers_[path] = func;
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