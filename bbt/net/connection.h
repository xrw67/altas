#ifndef BBT_NET_CONNECTION_H_
#define BBT_NET_CONNECTION_H_

#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  enum State { kDisconnected, kConnected };

  Connection() : state_(kDisconnected), context_(nullptr) {}

  State state() const { return state_; }
  Context* context() const { return context_; }
  void set_context(Context* c) { context_ = c; }

  void set_connection_callback(const ConnectionCallback& cb) {
    connection_callback_ = cb;
  }
  void set_read_callback(const ReadCallback& cb) { read_callback_ = cb; }

  virtual void Start() {
    state_ = kConnected;
    connection_callback_(shared_from_this());
  }

  virtual void Stop() {
    state_ = kDisconnected;
    connection_callback_(shared_from_this());
  }

  virtual void Send(const void* data, size_t len) = 0;

 protected:
  virtual ~Connection() { delete context_; }

  State state_;
  Context* context_;
  ConnectionCallback connection_callback_;
  ReadCallback read_callback_;
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_CONNECTION_H_