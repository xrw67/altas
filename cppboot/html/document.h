#ifndef CPPBOOT_HTML_DOCUMENT_H_
#define CPPBOOT_HTML_DOCUMENT_H_

#include <string>

#include "asio.hpp"

#include "elements.h"

namespace cppboot {
namespace html {

class Document {
 public:
  explicit Document(const std::string& title);
  void set_tittle(const std::string& s) noexcept { title_ = s; }
  ElementPtr body() { return body_; }
  std::string Dump() const noexcept;

 public:
  std::string title_;
  ElementPtr body_;
};

}  // namespace html
}  // namespace cppboot
#endif  // CPPBOOT_HTML_DOCUMENT_H_
