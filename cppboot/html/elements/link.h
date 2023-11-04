#ifndef CPPBOOT_HTML_ELEMENTS_LINK_H_
#define CPPBOOT_HTML_ELEMENTS_LINK_H_

#include "element.h"

namespace cppboot {
namespace html {

class Link : public Element {
 public:
  Link(const std::string& text, const std::string& url) : Element("a", text) {
    set_attribute("href", url);
    set_attribute("target", "_blank");
  }
};

}  // namespace html
}  // namespace cppboot

#endif  // CPPBOOT_HTML_ELEMENTS_LINK_H_