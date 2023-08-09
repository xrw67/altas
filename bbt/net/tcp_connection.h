#ifndef BBT_NET_TCP_CONNCECTION_H_
#define BBT_NET_TCP_CONNCECTION_H_

#include <mutex>

#include "asio.hpp"
#include "bbt/net/callbacks.h"
#include "bbt/net/buffer.h"

namespace bbt {
namespace net {


class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  struct Context {
   protected:
    virtual ~Context() {}
    TcpConnection* connection() { return conn_; }

   private:
    friend class TcpConnection;
    void set_connection(TcpConnection* conn) { conn_ = conn; }

    TcpConnection* conn_;
  };

  enum State { kDisconnected, kConnected };

  TcpConnection(asio::ip::tcp::socket socket);

  TcpConnection(const TcpConnection&) = delete;
  TcpConnection& operator=(const TcpConnection&) = delete;

  ~TcpConnection();

  /// Start the first asynchronous operation for the connection.
  void Start();

  /// Stop all asynchronous operations associated with the connection.
  void Stop();

  void Send(const void* message, int len);

  void DoRead();
  void DoWrite();

  State state() const noexcept { return state_; }
  Context* context() { return context_; }

  /// @brief ctx的owner将变成Connection
  /// @param ctx
  void set_connection_context(Context* ctx) {
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