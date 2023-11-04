#ifndef CPPBOOT_HTTP_HEADER_H_
#define CPPBOOT_HTTP_HEADER_H_

#include <string>

namespace cppboot {
namespace http {

struct Header {
  std::string name;
  std::string value;
};

}  // namespace http
}  // namespace cppboot

#endif  // CPPBOOT_HTTP_HEADER_H_
