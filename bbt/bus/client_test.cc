#include "gmock/gmock.h"

#include <vector>

#include "bbt/bus/method.h"
#include "bbt/bus/client.h"
#include "bbt/bus/msg_packer.h"

#include "bbt/net/testing/mocks.h"

namespace bbt {
namespace bus {
namespace {
using bbt::net::Conn;
using bbt::net::testing::MockConnectionPair;

struct MockConnection : public Conn {
  std::vector<bbt::bus::MsgPtr> output_msgs;  // 发送出去的msg
  bbt::Status last_status;

  void Send(const void* data, size_t len) {
    // 传输层收到了数据，发到远程服务器上。。。
    JsonPacker jp;
    MsgPtr req(new Msg);
    last_status = jp.Unpack(std::string((const char*)data + sizeof(MsgHeader)),
                            req.get());
    if (last_status) {
      output_msgs.push_back(req);

      // 。。。模拟远程服务器的Reply
      Msg resp;
      resp.set_id(req->id());
      resp.set_request(false);
      resp.set_caller(req->caller());
      resp.set_method(req->method());
      resp.set_param("return", "ok");

      std::string tmp;
      jp.Pack(resp, &tmp);
      Buffer buf;

      MsgHeader hdr = {kMsgMagic, (uint32_t)tmp.length()};
      buf.Append((const char*)&hdr, sizeof(hdr));
      buf.Append(tmp.c_str(), tmp.length());
      receive_callback_(shared_from_this(), &buf);
    }
  }
};

TEST(BusClient, should_call_and_reply) {
  auto mock_conn = std::make_shared<MockConnection>();
  Msg resp;

  bbt::bus::BusClient client("name1", mock_conn);

  bbt::bus::In in;
  in.set("key1", "str1");
  in.set("key2", "1001");

  bbt::bus::Out out;
  auto st = client.Call("MyEcho.Echo", in, &out);
  ASSERT_TRUE(st) << st.ToString();
  ASSERT_EQ(out.get("return"), "ok");

  client.Stop();
}

TEST(BusClient, should_reply_when_call_by_other_client) {
  auto conn1 = std::make_shared<MockConnectionPair>();
  auto conn2 = std::make_shared<MockConnectionPair>();

  conn1->connect(conn2);
  conn2->connect(conn1);

  bbt::bus::BusClient client1("name1", conn1);
  bbt::bus::BusClient client2("name1", conn2);

  client1.AddMethod("func1", [](const In& in, Out* out) {
    out->set("name", in.get("name"));
    out->set("key2", "666");
  });

  bbt::bus::In in;
  in.set("name", "xrw");

  bbt::bus::Out out;
  ASSERT_TRUE(client2.Call("func1", in, &out));
  ASSERT_EQ(out.get("name"), "xrw");
  ASSERT_EQ(out.get("key2"), "666");
}

}  // namespace
}  // namespace bus
}  // namespace bbt
