#ifndef BBT_NET_BUFFER_H_
#define BBT_NET_BUFFER_H_

#include <vector>
#include <algorithm>

#include "bbt/util/string_view.h"

namespace bbt {

/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode

class Buffer {
 public:
  static const size_t kCheapPrepend;
  static const size_t kInitialSize;

  explicit Buffer(size_t initial_size = kInitialSize)
      : buffer_(kCheapPrepend + initial_size),
        reader_(kCheapPrepend),
        writer_(kCheapPrepend) {}

  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  void swap(Buffer& rhs) {
    buffer_.swap(rhs.buffer_);
    std::swap(reader_, rhs.reader_);
    std::swap(writer_, rhs.writer_);
  }

  size_t PrependableBytes() const noexcept { return reader_; }
  size_t ReadableBytes() const noexcept { return writer_ - reader_; }
  size_t WritableBytes() const noexcept { return buffer_.size() - writer_; }

  const unsigned char* Peek() const noexcept { return begin() + reader_; }

  string_view Str() const noexcept {
    return string_view((const char*)Peek(), ReadableBytes());
  }

  void Retrive(size_t len) noexcept {
    if (len < ReadableBytes())
      reader_ += len;
    else
      RetriveAll();
  }

  void RetriveAll() noexcept {
    reader_ = kCheapPrepend;
    writer_ = kCheapPrepend;
  }

  void Append(const unsigned char* data, size_t len) noexcept {
    EnsureWritableBytes(len);
    std::copy(data, data + len, BeginWrite());
    HasWritten(len);
  }
  void Append(string_view s) noexcept {
    Append((const unsigned char*)s.data(), s.length());
  }

  void EnsureWritableBytes(size_t len) noexcept {
    if (WritableBytes() < len) {
      MakeSpace(len);
    }
    assert(WritableBytes() >= len);
  }

  unsigned char* BeginWrite() noexcept { return begin() + writer_; }

  const unsigned char* BeginWrite() const noexcept { return begin() + writer_; }

  void HasWritten(size_t len) noexcept {
    assert(len <= WritableBytes());
    writer_ += len;
  }

  void Unwrite(size_t len) noexcept {
    assert(len <= ReadableBytes());
    writer_ -= len;
  }

 private:
  unsigned char* begin() { return &*buffer_.begin(); }

  const unsigned char* begin() const { return &*buffer_.begin(); }

  void MakeSpace(size_t len) {
    if (WritableBytes() + PrependableBytes() < len + kCheapPrepend) {
      // FIXME: move readable data
      buffer_.resize(writer_ + len);
    } else {
      // move readable data to the front, make space inside buffer
      assert(kCheapPrepend < reader_);
      size_t readable = ReadableBytes();
      std::copy(begin() + reader_, begin() + writer_, begin() + kCheapPrepend);
      reader_ = kCheapPrepend;
      writer_ = reader_ + readable;
      assert(readable == ReadableBytes());
    }
  }

  std::vector<unsigned char> buffer_;
  size_t reader_;
  size_t writer_;
};

}  // namespace bbt

#endif  // BBT_UTIL_BUFFER_H_
