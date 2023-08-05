#include "bbt/http/server/serve_mux.h"

#include <sstream>

#include "bbt/base/log.h"
#include "bbt/base/str_util.h"
#include "bbt/http/request.h"
#include "bbt/http/response.h"

namespace bbt {
namespace http {

ServeMux::ServeMux() {}

void ServeMux::ServeHttp(Request& req, Response* resp) {
  auto it = funcs_.find(req.path);
  if (it != funcs_.end()) {
    it->second.fn(req, resp);
    return;
  }

  for (auto& i : prefix_funcs_) {
    if (StartsWithIgnoreCase(req.path, i.pattern)) {
      req.subpath = req.path.substr(i.pattern.length() - 1);
      i.fn(req, resp);
      return;
    }
  }

  resp->WriteText(Response::not_found, "Not found");
}

void ServeMux::set_handler(const std::string& pattern, const Func& func) {
  if (pattern.empty()) return;
  if (funcs_.find(pattern) != funcs_.end()) return;

  FuncEntry entry = {pattern, func};
  funcs_[pattern] = entry;

  if (pattern[pattern.length() - 1] == '/') {
    for (auto it = prefix_funcs_.begin(); it != prefix_funcs_.end(); it++) {
      if (pattern.length() > it->pattern.length()) {
        prefix_funcs_.insert(it, entry);
        return;
      }
    }
    prefix_funcs_.push_back(entry);
  }
}

}  // namespace http
}  // namespace bbt