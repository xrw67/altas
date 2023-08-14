#include "gmock/gmock.h"

#include "bbt/bus/router.h"
#include "bbt/net/connection.h"

namespace {

using bbt::net::Connection;
using bbt::net::ConnectionPtr;

struct MockConnection : public Connection {
  void Send(const void* data, size_t len) {}
};

TEST(BusRouterTest, example) {
  bbt::bus::BusRouter r;

  auto conn1 = std::make_shared<MockConnection>();
  auto conn2 = std::make_shared<MockConnection>();

  r.Add("xrw", conn1);
  r.Add("bob", conn2);

  ASSERT_EQ(r.Find("xrw"), conn1);
  ASSERT_EQ(r.Find("bob"), conn2);
  ASSERT_EQ(r.Find("bill"), nullptr);
}

}  // namespace