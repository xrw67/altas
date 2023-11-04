#ifndef CPPBOOT_NET_CONNECTION_H_
#define CPPBOOT_NET_CONNECTION_H_

#include "cppboot/net/callbacks.h"

namespace cppboot {
namespace net {

class Conn : public std::enable_shared_from_this<Conn> {
 public:
  enum State { kDisconnected, kConnected };

  Conn() : state_(kDisconnected), context_(nullptr) {}

  State state() const { return state_; }
  Context* context() const { return context_; }
  void set_context(Context* c) { context_ = c; }

  void set_conn_callback(const ConnCallback& cb) { conn_callback_ = cb; }
  void set_receive_callback(const ReceiveCallback& cb) {
    receive_callback_ = cb;
  }

  virtual void Stop() {}

  virtual void Send(const void* data, size_t len) = 0;

 protected:
  virtual ~Conn() { delete context_; }

  State state_;
  Context* context_;
  ConnCallback conn_callback_;
  ReceiveCallback receive_callback_;
};

}  // namespace net
}  // namespace cppboot

#endif  // CPPBOOT_NET_CONNECTION_H_