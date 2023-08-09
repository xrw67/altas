#include "bbt/http/request.h"
#include "bbt/http/url.h"

namespace bbt {
namespace http {

Request::Request() : url("") {}

Request::Request(const std::string& method, const std::string& raw_url)
    : method(method), url(raw_url) {
  if (url.IsValid()) {
    path = url.raw_path;
    if (url.raw_query.empty())
      uri = url.raw_path;
    else
      uri = url.raw_path + "?" + url.raw_query;

    host = url.host;
  }
}

Request::~Request() {}

void Request::set_header(const std::string& name,
                         const std::string& value) noexcept {
  for (auto& i : headers) {
    if (i.name == name) {
      i.value = value;
      return;
    }
  }

  Header h = {name, value};
  headers.push_back(h);
}

void Request::to_buffers(asio::streambuf* buf) const noexcept {
  // Form the request. We specify the "TcpConnection: close" header so that the
  // server will close the socket after transmitting the response. This will
  // allow us to treat all data up until the EOF as the content.

  std::ostream request_stream(buf);
  request_stream << method << " ";
  if (url.raw_query.empty())
    request_stream << url.raw_path;
  else
    request_stream << url.raw_path << "?" << url.raw_query;

  request_stream << " HTTP/1.0\r\n";
  request_stream << "Host: " << url.host << "\r\n";
  request_stream << "Accept: */*\r\n";
  if (!content.empty()) {
    request_stream << "Content-Length: " << content.length() << "\r\n";
  }
  request_stream << "TcpConnection: close\r\n\r\n";
  if (!content.empty()) {
    request_stream << content;
  }
}

}  // namespace http
}  // namespace bbt