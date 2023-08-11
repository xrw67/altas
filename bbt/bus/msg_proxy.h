#ifndef BBT_BUS_MSG_PROXY_H_
#define BBT_BUB_MSG_PROXY_H_

#include <mutex>
#include <string>
#include <set>
#include <unordered_map>

#include "bbt/base/status.h"
#include "bbt/net/connection.h"
#include "bbt/bus/msg.h"

namespace bbt {
namespace bus {

class MsgProxy {
 public:
  Status AddMethod(const bbt::net::ConnectionPtr& owner,
                   const std::string& method);
  void RemoveMethod(const std::string& method);
  void RemoveAllMethodsFromConnection(const bbt::net::ConnectionPtr& conn);

  Status ProxyRequestMsg(Msg* req);

 private:
  std::mutex mutex_;
  std::unordered_map<std::string, bbt::net::ConnectionPtr> methods_;
  std::unordered_map<bbt::net::ConnectionPtr, std::set<std::string>>
      conn_methods_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUB_MSG_PROXY_H_
