#include "gmock/gmock.h"

#include "bbt/net/buffer.h"
#include "bbt/net/net_proto.h"

namespace bbt {
namespace net {
namespace {

class MockConnection : public Connection {
 public:
  Status Write(const char* data, size_t size) {
    peer_->OnRead(data, size);
    return OkStatus();
  }

  void set_peer(MockConnection* peer) { peer_ = peer; }
  Buffer& input_buffer() { return input_buffer_; }

 private:
  void OnRead(const char* data, size_t size) {
    input_buffer_.Append(data, size);
  }

  MockConnection* peer_;
  Buffer input_buffer_;
};

TEST(NetProto, example) {
  MockConnection c1, c2;
  c1.set_peer(&c2);
  c2.set_peer(&c1);

  c1.Write("Hello", 5);
  ASSERT_EQ(c2.input_buffer().Str(), "Hello");

  c2.Write("World", 5);
  ASSERT_EQ(c1.input_buffer().Str(), "World");
}

}  // namespace
}  // namespace net

}  // namespace bbt