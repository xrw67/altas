#ifndef BBT_BUS_CALLBACKS_H_
#define BBT_BUS_CALLBACKS_H_

#include <memory>
#include <functional>

namespace bbt {

namespace bus {


using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

static const uint32_t kMsgMagic = 0x20141021;

typedef uint32_t MsgId;

struct MsgHeader {
  uint32_t magic;
  uint32_t length;
};

class Msg;
typedef std::shared_ptr<Msg> MsgPtr;
typedef std::function<void(const MsgPtr&)> MsgCallback;

class BusConnection;
typedef std::shared_ptr<BusConnection> ConnectionPtr;
typedef std::function<void(const ConnectionPtr&)> ConnectionCloseCallback;

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_CALLBACKS_H_