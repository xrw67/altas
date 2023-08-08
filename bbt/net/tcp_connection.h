#ifndef BBT_NET_TCP_CONNCECTION_H_
#define BBT_NET_TCP_CONNCECTION_H_

#include <mutex>

#include "asio.hpp"
#include "bbt/net/callbacks.h"
#include "bbt/net/buffer.h"

namespace bbt {
namespace net {


class Connection : public std::enable_shared_from_this<Connection> {
 public:
  struct Context {
   protected:
    virtual ~Context() {}
    Connection* connection() { return conn_; }

   private:
    friend class Connection;
    void set_connection(Connection* conn) { conn_ = conn; }

    Connection* conn_;
  };

  enum State { kDisconnected, kConnected };

  Connection(asio::ip::tcp::socket socket);

  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

  ~Connection();

  /// Start the first asynchronous operation for the connection.
  void Start();

  /// Stop all asynchronous operations associated with the connection.
  void Stop();

  void Write(const char* data, size_t len);

  void DoRead();
  void DoWrite();

  State state() const noexcept { return state_; }
  Context* context() { return context_; }

  /// @brief ctx的owner将变成Connection
  /// @param ctx
  void set_context(Context* ctx) {
    ctx->set_connection(this);
    delete context_;
    context_ = ctx;
  }

  void set_connection_callback(const ConnectionCallback& cb) {
    connection_callback_ = cb;
  }

  void set_message_callback(const MessageCallback& cb) {
    message_callback_ = cb;
  }

  State state_;
  /// Socket for the connection.
  asio::ip::tcp::socket socket_;
  ConnectionCallback connection_callback_;
  MessageCallback message_callback_;

  Context* context_;

  // Input
  std::array<char, 8192> buffer_;
  Buffer input_buffer_;

  // oputput
  std::mutex mutex_;
  Buffer output_buffer_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_NET_TCP_CONNCECTION_H_