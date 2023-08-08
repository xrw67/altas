#ifndef BBT_BUS_CONTEXT_H_
#define BBT_BUS_CONTEXT_H_

#include "asio.hpp"
#include "bbt/net/tcp_connection.h"

#include "msg.h"

namespace bbt {
namespace bus {

using bbt::net::Buffer;
using bbt::net::ConnectionPtr;

class BusContext : public bbt::net::Connection::Context {
 public:
  /// Construct a connection with the given socket.
  explicit BusContext(const BusMsgCallback& cb);

  void Write(const MsgPtr& msg);

  bool Parse(const ConnectionPtr& conn, Buffer* buf);

 private:
  /// Perform an asynchronous read operation.

  /// Perform an asynchronous write operation.

  // /// 每个连接一个自己的消息队列
  // /// - TODO: 新来的消息入队列，
  // /// - TODO: 处理完的消息出队列发送给客户端，
  // /// - TODO: 连接关闭后返回所有的消息服务已经不可用，
  // /// - TODO: 每个服务有一个超时时间
  // MsgQueue msgs_;

  BusMsgCallback bus_msg_callback_;

  enum State { kHeader, kBody };

  State state_;

  MsgHeader msg_header_;
  std::string msg_body_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONTEXT_H_
