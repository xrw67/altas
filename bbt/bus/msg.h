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

class MsgPacker {
 public:
  virtual void Pack(const Msg& msg, std::string* data) = 0;
  virtual Status UnPack(const std::string& data, Msg* msg) = 0;

 protected:
  virtual ~MsgPacker() {}
};

class JsonPacker : public MsgPacker {
 public:
  void Pack(const Msg& msg, std::string* result);
  Status UnPack(const std::string& data, Msg* msg);
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_MSG_H_