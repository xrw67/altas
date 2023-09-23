#include "addr.h"

#include <netinet/in.h>
#include <sys/_types/_sa_family_t.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <arpa/inet.h>

namespace bbt {
namespace net2 {

Addr::Addr(const std::string& network, const std::string& addr)
    : net_(network), addr_(addr) {
  if (network == "tcp" || network == "udp") {
    ParseIpAddr(addr);
  } else if (network == "unix") {
    ParseUnixAddr(addr);
  } else {
    net_.clear();
    addr_.clear();
    data_.clear();
  }
}

Addr::~Addr() {}

void Addr::ParseIpAddr(const std::string& addr_port) {
  std::string addr;
  unsigned short port = 0;

  if (addr_port[0] == '[') {
    auto colon = addr_port.find("]:");
    port = (colon == addr_port.npos)
               ? 0
               : atoi(addr_port.substr(colon + 2).c_str());
    if (port == 0) return;

    addr = addr_port.substr(1, colon - 1);
    ParseIpv6Addr(addr, port);
  } else {
    auto colon = addr_port.find(":");
    port = (colon == addr_port.npos)
               ? 0
               : atoi(addr_port.substr(colon + 1).c_str());
    if (port == 0) return;

    addr = addr_port.substr(0, colon);
    ParseIpv4Addr(addr, port);
  }
}

void Addr::ParseIpv4Addr(const std::string& addr, unsigned short port) {
  data_.resize(sizeof(struct sockaddr_in), 0);
  struct sockaddr_in* sin = reinterpret_cast<struct sockaddr_in*>(data_.data());

  sin->sin_len = sizeof(struct sockaddr_in);
  sin->sin_family = AF_INET;
  sin->sin_port = htons(port);

  if (addr.empty()) return;

  if (inet_pton(sin->sin_family, addr.c_str(), &sin->sin_addr) != 1) {
    data_.clear();
    return;
  }
}

void Addr::ParseIpv6Addr(const std::string& addr, unsigned short port) {
  data_.resize(sizeof(struct sockaddr_in6), 0);
  struct sockaddr_in6* sin6 =
      reinterpret_cast<struct sockaddr_in6*>(data_.data());

  sin6->sin6_len = sizeof(struct sockaddr_in6);
  sin6->sin6_family = AF_INET6;
  sin6->sin6_port = htons(port);

  if (addr.empty()) return;

  if (inet_pton(sin6->sin6_family, addr.c_str(), &sin6->sin6_addr) != 1) {
    data_.clear();
    return;
  }
}

void Addr::ParseUnixAddr(const std::string& addr) {
  if (addr.empty()) return;

  unsigned char sun_len =
      offsetof(struct sockaddr_un, sun_path) + addr.length() + 1;

  data_.resize(sun_len, 0);
  struct sockaddr_un* sa = reinterpret_cast<struct sockaddr_un*>(data_.data());
  sa->sun_len = sun_len;
  sa->sun_family = AF_UNIX;
  std::copy(addr.begin(), addr.end(), sa->sun_path);
}

}  // namespace net2

}  // namespace bbt