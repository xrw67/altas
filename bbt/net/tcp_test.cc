#include "gmock/gmock.h"

#include "asio.hpp"
#include "tcp_client.h"
#include "tcp_server.h"

namespace bbt {
namespace net {
namespace {

TEST(Tcp, echo_server) {
  asio::io_context io_context;
  MyTcpServer svr(io_context);

  std::string svr_readed;
  svr.set_conn_callback([](const BaseConnectionPtr& conn) {});
  svr.set_read_callback([&](const BaseConnectionPtr& conn, Buffer* buf) {
    svr_readed = buf->ToString();
  });
}

}  // namespace
}  // namespace net
}  // namespace bbt