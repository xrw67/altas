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
 * @brief Conn's context
 *
 */
class Context {
 public:
  virtual ~Context() {}
};

class Conn;
typedef std::shared_ptr<Conn> ConnPtr;
typedef std::function<void(const ConnPtr&)> ConnCallback;
typedef std::function<void(const ConnPtr&, Buffer*)> ReceiveCallback;

class TcpConn;
typedef std::shared_ptr<TcpConn> TcpConnPtr;

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_CALLBACKS_H_