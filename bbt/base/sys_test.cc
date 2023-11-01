#include "gmock/gmock.h"
#include "bbt/base/sys.h"

namespace {

TEST(Sys, GetNanoClock) {
  auto tm1 = bbt::GetNanoClock();
  sleep(0);
  ASSERT_GT(bbt::GetNanoClock(), tm1);
}

}  // namespace
