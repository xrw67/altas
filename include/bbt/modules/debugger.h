#ifndef BBT_MODULES_DEBUGGER_H_
#define BBT_MODULES_DEBUGGER_H_

#include "bbt/util/string_view.h"
#include "bbt/util/status.h"

#include "bbt/appkit/object.h"

namespace bbt {

class Debugger;

class DebugIo {
 public:
  virtual ~DebugIo() {}
  virtual void set_debugger(Debugger* debugger) = 0;
  virtual void Write(const std::string& content) = 0;
};

class DebugHandler {
 public:
  virtual Status OnDebugRequest(DebugIo* io, const std::string& cmd,
                                const std::string& args) = 0;

 protected:
  virtual ~DebugHandler() {}
};

class Debugger : public Object {
 public:
  static Debugger* New();
  static void Release(Debugger* debugger);

  const char* obj_name() const { return "debugger"; }

  virtual Status Register(const char* command, DebugHandler* handler) = 0;
  virtual void Unregister(const char* command) = 0;
  virtual Status OnRequest(DebugIo* io, const std::string& command,
                           const std::string& args) = 0;
};

}  // namespace bbt

#endif  // BBT_MODULES_DEBUGGER_H_
