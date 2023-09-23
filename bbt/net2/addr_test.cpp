#include "gmock/gmock.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include "addr.h"

namespace {

using bbt::net2::Addr;

TEST(Addr, should_network_is_supported) {
  ASSERT_EQ("tcp", Addr("tcp", "").network());
  ASSERT_EQ("udp", Addr("udp", "").network());
  ASSERT_EQ("unix", Addr("unix", "").network());
  ASSERT_EQ("", Addr("xxx", "").network());
}

// UNIX
TEST(Addr, should_parse_unix_path) {
  std::string path = "/tmp/test.sock";
  auto addr = Addr("unix", path);

  const struct sockaddr_un* sun =
      reinterpret_cast<const struct sockaddr_un*>(addr.addr());

  ASSERT_EQ(AF_UNIX, sun->sun_family);
  ASSERT_EQ(path, sun->sun_path);
  ASSERT_EQ(offsetof(struct sockaddr_un, sun_path) + path.length() + 1,
            sun->sun_len);
  ASSERT_EQ(sun->sun_len, addr.len());
}

// IPV4
TEST(Addr, should_parse_ipv4_with_port) {
  auto addr = Addr("tcp", "1.2.3.4:9999");
  const struct sockaddr_in* sin =
      reinterpret_cast<const struct sockaddr_in*>(addr.addr());
  ASSERT_EQ(AF_INET, sin->sin_family);
  ASSERT_EQ(sizeof(struct sockaddr_in), sin->sin_len);
  ASSERT_EQ(9999, ntohs(sin->sin_port));
  ASSERT_EQ(0x04030201, sin->sin_addr.s_addr);
}

TEST(Addr, should_fail_ipv4_without_port) {
  auto addr = Addr("tcp", "1.2.3.4");
  ASSERT_EQ(nullptr, addr.addr());
}

TEST(Addr, should_parse_ipv4_only_port) {
  auto addr = Addr("tcp", ":9999");
  const struct sockaddr_in* sin =
      reinterpret_cast<const struct sockaddr_in*>(addr.addr());
  ASSERT_EQ(AF_INET, sin->sin_family);
  ASSERT_EQ(sizeof(struct sockaddr_in), sin->sin_len);
  ASSERT_EQ(9999, ntohs(sin->sin_port));
  ASSERT_EQ(0, sin->sin_addr.s_addr);
}

// IPV6

TEST(Addr, should_parse_ipv6_with_port) {
  auto addr = Addr("tcp", "[2001::1]:9999");
  const struct sockaddr_in6* sin6 =
      reinterpret_cast<const struct sockaddr_in6*>(addr.addr());
  ASSERT_EQ(AF_INET6, sin6->sin6_family);
  ASSERT_EQ(sizeof(struct sockaddr_in6), sin6->sin6_len);
  ASSERT_EQ(9999, ntohs(sin6->sin6_port));
  ASSERT_EQ(0x20, sin6->sin6_addr.s6_addr[0]);
  ASSERT_EQ(0x1, sin6->sin6_addr.s6_addr[1]);
  ASSERT_EQ(0x1, sin6->sin6_addr.s6_addr[15]);
}

TEST(Addr, should_fail_ipv6_without_port) {
  auto addr = Addr("tcp", "2001::1");
  ASSERT_EQ(nullptr, addr.addr());
}

}  // namespace
