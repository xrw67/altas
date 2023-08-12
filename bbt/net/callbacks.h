#ifndef BBT_NET_CALLBACKS_H_
#define BBT_NET_CALLBACKS_H_

#include <memory>
#include <functional>

#include "asio.hpp"

#include "bbt/base/status.h"

namespace bbt {
namespace net {

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class Buffer;

/**
 * @brief Connection's context
 *
 */
class Context {
 public:
  virtual ~Context() {}
};

class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;
typedef std::function<void(const ConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const ConnectionPtr&, Buffer*)> ReadCallback;

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_CALLBACKS_H_