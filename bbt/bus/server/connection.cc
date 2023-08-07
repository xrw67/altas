#include "bbt/bus/server/connection.h"

#include <algorithm>
#include <functional>

#include "bbt/bus/msg_packer.h"
#include "bbt/bus/server/connection_manager.h"

namespace bbt {

namespace bus {

Connection::Connection(asio::ip::tcp::socket socket, ConnectionManager& manager,
                       const MsgHandler& handler)
    : socket_(std::move(socket)),
      connection_manager_(manager),
      msg_handler_(handler) {}

void Connection::Start() { DoRead(); }

void Connection::Stop() { socket_.close(); }

void Connection::DoRead() {
  asio::async_read(
      socket_, asio::buffer(&msg_header_, sizeof(msg_header_)),
      std::bind(&Connection::HandleReadMsgHeader, this, std::placeholders::_1));
}

void Connection::HandleReadMsgHeader(const asio::error_code& error) {
  if (error) {
    DoClose();
    return;
  }

  if (msg_header_.magic != kMsgMagic) {
    DoClose();
    return;
  }

  msg_body_.resize(msg_header_.length);

  asio::async_read(
      socket_, asio::buffer((char*)msg_body_.data(), msg_header_.length),
      std::bind(&Connection::HandleReadMsgBody, this, std::placeholders::_1));
}

void Connection::HandleReadMsgBody(const asio::error_code& error) {
  if (error) {
    DoClose();
    return;
  }

  JsonPacker p;
  Msg in;

  auto st = p.Unpack(msg_body_, &in);
  if (st) {
    DoClose();
    return;
  }

  Msg out(in.id());
  msg_handler_(in, &out);

  p.Pack(out, &msg_body_);
  msg_header_.length = msg_body_.length() + 1;

  DoWrite();
}

void Connection::DoWrite() {
  auto self(shared_from_this());
  asio::async_write(
      socket_, asio::buffer(&msg_header_, sizeof(msg_header_)),
      std::bind(&Connection::DoWriteMsgBody, this, std::placeholders::_1));
}

void Connection::DoWriteMsgBody(const asio::error_code& error) {
  asio::async_write(
      socket_, asio::buffer(msg_body_.data(), msg_body_.length() + 1),
      std::bind(&Connection::DoWriteComplete, this, std::placeholders::_1));
}

void Connection::DoWriteComplete(const asio::error_code& error) {
  if (error) {
    DoClose();
    return;
  }

  DoRead();
}

void Connection::DoClose() { connection_manager_.Stop(shared_from_this()); }

}  // namespace bus
}  // namespace bbt