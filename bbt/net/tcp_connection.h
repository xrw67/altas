#ifndef BBT_NET_TCP_CONNCECTION_H_
#define BBT_NET_TCP_CONNCECTION_H_

#include <mutex>
#include <array>

#include "asio.hpp"
#include "bbt/net/callbacks.h"
#include "bbt/net/buffer.h"

#include "base_connection.h"

namespace bbt {
namespace net {

class MyTcpConnection : public BaseConnection {
 public:
  MyTcpConnection(asio::ip::tcp::socket socket);

  /// Start the first asynchronous operation for the connection.
  void Start();

  /// Stop all asynchronous operations associated with the connection.
  void Stop();

  void Send(const void* data, size_t len);

  std::string GetLocalAddress() const noexcept;
  std::string GetRemoteAddress() const noexcept;

 private:
  void ReadFromSocket();
  void WriteToSocket();

  asio::ip::tcp::socket socket_;
  Buffer input_buffer_, output_buffer_;

  std::array<char, 8192> buffer_;
  std::mutex mutex_;
};

typedef std::shared_ptr<MyTcpConnection> MyTcpConnectionPtr;

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CONNCECTION_H_