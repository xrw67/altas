#ifndef BBT_HTML_DOCUMENT_H_
#define BBT_HTML_DOCUMENT_H_

#include <string>

#include "asio.hpp"

#include "elements.h"

namespace bbt {
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
}  // namespace bbt
#endif  // BBT_HTML_DOCUMENT_H_
