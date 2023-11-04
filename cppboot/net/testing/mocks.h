#ifndef CPPBOOT_NET_TESTING_MOCKS_H_
#define CPPBOOT_NET_TESTING_MOCKS_H_

#include <memory>

#include "cppboot/net/buffer.h"
#include "cppboot/net/connection.h"

namespace cppboot {
namespace net {
namespace testing {

struct MockConnectionPair : public Conn {
  std::weak_ptr<MockConnectionPair> peer_conn;

  void connect(const std::shared_ptr<MockConnectionPair>& other) {
    peer_conn = other;
    state_ = kConnected;
  }

  virtual void Send(const void* data, size_t len) {
    // 直接发给对端
    auto l = peer_conn.lock();
    if (l) l->OnReceive(data, len);
  }

  void OnReceive(const void* data, size_t len) {
    // 收到对端的数据，抛给上层
    Buffer buf;
    buf.Append((const char*)data, len);
    receive_callback_(shared_from_this(), &buf);
  }
};

typedef std::shared_ptr<MockConnectionPair> MockConnectionPairPtr;

}  // namespace testing
}  // namespace net
}  // namespace cppboot

#endif  // CPPBOOT_NET_TESTING_MOCKS_H_
