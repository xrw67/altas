#include "bbt/http/server/file_server.h"

#include <fstream>
#include <sstream>
#include <string>

#include "bbt/http/request.h"
#include "bbt/http/response.h"

namespace bbt {
namespace http {

namespace {

static const struct mapping {
  const char* extension;
  const char* mime_type;
} mappings[] = {
    {"gif", "image/gif"},  {"htm", "text/html"}, {"html", "text/html"},
    {"jpg", "image/jpeg"}, {"png", "image/png"},
};

std::string ExtensionToType(const std::string& extension) {
  for (mapping m : mappings) {
    if (m.extension == extension) {
      return m.mime_type;
    }
  }

  return "text/plain";
}

}  // namespace

void FileServer::ServeHttp(const Request& req, Response* rep) {
#if 0
  // Decode url to path.
  std::string request_path = req.path;

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/' ||
      request_path.find("..") != std::string::npos) {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/') {
    request_path += "index.html";
  }

  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
    extension = request_path.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::string full_path = root_ + request_path;
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is) {
    rep = reply::stock_reply(reply::not_found);
    return;
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    rep.content.append(buf, is.gcount());
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = mime_types::extension_to_type(extension);
#endif
}

}  // namespace http
}  // namespace bbt