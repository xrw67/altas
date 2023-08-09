#ifndef BBT_NET_PROTO_H_
#define BBT_NET_PROTO_H_

#include <memory>
#include "bbt/base/status.h"

namespace bbt {
namespace net {

class Connection {
 public:

  virtual Status Write(const char* data, size_t size) = 0;

 protected:
  virtual ~Connection() {}
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_PROTO_H_
