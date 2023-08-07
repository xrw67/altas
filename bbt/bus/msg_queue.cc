#include "bbt/bus/msg_queue.h"

namespace bbt {
namespace bus {

Status MsgQueue::Push(MsgPtr& msg) noexcept {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = msgs_.find(msg->id());
  if (it != msgs_.end()) {
    return AlreadyExistsError(std::to_string(msg->id()));
  } else {
    msgs_[msg->id()] = msg;
  }
  return OkStatus();
}

MsgPtr MsgQueue::Pop(Msg::Id id) noexcept {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = msgs_.find(id);
  if (it != msgs_.end()) {
    MsgPtr result = it->second;
    msgs_.erase(it);
    return result;
  } else {
    return nullptr;
  }
}

}  // namespace bus
}  // namespace bbt