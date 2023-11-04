#ifndef CPPBOOT_BUS_MSG_PACKER_H_
#define CPPBOOT_BUS_MSG_PACKER_H_

#include <string>
#include "cppboot/base/status.h"

namespace cppboot {
namespace bus {

class Msg;

/// @brief Msg序列化和反序列化
class MsgPacker {
 public:
  /// @brief 序列化消息
  /// @param msg 消息对象
  /// @param data 序列化后的结果
  virtual void Pack(const Msg& msg, std::string* data) = 0;

  /// @brief 反序列化
  /// @param data 数据源
  /// @param msg 反序列化得到的消息对象
  /// @return 状态吗
  virtual Status Unpack(const std::string& data, Msg* msg) = 0;

 protected:
  virtual ~MsgPacker() {}
};

class JsonPacker : public MsgPacker {
 public:
  void Pack(const Msg& msg, std::string* result);
  Status Unpack(const std::string& data, Msg* msg);
};

}  // namespace bus
}  // namespace cppboot

#endif  // CPPBOOT_BUS_MSG_PACKER_H_