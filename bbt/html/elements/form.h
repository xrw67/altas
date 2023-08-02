#ifndef BBT_HTML_ELEMENTS_FORM_H_
#define BBT_HTML_ELEMENTS_FORM_H_

#include "element.h"

namespace bbt {
namespace html {

class Form : public Element {
 public:
  Form(const std::string& action) : Element("form") {
    set_attribute("action", action);
  }
};

class Input : public Element {
 public:
  Input(const std::string& id, const std::string& label,
        const std::string& value)
      : Element(""), id_(id), label_(label), value_(value) {}

  void Dump(std::stringstream& ss, int depth) {
    //
    // <label for="fname">First name:</label>
    // <input type="text" id="fname" name="fname" value="John"><br>
    //
    std::string prefix(depth, '\t');

    ss << prefix << "<label for=\"" << id_ << "\">" << label_ << "</label>\n";
    ss << prefix << "<input type=\"text\" id=\"" << id_ << "\" name=\"" << id_
       << "\" value=\"" << value_ << "\"><br>\n";
  }

 private:
  std::string id_;
  std::string label_;
  std::string value_;
};

class SubmitButton : public Element {
 public:
  SubmitButton(const std::string& text) : Element("input") {
    set_attribute("type", "submit");
    set_attribute("value", text);
  }
};

}  // namespace html
}  // namespace bbt

#endif  // BBT_HTML_ELEMENTS_FORM_H_