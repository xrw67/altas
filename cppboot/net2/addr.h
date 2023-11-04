#ifndef CPPBOOT_NET2_ADDR_H_
#define CPPBOOT_NET2_ADDR_H_

#include <vector>
#include <string>

struct sockaddr;

namespace cppboot {
namespace net2 {

struct Addr {
 public:
  /**
   * @brief Construct a new Addr object
   *        "xyz.org:http"
   *        "192.0.2.1:http"
   *        "198.51.100.1:80"
   *        "[2001:db8::1]:80"
   *        ":80"
   * @param network
   * @param addr
   */
  Addr(const std::string& network, const std::string& addr);
  ~Addr();

  const struct sockaddr* addr() const {
    if (data_.empty())
      return nullptr;
    else
      return reinterpret_cast<const struct sockaddr*>(data_.data());
  };

  std::string network() const { return net_; }
  int len() const { return data_.size(); }

  bool IsValid() const { return len() != 0; }

 private:
  void ParseIpAddr(const std::string& addr_port);
  void ParseIpv4Addr(const std::string& addr, unsigned short port);
  void ParseIpv6Addr(const std::string& addr, unsigned short port);
  void ParseUnixAddr(const std::string& addr);

  std::string net_;
  std::string addr_;
  std::vector<char> data_;
};

}  // namespace net2
}  // namespace cppboot

#endif  // CPPBOOT_NET2_ADDR_H_
