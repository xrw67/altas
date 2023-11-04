#include "gmock/gmock.h"

#include "cppboot/bus/router.h"
#include "cppboot/net/connection.h"

namespace {

using cppboot::net::Conn;
using cppboot::net::ConnPtr;

struct MockConnection : public Conn {
  void Send(const void* data, size_t len) {}
};

TEST(BusRouterTest, example) {
  cppboot::bus::BusRouter r;

  auto conn1 = std::make_shared<MockConnection>();
  auto conn2 = std::make_shared<MockConnection>();

  r.Add("xrw", conn1);
  r.Add("bob", conn2);

  ASSERT_EQ(r.Find("xrw"), conn1);
  ASSERT_EQ(r.Find("bob"), conn2);
  ASSERT_EQ(r.Find("bill"), nullptr);
}

}  // namespace