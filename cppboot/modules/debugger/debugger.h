#ifndef CPPBOOT_MODULES_DEBUGGER_H_
#define CPPBOOT_MODULES_DEBUGGER_H_

#include "cppboot/base/string_view.h"
#include "cppboot/base/status.h"

#include "cppboot/appkit/object.h"

namespace cppboot {

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

  const char* object_id() const { return "debugger"; }

  virtual Status Register(const char* command, DebugHandler* handler) = 0;
  virtual void Unregister(const char* command) = 0;
  virtual Status OnRequest(DebugIo* io, const std::string& command,
                           const std::string& args) = 0;
};

}  // namespace cppboot

#endif  // CPPBOOT_MODULES_DEBUGGER_H_
