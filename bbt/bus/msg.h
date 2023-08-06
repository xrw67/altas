#ifndef BBT_BUS_MSG_H_
#define BBT_BUS_MSG_H_

#include <string>
#include <map>

#include "bbt/base/status.h"

namespace bbt {
namespace bus {

class Msg {
 public:
  typedef std::map<std::string, std::string> Type;

  Msg() : id_(0) {}

  long id() const { return id_; }
  void set_id(long id) { id_ = id; }
  std::string method() const { return method_; }
  void set_method(const std::string& method) { method_ = method; }
  bool Has(const std::string& key) const;
  void Set(const std::string& key, const std::string& value);
  std::string Get(const std::string& key) const;

  const Type& data() const { return values_; }

 private:
  long id_;
  std::string method_;
  Type values_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_MSG_H_