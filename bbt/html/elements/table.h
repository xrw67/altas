#ifndef BBT_HTML_ELEMENTS_TABLE_H_
#define BBT_HTML_ELEMENTS_TABLE_H_

#include <string>
#include <vector>

#include "element.h"

namespace bbt {
namespace html {

class Table : public Element {
 public:
  typedef std::vector<std::string> Row;

  explicit Table(const Row& heads) : Element("table"), heads_(heads) {}
  void AddRow(const Row& row) { data_.push_back(row); }

  void Dump(std::stringstream& ss, int depth);

 private:
  Row heads_;
  std::vector<Row> data_;
};

}  // namespace html
}  // namespace bbt

#endif  // BBT_HTML_ELEMENTS_TABLE_H_