#ifndef BBT_HTML_ELEMENTS_LINK_H_
#define BBT_HTML_ELEMENTS_LINK_H_

#include "element.h"

namespace bbt {
namespace html {

class Link : public Element {
 public:
  typedef std::vector<std::string> Row;

  Link(const std::string& text, const std::string& url) : Element("a", text) {
    set_attribute("href", url);
    set_attribute("target", "_blank");
  }
};

}  // namespace html
}  // namespace bbt

#endif  // BBT_HTML_ELEMENTS_LINK_H_