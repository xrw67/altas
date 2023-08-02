#ifndef BBT_HTML_ELEMENTS_ELEMENT_H_
#define BBT_HTML_ELEMENTS_ELEMENT_H_

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

#include <bbt/base/fmt.h>

namespace bbt {
namespace html {

class Element;
typedef std::shared_ptr<Element> ElementPtr;

class Element {
 public:
  Element(const std::string& name);
  Element(const std::string& name, const std::string& text);
  virtual ~Element();

  void set_attribute(const std::string& name, const std::string& value) {
    attr_[name] = value;
  };

  Element* AddChild(Element* e);

  virtual void Dump(std::stringstream& ss, int depth);

 protected:
  std::string name_;
  std::map<std::string, std::string> attr_;

  std::string text_;
  std::vector<ElementPtr> children_;
};

}  // namespace html
}  // namespace bbt

#endif  // BBT_HTML_ELEMENTS_ELEMENT_H_