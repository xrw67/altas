#include "gmock/gmock.h"
#include "bbt/bus/msg_queue.h"
#include "bbt/bus/msg.h"

namespace {
using bbt::bus::Msg;
using bbt::bus::MsgPtr;

TEST(MsgQueue, example) {
  bbt::bus::MsgQueue q;

  MsgPtr msg1(new Msg(1)), msg2(new Msg(2)), msg3(new Msg(3));

  ASSERT_TRUE(q.Push(msg1));
  ASSERT_FALSE(q.Push(msg1));
  ASSERT_TRUE(q.Push(msg2));
  ASSERT_TRUE(q.Push(msg3));

  ASSERT_EQ(q.Size(), 3);

  ASSERT_EQ(q.Pop(2), msg2);
  ASSERT_EQ(q.Size(), 2);

  ASSERT_EQ(q.Pop(2), nullptr);

  ASSERT_EQ(q.Pop(3), msg3);
  ASSERT_EQ(q.Size(), 1);

  ASSERT_EQ(q.Pop(1), msg1);
  ASSERT_EQ(q.Size(), 0);
}

}  // namespace