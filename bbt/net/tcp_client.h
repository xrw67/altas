#ifndef BBT_NET_TCP_CLIENT_H_
#define BBT_NET_TCP_CLIENT_H_

#include <string>
#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/net/callbacks.h"

namespace bbt {
namespace net {

class TcpClient {
 public:
  TcpClient(asio::io_context& ioctx);

  Status Connect(const std::string& address, const std::string& port);
  asio::io_context& io_context_;
  asio::ip::tcp::socket socket_;  // Temp
};

}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_TCP_CLIENT_H_