#include "gmock/gmock.h"
#include "cppboot/base/sys.h"

namespace {

TEST(Sys, GetNanoClock) {
  auto tm1 = cppboot::GetNanoClock();
  sleep(0);
  ASSERT_GT(cppboot::GetNanoClock(), tm1);
}

}  // namespace
