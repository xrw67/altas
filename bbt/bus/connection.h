#ifndef BBT_BUS_CONNCECTION_H_
#define BBT_BUS_CONNCECTION_H_

#include "asio.hpp"
#include "callbacks.h"

namespace bbt {
namespace bus {

class BaseConnection {
 public:
  BaseConnection(asio::ip::tcp::socket socket);

  BaseConnection(const BaseConnection&) = delete;
  BaseConnection& operator=(const BaseConnection&) = delete;


  void set_on_close_handler(const ConnectionCloseCallback& cb) {
    on_close_callback_ = cb;
  }

  /// Socket for the connection.
  asio::ip::tcp::socket socket_;
  ConnectionCloseCallback on_close_callback_;
};

class MsgParser {
 public:
 enum State { };

  MsgHeader msg_header_;
  std::string msg_body_;
};

class BusConnection : public BaseConnection,
                   public std::enable_shared_from_this<BusConnection> {
 public:
  /// Construct a connection with the given socket.
  explicit BusConnection(asio::ip::tcp::socket socket, const MsgCallback& cb);

  /// Start the first asynchronous operation for the connection.
  void Start();

  /// Stop all asynchronous operations associated with the connection.
  void Stop();

  void Send(const MsgPtr& msg) {}

 private:
  /// Perform an asynchronous read operation.
  void DoRead();
  /// Perform an asynchronous write operation.
  void DoWrite();
  void DoWriteMsgBody(const asio::error_code& error);
  void DoWriteComplete(const asio::error_code& error);

  void OnReadMsgHeader(const asio::error_code& error);
  void OnReadMsgBody(const asio::error_code& error);
  void DoClose();

  MsgCallback msg_callback_;

  // /// 每个连接一个自己的消息队列
  // /// - TODO: 新来的消息入队列，
  // /// - TODO: 处理完的消息出队列发送给客户端，
  // /// - TODO: 连接关闭后返回所有的消息服务已经不可用，
  // /// - TODO: 每个服务有一个超时时间
  // MsgQueue msgs_;

  //
  // Cache
  //

  MsgParser parser_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CONNCECTION_H_