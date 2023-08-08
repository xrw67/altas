#include "bbt/bus/context.h"
#include "bbt/net/buffer.h"
#include "bbt/bus/msg.h"
#include "bbt/bus/msg_packer.h"

namespace bbt {
namespace bus {

BusContext::BusContext(const BusMsgCallback& cb)
    : bus_msg_callback_(cb), state_(kHeader) {}

void BusContext::Write(const MsgPtr& msg) {
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

  connection()->Write(tmp.Peek(), tmp.ReadableBytes());
}

// TODO: check msg magic
bool BusContext::Parse(const ConnectionPtr& conn, Buffer* buf) {
  if (state_ == kHeader) {
    if (buf->ReadableBytes() < sizeof(msg_header_)) {
      return true;  // continue read
    }

    memcpy(&msg_header_, buf->BeginWrite(), sizeof(msg_header_));
    buf->Retrive(sizeof(msg_header_));
    state_ = kBody;
  }

  if (state_ == kBody) {
    if (buf->ReadableBytes() < msg_header_.length) {
      return true;  // continue read
    }

    // TODO: 性能优化
    std::string msg_body(buf->Peek(), msg_header_.length);
    buf->Retrive(msg_header_.length);
    state_ = kHeader;

    JsonPacker p;
    MsgPtr in(new Msg());

    auto st = p.Unpack(msg_body, in.get());
    if (st) {
      return false;
    }
    bus_msg_callback_(conn, in);
  }
  return true;
}

}  // namespace bus
}  // namespace bbt