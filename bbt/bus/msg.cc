#include "bbt/bus/msg.h"

#include "bbt/net/buffer.h"
#include "bbt/net/connection.h"

#include "bbt/bus/msg_packer.h"
namespace bbt {
namespace bus {

using bbt::net::Buffer;

bool Msg::has_param(const std::string& key) const {
  return !key.empty() && values_.find(key) != values_.end();
}

void Msg::set_param(const std::string& key, const std::string& value) {
  if (key.empty()) return;
  values_[key] = value;
}

std::string Msg::param(const std::string& key) const {
  if (key.empty()) return "";
  auto it = values_.find(key);
  return (it != values_.end()) ? it->second : "";
}

void SendMessageToConnection(const ConnectionPtr& conn, const MsgPtr& msg) {
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