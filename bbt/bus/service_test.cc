#include "gmock/gmock.h"

#include "bbt/bus/service.h"
#include "bbt/bus/msg.h"

namespace {

using bbt::bus::BusService;
using bbt::bus::In;
using bbt::bus::Msg;
using bbt::bus::Out;

TEST(BusServiceTest, should_serve_msg) {
  BusService svc("noname");

  ASSERT_TRUE(svc.AddMethod("echo", [](const In& in, Out* out) {
    out->set("key", in.get("key") + "World");
  }));

  Msg req, resp;
  req.set_method("echo");
  req.set_param("key", "Hello");
  svc.ServeMsg(req, &resp);
  ASSERT_EQ(resp.param("key"), "HelloWorld");
}

}  // namespace