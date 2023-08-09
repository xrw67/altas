#ifndef BBT_BUS_METHOD_MUX_H_
#define BBT_BUS_METHOD_MUX_H_

#include <map>
#include <string>

#include "bbt/bus/method.h"

namespace bbt {
namespace bus {

class MethodMux {
 public:

  MsgPtr ServeMethod (const MsgPtr& in);

  std::map<std::string, MethodFunc> methods_;
};

}  // namespace bus
}  // namespace bbt

#endif  // BBT_BUS_METHOD_MUX_H_
