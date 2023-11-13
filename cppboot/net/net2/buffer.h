#ifndef CPPBOOT_NET2_BUFFER_H_
#define CPPBOOT_NET2_BUFFER_H_

#include <assert.h>
#include <string>
#include <vector>
#include <algorithm>

namespace cppboot {
namespace net2 {

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
  enum {
    kCheapPrepend = 8,
    kInitialSize = 256,
  };

  struct Slice {
    const char* data;
    size_t len;

    Slice(const char* data, size_t len) : data(data), len(len) {}
  };

  explicit Buffer(size_t initial_size = kInitialSize)
      : buffer_(kCheapPrepend + initial_size),
        reader_(kCheapPrepend),
        writer_(kCheapPrepend) {}

  void swap(Buffer& rhs) {
    buffer_.swap(rhs.buffer_);
    std::swap(reader_, rhs.reader_);
    std::swap(writer_, rhs.writer_);
  }

  bool empty() const { return ReadableBytes() == 0; }
  size_t PrependableBytes() const { return reader_; }
  size_t ReadableBytes() const { return writer_ - reader_; }
  size_t WritableBytes() const { return buffer_.size() - writer_; }

  const char* Peek() const { return begin() + reader_; }
  const char* FindCRLF() const {
    const char* crlf = std::search(Peek(), BeginWrite(), kCRLF, kCRLF + 2);
    return crlf == BeginWrite() ? NULL : crlf;
  };

  Slice ToSlice() const { return Slice(Peek(), ReadableBytes()); }

  std::string ToString() const { return std::string(Peek(), ReadableBytes()); }

  void Retrive(size_t len) {
    if (len < ReadableBytes())
      reader_ += len;
    else
      RetriveAll();
  }

  void RetriveUtil(const char* end) {
    assert(Peek() <= end);
    assert(end <= BeginWrite());
    Retrive(end - Peek());
  }

  void RetriveAll() {
    reader_ = kCheapPrepend;
    writer_ = kCheapPrepend;
  }

  std::string RetriveAsString(size_t len) {
    assert(len <= ReadableBytes());
    std::string result(Peek(), len);
    Retrive(len);
    return result;
  }

  std::string RetriveAllAsString() { return RetriveAsString(ReadableBytes()); }

  Buffer& Append(const char* data, size_t len) {
    EnsureWritableBytes(len);
    std::copy(data, data + len, BeginWrite());
    HasWritten(len);
    return *this;
  }

  Buffer& Append(const void* data, size_t len) {
    return Append(static_cast<const char*>(data), len);
  }

  Buffer& Append(Slice s) { return Append(s.data, s.len); }

  Buffer& Append(const std::string& s) {
    return Append((const char*)s.data(), s.length());
  }

  void EnsureWritableBytes(size_t len) {
    if (WritableBytes() < len) {
      MakeSpace(len);
    }
    assert(WritableBytes() >= len);
  }

  char* BeginWrite() { return begin() + writer_; }

  const char* BeginWrite() const { return begin() + writer_; }

  void HasWritten(size_t len) {
    assert(len <= WritableBytes());
    writer_ += len;
  }

  void Unwrite(size_t len) {
    assert(len <= ReadableBytes());
    writer_ -= len;
  }

  void Shrink(size_t reserve) {
    Buffer other;
    other.EnsureWritableBytes(ReadableBytes() + reserve);
    other.Append(ToSlice());
    swap(other);
  }

  size_t Capacity() const { return buffer_.capacity(); }

 private:
  Buffer(const Buffer&);
  Buffer& operator=(const Buffer&);

  char* begin() { return &*buffer_.begin(); }

  const char* begin() const { return &*buffer_.begin(); }

  void MakeSpace(size_t len) {
    if (WritableBytes() + PrependableBytes() < len + kCheapPrepend) {
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

  std::vector<char> buffer_;
  size_t reader_;
  size_t writer_;

  static const char kCRLF[];
};

}  // namespace net2
}  // namespace cppboot

#endif  // CPPBOOT_NET2_BUFFER_H_
