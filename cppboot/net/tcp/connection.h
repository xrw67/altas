#ifndef CPPBOOT_NET_TCP_CONNCECTION_H_
#define CPPBOOT_NET_TCP_CONNCECTION_H_

#include <mutex>
#include <array>

#include "cppboot/net/callbacks.h"
#include "cppboot/net/buffer.h"
#include "cppboot/net/connection.h"

namespace cppboot {
namespace net {

class TcpConn : public Conn {
 public:
  TcpConn(asio::ip::tcp::socket socket);

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

  // Input
  std::array<char, 8192> buffer_;
  Buffer input_buffer_;

  // Output
  std::mutex mutex_;
  Buffer output_buffer_;
};

}  // namespace net
}  // namespace cppboot

#endif  // CPPBOOT_NET_TCP_CONNCECTION_H_