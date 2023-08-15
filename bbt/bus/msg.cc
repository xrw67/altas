#include "bbt/bus/msg.h"

#include "bbt/net/buffer.h"
#include "bbt/net/connection.h"

#include "bbt/bus/msg_packer.h"
namespace bbt {
namespace bus {

using bbt::net::Buffer;


void SendMessageToConnection(const ConnPtr& conn, const MsgPtr& msg) {
  bbt::net::Buffer buffer;

  JsonPacker jp;

  std::string body;  // TODO 性能优化
  jp.Pack(*msg, &body);

  MsgHeader hdr = {
      .magic = kMsgMagic,
      .length = (uint32_t)body.length() + 1,
  };

  Buffer tmp(sizeof(hdr) + body.length() + 1);
  tmp.Append((const char*)&hdr, sizeof(hdr));
  tmp.Append(body.data(), body.length() + 1);

  conn->Send(tmp.Peek(), tmp.ReadableBytes());
}

}  // namespace bus
}  // namespace bbt