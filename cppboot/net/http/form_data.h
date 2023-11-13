#ifndef CPPBOOT_NET_HTTP_PARAM_H_
#define CPPBOOT_NET_HTTP_PARAM_H_

#include <string>
#include <map>

#include "cppboot/base/status.h"
#include "cppboot/base/string_view.h"

namespace cppboot {
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
}  // namespace cppboot

#endif  // CPPBOOT_NET_HTTP_PARAM_H_
