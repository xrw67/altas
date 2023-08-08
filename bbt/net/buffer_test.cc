#include "gmock/gmock.h"

#include "bbt/net/buffer.h"

namespace {

using bbt::net::Buffer;

TEST(Buffer, Constructor) {
  Buffer buf;
  ASSERT_EQ(buf.ReadableBytes(), 0);
  ASSERT_EQ(buf.WritableBytes(), Buffer::kInitialSize);
  ASSERT_EQ(buf.PrependableBytes(), Buffer::kCheapPrepend);
}

TEST(Buffer, Swap) {
  Buffer buf1(100);
  Buffer buf2(200);

  buf1.swap(buf2);
  ASSERT_EQ(buf1.WritableBytes(), 200);
  ASSERT_EQ(buf2.WritableBytes(), 100);
}

TEST(Buffer, Append) {
  Buffer buf(1);

  buf.Append("Hi");
  ASSERT_EQ(buf.ReadableBytes(), 2);
  buf.Append("H", 1);
  ASSERT_EQ(buf.ReadableBytes(), 3);

  ASSERT_EQ(buf.Str(), bbt::string_view("HiH"));

  // Retrive
  buf.Retrive(2);
  ASSERT_EQ(buf.ReadableBytes(), 1);
  ASSERT_EQ(buf.Str(), bbt::string_view("H"));
}

}  // namespace