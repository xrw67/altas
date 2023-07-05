#ifndef BBT_MODULES_DEBUGGER_H_
#define BBT_MODULES_DEBUGGER_H_

#include "bbt/appkit/service.h"

namespace bbt {

class DebugHandler {
 public:
  virtual std::string OnDebugCommand(const char* cmd, const char* args) = 0;

 protected:
  virtual ~DebugHandler() {}
};

class DebugProvider {
 public:
  virtual ~DebugProvider() {}
  // virtual Status Start() = 0;
  // virtual Status Stop() = 0;
  // virtual Status WriteResponse(const char* content) = 0;
};

class Debugger : public Service {
 public:
  static Debugger* New(DebugProvider* provider);
  static void Release(Debugger* debugger);

  const char* service_name() const { return "debugger"; }

  virtual Status Register(const char* command, DebugHandler* handler) = 0;
  // virtual void Unregister(const char* command) = 0;
  // virtual Status OnRequest(const char* command) = 0;
};

}  // namespace bbt

#endif  // BBT_MODULES_DEBUGGER_H_
