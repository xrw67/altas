#include <gmock/gmock.h>

#include <thread>

#include "context.h"

namespace {
using bbt::io::Context;

class ContextTest : public ::testing::Test {
 protected:
  Context ctx;
};

TEST_F(ContextTest, create_context_and_queue_is_empty) {
  ASSERT_EQ(0, ctx.QueueSize());
}

TEST_F(ContextTest, DISABLED_run_blocking_when_stop_it) {
  std::thread t(std::bind(&Context::RunOnce, &ctx));
  ctx.Stop();
  t.join();
}

//
// Post
//

TEST_F(ContextTest, post_functor_and_queue_is_1) {
  ctx.Post([] {});
  ASSERT_EQ(1, ctx.QueueSize());
}

TEST_F(ContextTest, DISABLED_post_and_run_once) {
  int num = 0;

  ctx.Post([&num]() { num += 1; });
  ASSERT_EQ(0, num);
  ctx.RunOnce();
  ASSERT_EQ(1, num);
  ASSERT_EQ(0, ctx.QueueSize());
};

TEST_F(ContextTest, DISABLED_post_multi_functios_and_run) {
  int n = 0;

  ctx.Post([&] { n = 88; });
  ctx.Post([&] { n = 99; });
  ctx.RunOnce();
  ASSERT_EQ(99, n);
}

//
// Notify
//

}  // namespace