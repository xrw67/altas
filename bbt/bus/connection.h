#ifndef BBT_BUS_CONNCECTION_H_
#define BBT_BUS_CONNCECTION_H_

#include <memory>
#include <functional>

#include "asio.hpp"

#include "bbt/bus/msg_queue.h"

namespace bbt {

namespace bus {

class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;

typedef std::function<void(ConnectionPtr)> OnCloseHandler;

class Connection : public std::enable_shared_from_this<Connection> {
 public:
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

  /// Construct a connection with the given socket.
  explicit Connection(asio::ip::tcp::socket socket, const MsgHandler& handler);

  /// Start the first asynchronous operation for the connection.
  void Start();

  /// Stop all asynchronous operations associated with the connection.
  void Stop();

  void set_on_close_handler(const OnCloseHandler& handler) {
    on_close_handler_ = handler;
  }

 private:
  /// Perform an asynchronous read operation.
  void DoRead();
  /// Perform an asynchronous write operation.
  void DoWrite();
  void DoWriteMsgBody(const asio::error_code& error);
  void DoWriteComplete(const asio::error_code& error);

  void HandleReadMsgHeader(const asio::error_code& error);
  void HandleReadMsgBody(const asio::error_code& error);
  void DoClose();

  /// Socket for the connection.
  asio::ip::tcp::socket socket_;

  MsgHandler msg_handler_;

  OnCloseHandler on_close_handler_;

  // /// 每个连接一个自己的消息队列
  // /// - TODO: 新来的消息入队列，
  // /// - TODO: 处理完的消息出队列发送给客户端，
  // /// - TODO: 连接关闭后返回所有的消息服务已经不可用，
  // /// - TODO: 每个服务有一个超时时间
  // MsgQueue msgs_;

  //
  // Cache
  //

  MsgHeader msg_header_;
  std::string msg_body_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONNCECTION_H_