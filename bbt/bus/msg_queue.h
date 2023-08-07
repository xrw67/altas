#ifndef BBT_BUS_MSG_QUEUE_H_
#define BBT_BUS_MSG_QUEUE_H_

#include <mutex>
#include <unordered_map>
#include "bbt/bus/msg.h"
#include "bbt/base/status.h"

namespace bbt {
namespace bus {

class MsgQueue {
 public:
  Status Push(MsgPtr& msg) noexcept;
  MsgPtr Pop(Msg::Id id) noexcept;
  size_t Size() const noexcept { return msgs_.size(); }

 private:
  std::mutex mutex_;
  std::unordered_map<Msg::Id, MsgPtr> msgs_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_MSG_QUEUE_H_