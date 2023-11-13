#include "cppboot/plugins/debugger/debugger.h"

#include <map>
#include <string>

#include "gmock/gmock.h"

namespace cppboot {

class MockDebugIo : public DebugIo {
 public:
  void set_debugger(Debugger* debugger) { debugger_ = debugger; }
  void Write(const std::string& content) { write_buffer_ = content; }

  std::string Run(const std::string& cmd, const std::string& args = "") {
    write_buffer_.clear();
    Status st = debugger_->OnRequest(this, cmd, args);
    if (!st.ok())
      return st.ToString();
    else
      return write_buffer_;
  }

 private:
  Debugger* debugger_;
  std::string write_buffer_;
};

class MockDebugHandler : public DebugHandler {
 public:
  MockDebugHandler() {
    cmds_["cmd1"] = "resp1";
    cmds_["cmd2"] = "resp2";
    cmds_["cmd3"] = "resp3";
  }

  Status OnDebugRequest(DebugIo* io, const std::string& cmd,
                        const std::string& args) {
    auto it = cmds_.find(cmd);
    if (it != cmds_.end())
      io->Write(it->second);
    else
      io->Write("empty");

    return OkStatus();
  }

 private:
  std::map<std::string, std::string> cmds_;
};

TEST(Debugger, Create) {
  Debugger* dbg = Debugger::New();
  MockDebugHandler handler;
  MockDebugIo io;

  io.set_debugger(dbg);

  ASSERT_EQ(dbg->Register(NULL, &handler).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(dbg->Register("", &handler).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(dbg->Register("cmd1", NULL).code(), StatusCode::kInvalidArgument);

  ASSERT_EQ(dbg->Register("cmd1", &handler).code(), StatusCode::kOk);
  ASSERT_EQ(dbg->Register("cmd2", &handler).code(), StatusCode::kOk);

  ASSERT_EQ(io.Run("cmd1"), "resp1");
  ASSERT_EQ(io.Run("cmd2"), "resp2");
  ASSERT_EQ(io.Run("cmd3"), "NOT_FOUND: cmd3");

  dbg->Unregister(NULL);
  dbg->Unregister("");
  dbg->Unregister("cmd1");

  ASSERT_EQ(io.Run("cmd1"), "NOT_FOUND: cmd1");

  Debugger::Release(dbg);
}

}  // namespace cppboot