#ifndef CPPBOOT_NET_CALLBACKS_H_
#define CPPBOOT_NET_CALLBACKS_H_

#include <memory>
#include <functional>

#include "asio.hpp"

#include "cppboot/base/status.h"

namespace cppboot {
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
}  // namespace cppboot

#endif  // CPPBOOT_NET_CALLBACKS_H_