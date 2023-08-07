#ifndef BBT_BUS_CLIENT_H_
#define BBT_BUS_CLIENT_H_

#include <string>
#include <map>
#include <atomic>

#include "asio.hpp"

#include "bbt/base/status.h"
#include "bbt/bus/method.h"
#include "bbt/bus/msg.h"

#include "bbt/bus/connection.h"

namespace bbt {
namespace bus {

class Client {
 public:
  Client(const std::string& name, asio::io_context& io_context);
  ~Client();

  Status Connect(const std::string& address, const std::string& port);
  void Close();

  Status RegisterMethod(const std::string& name, MethodFunc func);

  Status Call(const std::string& method, const In& in, Out* out);
  Status ACall(const std::string& method, const In& in, Result* result);

 private:
  Msg::Id NextMsgId() noexcept;

  /// Perform an asynchronous read operation.
  void DoRead();
  /// Perform an asynchronous write operation.
  void DoWrite();

  /// Buffer for incoming data.
  std::array<char, 8192> buffer_;

  std::string name_;
  asio::io_context& io_context_;
  asio::ip::tcp::socket socket_;

  std::map<std::string, MethodFunc> services_;

  std::atomic<Msg::Id> next_id_;

  /// 等待结果的列表
  std::map<Msg::Id, Result> waitings_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CLIENT_H_