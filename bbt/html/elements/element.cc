#include "element.h"

namespace bbt {
namespace html {

Element::Element(const std::string& name) : name_(name) {}

Element::Element(const std::string& name,
                 const std::string& text = std::string())
    : name_(name), text_(text) {}

Element::~Element() {}

Element* Element::AddChild(Element* e) {
  children_.push_back(ElementPtr(e));
  return e;
}

void Element::Dump(std::stringstream& ss, int depth) {
  //
  // IF NO text_: <NAME ATTRIBUTE>TEXT</NAME>
  // IF NO children_: <NAME ATTRIBUTE>
  // IF HAS children_: <NAME ATTR>CHILDREN</NAME>
  //
  std::string prefix(depth, '\t');

  ss << prefix << "<" << name_;
  for (auto i : attr_) {
    ss << " " << i.first << "=\"" << i.second << "\"";
  }
  ss << ">";

  if (!children_.empty()) {
    ss << "\n";
    for (auto i : children_) {
      i->Dump(ss, depth + 1);
    }
    ss << prefix << "</" << name_ << ">\n";
  } else if (!text_.empty()) {
    ss << text_ << "</" << name_ << ">\n";
  } else {
    ss << "\n";
  }
};

}  // namespace html
}  // namespace bbt