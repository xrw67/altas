#include "cppboot/http/server/request_parser.h"
#include "cppboot/http/request.h"

namespace cppboot {
namespace http {

RequestParser::RequestParser() : state_(method_start) {}

void RequestParser::reset() { state_ = method_start; }

void RequestParser::parse_uri(Request& req) {
  auto sep = req.uri.find_first_of('?');
  if (sep == std::string::npos) {
    req.path = req.uri;
  } else {
    req.path = req.uri.substr(0, sep);
    req.params.Parse(req.uri.substr(sep + 1));
  }
}

RequestParser::result_type RequestParser::consume(Request& req, char input) {
  switch (state_) {
    case method_start:
      if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return bad;
      } else {
        state_ = method;
        req.method.push_back(input);
        return indeterminate;
      }
    case method:
      if (input == ' ') {
        state_ = uri;
        return indeterminate;
      } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return bad;
      } else {
        req.method.push_back(input);
        return indeterminate;
      }
    case uri:
      if (input == ' ') {
        state_ = http_version_h;
        return indeterminate;
      } else if (is_ctl(input)) {
        return bad;
      } else {
        req.uri.push_back(input);
        return indeterminate;
      }
    case http_version_h:
      if (input == 'H') {
        state_ = http_version_t_1;
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_t_1:
      if (input == 'T') {
        state_ = http_version_t_2;
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_t_2:
      if (input == 'T') {
        state_ = http_version_p;
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_p:
      if (input == 'P') {
        state_ = http_version_slash;
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_slash:
      if (input == '/') {
        req.http_version_major = 0;
        req.http_version_minor = 0;
        state_ = http_version_major_start;
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_major_start:
      if (is_digit(input)) {
        req.http_version_major = req.http_version_major * 10 + input - '0';
        state_ = http_version_major;
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_major:
      if (input == '.') {
        state_ = http_version_minor_start;
        return indeterminate;
      } else if (is_digit(input)) {
        req.http_version_major = req.http_version_major * 10 + input - '0';
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_minor_start:
      if (is_digit(input)) {
        req.http_version_minor = req.http_version_minor * 10 + input - '0';
        state_ = http_version_minor;
        return indeterminate;
      } else {
        return bad;
      }
    case http_version_minor:
      if (input == '\r') {
        state_ = expecting_newline_1;
        return indeterminate;
      } else if (is_digit(input)) {
        req.http_version_minor = req.http_version_minor * 10 + input - '0';
        return indeterminate;
      } else {
        return bad;
      }
    case expecting_newline_1:
      if (input == '\n') {
        state_ = header_line_start;
        return indeterminate;
      } else {
        return bad;
      }
    case header_line_start:
      if (input == '\r') {
        state_ = expecting_newline_3;
        return indeterminate;
      } else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
        state_ = header_lws;
        return indeterminate;
      } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return bad;
      } else {
        req.headers.push_back(Header());
        req.headers.back().name.push_back(input);
        state_ = header_name;
        return indeterminate;
      }
    case header_lws:
      if (input == '\r') {
        state_ = expecting_newline_2;
        return indeterminate;
      } else if (input == ' ' || input == '\t') {
        return indeterminate;
      } else if (is_ctl(input)) {
        return bad;
      } else {
        state_ = header_value;
        req.headers.back().value.push_back(input);
        return indeterminate;
      }
    case header_name:
      if (input == ':') {
        state_ = space_before_header_value;
        return indeterminate;
      } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
        return bad;
      } else {
        req.headers.back().name.push_back(input);
        return indeterminate;
      }
    case space_before_header_value:
      if (input == ' ') {
        state_ = header_value;
        return indeterminate;
      } else {
        return bad;
      }
    case header_value:
      if (input == '\r') {
        state_ = expecting_newline_2;
        return indeterminate;
      } else if (is_ctl(input)) {
        return bad;
      } else {
        req.headers.back().value.push_back(input);
        return indeterminate;
      }
    case expecting_newline_2:
      if (input == '\n') {
        state_ = header_line_start;
        return indeterminate;
      } else {
        return bad;
      }
    case expecting_newline_3:
      return (input == '\n') ? good : bad;
    default:
      return bad;
  }
}

bool RequestParser::is_char(int c) { return c >= 0 && c <= 127; }

bool RequestParser::is_ctl(int c) { return (c >= 0 && c <= 31) || (c == 127); }

bool RequestParser::is_tspecial(int c) {
  switch (c) {
    case '(':
    case ')':
    case '<':
    case '>':
    case '@':
    case ',':
    case ';':
    case ':':
    case '\\':
    case '"':
    case '/':
    case '[':
    case ']':
    case '?':
    case '=':
    case '{':
    case '}':
    case ' ':
    case '\t':
      return true;
    default:
      return false;
  }
}

bool RequestParser::is_digit(int c) { return c >= '0' && c <= '9'; }

bool RequestParser::UrlDecode(const std::string& in, std::string& out) {
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
}  // namespace cppboot
