#ifndef BBT_NET_CALLBACKS_H_
#define BBT_NET_CALLBACKS_H_

#include <memory>
#include <functional>

// TODO: remove it
#include "bbt/net/buffer.h"

namespace bbt {
namespace net {

class Buffer;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


}  // namespace net
}  // namespace bbt

#endif  // BBT_NET_CALLBACKS_H_