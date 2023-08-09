#include "gmock/gmock.h"

#include "bbt/bus/connection.h"

namespace bbt {
namespace bus {
namespace {

TEST(BusConnectionTest, hello) {
  asio::io_context io_context;

  BusConnection client("client", io_context), server("server", io_context);
}

}  // namespace
}  // namespace bus
}  // namespace bbt