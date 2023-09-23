#include "gmock/gmock.h"

#include "buffer.h"

namespace {

using bbt::net2::Buffer;

TEST(Buffer, Constructor) {
  Buffer buf;
  ASSERT_EQ(0, buf.ReadableBytes());
  ASSERT_EQ(Buffer::kInitialSize, buf.WritableBytes());
  ASSERT_EQ(Buffer::kCheapPrepend, buf.PrependableBytes());
}

TEST(Buffer, Swap) {
  Buffer buf1(100);
  Buffer buf2(200);

  buf1.swap(buf2);
  ASSERT_EQ(200, buf1.WritableBytes());
  ASSERT_EQ(100, buf2.WritableBytes());
}

TEST(Buffer, Append) {
  Buffer buf(1);

  buf.Append("Hi");
  ASSERT_EQ(2, buf.ReadableBytes());
  buf.Append("H", 1);
  ASSERT_EQ(3, buf.ReadableBytes());
  ASSERT_EQ("HiH", buf.ToString());
}

TEST(Buffer, Retrive) {
  Buffer b;

  b.Append("Hello World");
  b.Retrive(2);
  ASSERT_EQ("llo World", b.ToString());

  b.RetriveUtil(b.Peek() + 4);
  ASSERT_EQ("World", b.ToString());

  ASSERT_EQ("Wo", b.RetriveAsString(2));
  ASSERT_EQ("rld", b.RetriveAllAsString());
}

TEST(Buffer, Shrink) {
  Buffer b(256);
  b.Append("Hello");
  ASSERT_EQ(Buffer::kCheapPrepend + 256, b.Capacity());
  b.Shrink(1000);
  ASSERT_EQ("Hello", b.ToString());
  ASSERT_EQ(Buffer::kCheapPrepend + 1000 + 5, b.Capacity());
}

TEST(Buffer, CRLF) {
  Buffer b;
  b.Append("Hello\r\nWorld");
  ASSERT_STREQ("\r\nWorld", b.FindCRLF());
}

}  // namespace