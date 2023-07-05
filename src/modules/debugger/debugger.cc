#include "bbt/appkit/module.h"

#include <map>

#include "bbt/base/log.h"
#include "bbt/modules/debugger.h"

namespace bbt {
class DebuggerImpl : public Debugger {
 public:
  DebuggerImpl() {}

  Status Register(const char* command, DebugHandler* handler) {
    if (!command || !*command) return InvalidArgumentError("no command");
    if (!handler) return InvalidArgumentError("no handler");
    return OkStatus();
  }

 private:
  std::map<std::string, DebugHandler*> cmds_;
};

Debugger* Debugger::New(DebugProvider* provider) { return new DebuggerImpl(); }
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
