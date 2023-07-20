#include "bbt/net/buffer.h"

#include "gtest/gtest.h"

namespace {

TEST(Buffer, Constructor) {
  bbt::Buffer buf;
  ASSERT_EQ(buf.ReadableBytes(), 0);
  ASSERT_EQ(buf.WritableBytes(), bbt::Buffer::kInitialSize);
  ASSERT_EQ(buf.PrependableBytes(), bbt::Buffer::kCheapPrepend);
}

TEST(Buffer, Swap) {
  bbt::Buffer buf1(100);
  bbt::Buffer buf2(200);

  buf1.swap(buf2);
  ASSERT_EQ(buf1.WritableBytes(), 200);
  ASSERT_EQ(buf2.WritableBytes(), 100);
}

TEST(Buffer, Append) {
  bbt::Buffer buf(1);

  buf.Append("Hi");
  ASSERT_EQ(buf.ReadableBytes(), 2);
  buf.Append((const unsigned char*)"H", 1);
  ASSERT_EQ(buf.ReadableBytes(), 3);

  ASSERT_EQ(buf.Str(), bbt::string_view("HiH"));

  // Retrive
  buf.Retrive(2);
  ASSERT_EQ(buf.ReadableBytes(), 1);
  ASSERT_EQ(buf.Str(), bbt::string_view("H"));
}

}  // namespace