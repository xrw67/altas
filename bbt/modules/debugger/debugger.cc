#include "bbt/modules/debugger/debugger.h"

#include <map>

#include "bbt/util/logging.h"
#include "bbt/appkit/module.h"

namespace bbt {
class DebuggerImpl : public Debugger {
 public:
  DebuggerImpl() {}

  Status Register(const char* command, DebugHandler* handler) {
    if (!command || !*command) return InvalidArgumentError("no command");
    if (!handler) return InvalidArgumentError("no handler");

    cmds_[command] = handler;
    return OkStatus();
  }

  void Unregister(const char* command) {
    if (!command || !*command) return;

    auto it = cmds_.find(command);
    if (it != cmds_.end()) cmds_.erase(it);
  }

  Status OnRequest(DebugIo* io, const std::string& command,
                   const std::string& args) {
    auto it = cmds_.find(command);
    if (it != cmds_.end())
      return it->second->OnDebugRequest(io, command, args);
    else
      return NotFoundError(command);
  }

 private:
  std::map<std::string, DebugHandler*> cmds_;
};

Debugger* Debugger::New() { return new DebuggerImpl(); }

void Debugger::Release(Debugger* debugger) { delete debugger; }

}  // namespace bbt

int debugger_init(const char* param) {
  BBT_LOG(INFO, "Debugger init");
  return 0;
}

void debugger_exit(void) { BBT_LOG(INFO, "Debugger exit"); }

extern "C" __attribute__((visibility("default")))
BBT_MODULE_HEADER bbt_module_header = {
    .name = "debugger",
    .version = "0.1.0",
    .requires = "",
    .init = debugger_init,
    .exit = debugger_exit,
};
