#ifndef BBT_BUS_MSG_PROXY_H_
#define BBT_BUB_MSG_PROXY_H_

#include <mutex>
#include <string>
#include <set>
#include <unordered_map>

#include "bbt/base/status.h"
#include "bbt/net/base_connection.h"
#include "bbt/bus/msg.h"

namespace bbt {
namespace bus {

class MsgProxy {
 public:
  Status AddMethod(const bbt::net::BaseConnectionPtr& owner,
                   const std::string& method);
  void RemoveMethod(const std::string& method);
  void RemoveAllMethodsFromConnection(const bbt::net::BaseConnectionPtr& conn);

  Status ProxyRequestMsg(Msg* req);

 private:
  std::mutex mutex_;
  std::unordered_map<std::string, bbt::net::BaseConnectionPtr> methods_;
  std::unordered_map<bbt::net::BaseConnectionPtr, std::set<std::string>>
      conn_methods_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUB_MSG_PROXY_H_
