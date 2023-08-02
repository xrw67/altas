#ifndef BBT_HTTP_PARAM_H_
#define BBT_HTTP_PARAM_H_

#include <string>
#include <map>

#include "bbt/base/status.h"
#include "bbt/base/string_view.h"

namespace bbt {
namespace http {

/**
 * @brief 来自query或form的kv数据
 *
 */
class FormData {
 public:
  void Set(const char* key, const std::string& value) noexcept;
  std::string Get(const char* key) const noexcept;

  Status Parse(string_view query) noexcept;
  std::string Encode() const noexcept;

 private:
  // TODO: values其实是个数据 std::vector<std::string>
  std::map<std::string, std::string> values_;
};

}  // namespace http
}  // namespace bbt

#endif  // BBT_HTTP_PARAM_H_
