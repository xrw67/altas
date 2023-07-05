#ifndef BBT_MODULES_DEBUGGER_H_
#define BBT_MODULES_DEBUGGER_H_

#include "bbt/appkit/service.h"
#include "bbt/base/string_view.h"

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

class Debugger : public Service {
 public:
  static Debugger* New();
  static void Release(Debugger* debugger);

  const char* service_name() const { return "debugger"; }

  virtual Status Register(const char* command, DebugHandler* handler) = 0;
  virtual void Unregister(const char* command) = 0;
  virtual Status OnRequest(DebugIo* io, const std::string& command,
                           const std::string& args) = 0;
};

}  // namespace bbt

#endif  // BBT_MODULES_DEBUGGER_H_
