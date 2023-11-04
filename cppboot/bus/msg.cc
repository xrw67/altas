#include "cppboot/bus/msg.h"

#include "cppboot/net/buffer.h"
#include "cppboot/net/connection.h"

#include "cppboot/bus/msg_packer.h"
namespace cppboot {
namespace bus {

using cppboot::net::Buffer;


void SendMessageToConnection(const ConnPtr& conn, const MsgPtr& msg) {
  cppboot::net::Buffer buffer;

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
}  // namespace cppboot