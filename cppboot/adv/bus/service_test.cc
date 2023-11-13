#include "gmock/gmock.h"

#include "cppboot/adv/bus/service.h"
#include "cppboot/adv/bus/msg.h"

namespace {

using cppboot::BusService;
using cppboot::In;
using cppboot::Msg;
using cppboot::Out;

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