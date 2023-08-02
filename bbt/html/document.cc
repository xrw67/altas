#include "bbt/html/document.h"

#include <sstream>

namespace bbt {
namespace html {

namespace {
const char* style =
    "<style>\n"
    "table, th, td {\n"
    "  border: 1px solid black;\n"
    "  border-collapse: collapse;\n"
    "}\n"
    "</style>\n";
}

Document::Document(const std::string& title)
    : title_(title), body_(new Element("body")) {}

std::string Document::Dump() const noexcept {
  std::stringstream ss;
  ss << "<!DOCTYPE html>\n";
  ss << "<html>\n";
  ss << "<head>\n";
  ss << "<meta charset=\"UTF-8\">\n";
  ss << style;
  ss << "<title>" << title_ << "</title>\n";
  ss << "</head>\n";
  body_->Dump(ss, 0);
  ss << "</html>\n";

  return ss.str();
}

}  // namespace html
}  // namespace bbt