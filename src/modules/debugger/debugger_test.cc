#include "bbt/modules/debugger.h"

#include <map>
#include <string>

#include <gmock/gmock.h>

namespace bbt {

class MockDebugProvider : public DebugProvider {
 public:
  std::string Run(const std::string& cmd) { return "resp1"; }

  // Status Start() {}
  // Status Stop() {}
  Status WriteResponse(const char* content) {
    write_buffer_ = content;
    return OkStatus();
  }

 private:
  std::string write_buffer_;
};

class MockDebugHandler : public DebugHandler {
 public:
  MockDebugHandler() {
    cmds_["cmd1"] = "resp1";
    cmds_["cmd2"] = "resp2";
    cmds_["cmd3"] = "resp3";
    cmds_["cmd4"] = "resp4";
    cmds_["cmd5"] = "resp5";
  }

  std::string OnDebugCommand(const char* cmd, const char* args) {
    auto it = cmds_.find(cmd);
    if (it != cmds_.end()) return it->second;

    return "empty";
  }

 private:
  std::map<std::string, std::string> cmds_;
};

TEST(Debugger, Create) {
  MockDebugHandler handler;
  MockDebugProvider provider;
  Debugger* dbg = Debugger::New(&provider);

  ASSERT_EQ(dbg->Register(NULL, &handler).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(dbg->Register("", &handler).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(dbg->Register("cmd1", NULL).code(), StatusCode::kInvalidArgument);

  //   dbg->Register("cmd1", &handler);
  //   dbg->Register("cmd2", &handler);
  //   dbg->Register("cmd3", &handler);

  Debugger::Release(dbg);
}
}  // namespace bbt