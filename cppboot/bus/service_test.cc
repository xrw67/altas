#include "gmock/gmock.h"

#include "cppboot/bus/service.h"
#include "cppboot/bus/msg.h"

namespace {

using cppboot::bus::BusService;
using cppboot::bus::In;
using cppboot::bus::Msg;
using cppboot::bus::Out;

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