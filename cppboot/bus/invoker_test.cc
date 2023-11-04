#include "gmock/gmock.h"
#include "cppboot/bus/invoker.h"

namespace cppboot {
namespace bus {
namespace {

TEST(BusInvokerTest, should_call_service) {
  BusInvoker invoker("noname");

  invoker.set_msg_writer([&invoker](const MsgPtr& req) {
    MsgPtr resp(new Msg());
    resp->set_id(req->id());
    resp->set_caller(req->caller());
    resp->set_request(false);

    for (auto i : req->params()) {
      resp->set_param(i.first, i.second);
    }
    resp->set_param("return", "OK");
    invoker.HandleResponseMessage(resp);
  });

  In in;
  Out out;

  in.set("key", "666");
  auto st = invoker.Call("MyEcho.Echo", in, &out);

  ASSERT_EQ(out.get("key"), "666");
  ASSERT_EQ(out.get("key"), "666");
}

}  // namespace
}  // namespace bus
}  // namespace cppboot