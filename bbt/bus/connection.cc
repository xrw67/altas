#include "bbt/bus/connection.h"

#include <algorithm>
#include <functional>

#include "bbt/bus/msg.h"
#include "bbt/bus/msg_packer.h"

namespace bbt {

namespace bus {

BaseConnection::BaseConnection(asio::ip::tcp::socket socket)
    : socket_(std::move(socket)) {}

//
// BusConnection
//
BusConnection::BusConnection(asio::ip::tcp::socket socket, const MsgCallback& cb)
    : BaseConnection(std::move(socket)), msg_callback_(cb) {}

void BusConnection::Start() { DoRead(); }

void BusConnection::Stop() { socket_.close(); }

void BusConnection::DoRead() {
  asio::async_read(
      socket_, asio::buffer(&parser_.msg_header_, sizeof(parser_.msg_header_)),
      std::bind(&BusConnection::OnReadMsgHeader, this, std::placeholders::_1));
}

void BusConnection::OnReadMsgHeader(const asio::error_code& error) {
  if (error) {
    DoClose();
    return;
  }

  if (parser_.msg_header_.magic != kMsgMagic) {
    DoClose();
    return;
  }

  parser_.msg_body_.resize(parser_.msg_header_.length);

  asio::async_read(
      socket_,
      asio::buffer((char*)parser_.msg_body_.data(), parser_.msg_header_.length),
      std::bind(&BusConnection::OnReadMsgBody, this, std::placeholders::_1));
}

void BusConnection::OnReadMsgBody(const asio::error_code& error) {
  if (error) {
    DoClose();
    return;
  }

  JsonPacker p;
  MsgPtr in(new Msg());

  auto st = p.Unpack(parser_.msg_body_, in.get());
  if (st) {
    DoClose();
    return;
  }

  msg_callback_(in);
  DoRead();  // Next msg
}

void BusConnection::DoWrite() {
  auto self(shared_from_this());
  asio::async_write(
      socket_, asio::buffer(&parser_.msg_header_, sizeof(parser_.msg_header_)),
      std::bind(&BusConnection::DoWriteMsgBody, this, std::placeholders::_1));
}

void BusConnection::DoWriteMsgBody(const asio::error_code& error) {
  asio::async_write(
      socket_,
      asio::buffer(parser_.msg_body_.data(), parser_.msg_body_.length() + 1),
      std::bind(&BusConnection::DoWriteComplete, this, std::placeholders::_1));
}

void BusConnection::DoWriteComplete(const asio::error_code& error) {
  if (error) {
    DoClose();
    return;
  }

  DoRead();
}

void BusConnection::DoClose() { on_close_callback_(shared_from_this()); }

}  // namespace bus
}  // namespace bbt