#ifndef BBT_HTTP_HEADER_H_
#define BBT_HTTP_HEADER_H_

#include <string>

namespace bbt {
namespace http {

struct Header {
  std::string name;
  std::string value;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_HEADER_H_
