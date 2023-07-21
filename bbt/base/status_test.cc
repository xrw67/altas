#include "bbt/base/status.h"

#include <utility>

#include "gmock/gmock.h"

#include "bbt/base/fmt.h"
#include "bbt/base/str_util.h"

using namespace bbt;

TEST(StatusCode, InsertionOperator) {
  const StatusCode code = StatusCode::kUnknown;
  std::ostringstream oss;
  oss << code;
  EXPECT_EQ(oss.str(), StatusCodeToString(code));
  EXPECT_EQ(StatusCodeToString(StatusCode::kOk), "OK");
  EXPECT_EQ(StatusCodeToString(StatusCode(-1)), "");
}

// This structure holds the details for testing a single error code,
// its creator, and its classifier.
struct ErrorTest {
  bbt::StatusCode code;
  using Creator = bbt::Status (*)(bbt::string_view);
  using Classifier = bool (*)(const bbt::Status&);
  Creator creator;
  Classifier classifier;
};

constexpr ErrorTest kErrorTests[]{
    {bbt::StatusCode::kCancelled, bbt::CancelledError, bbt::IsCancelled},
    {bbt::StatusCode::kUnknown, bbt::UnknownError, bbt::IsUnknown},
    {bbt::StatusCode::kDeadlineExceeded, bbt::DeadlineExceededError,
     bbt::IsDeadlineExceeded},
    {bbt::StatusCode::kInvalidArgument, bbt::InvalidArgumentError,
     bbt::IsInvalidArgument},
    {bbt::StatusCode::kNotFound, bbt::NotFoundError, bbt::IsNotFound},
    {bbt::StatusCode::kAlreadyExists, bbt::AlreadyExistsError,
     bbt::IsAlreadyExists},
    {bbt::StatusCode::kPermissionDenied, bbt::PermissionDeniedError,
     bbt::IsPermissionDenied},
    {bbt::StatusCode::kResourceExhausted, bbt::ResourceExhaustedError,
     bbt::IsResourceExhausted},
    {bbt::StatusCode::kFailedPrecondition, bbt::FailedPreconditionError,
     bbt::IsFailedPrecondition},
    {bbt::StatusCode::kAborted, bbt::AbortedError, bbt::IsAborted},
    {bbt::StatusCode::kOutOfRange, bbt::OutOfRangeError, bbt::IsOutOfRange},
    {bbt::StatusCode::kUnimplemented, bbt::UnimplementedError,
     bbt::IsUnimplemented},
    {bbt::StatusCode::kUnavailable, bbt::UnavailableError, bbt::IsUnavailable},
};

TEST(Status, CreateAndClassify) {
  for (const auto& test : kErrorTests) {
    SCOPED_TRACE(bbt::StatusCodeToString(test.code));

    // Ensure that the creator does, in fact, create status objects with the
    // expected error code and message.
    std::string message = bbt::format("error code {} test message", test.code);
    bbt::Status status = test.creator(message);
    EXPECT_EQ(test.code, status.code());
    EXPECT_EQ(message, status.message());

    // Ensure that the classifier returns true for a status produced by the
    // creator.
    EXPECT_TRUE(test.classifier(status));

    // Ensure that the classifier returns false for status with a different
    // code.
    for (const auto& other : kErrorTests) {
      if (other.code != test.code) {
        EXPECT_FALSE(test.classifier(bbt::Status(other.code, "")))
            << " other.code = " << other.code;
      }
    }
  }
}

TEST(Status, DefaultConstructor) {
  bbt::Status status;
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(bbt::StatusCode::kOk, status.code());
  EXPECT_EQ("", status.message());
}

TEST(Status, OkStatus) {
  bbt::Status status = bbt::OkStatus();
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(bbt::StatusCode::kOk, status.code());
  EXPECT_EQ("", status.message());
}

TEST(Status, ConstructorWithCodeMessage) {
  {
    bbt::Status status(bbt::StatusCode::kCancelled, "");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(bbt::StatusCode::kCancelled, status.code());
    EXPECT_EQ("", status.message());
  }
  {
    bbt::Status status(bbt::StatusCode::kUnknown, "message");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(bbt::StatusCode::kUnknown, status.code());
    EXPECT_EQ("message", status.message());
  }
}

TEST(Status, ConstructorWithFormatMessage) {
  {
    bbt::Status status(bbt::StatusCode::kUnknown, "%s %s %d", "Hello", "World",
                       666);
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(bbt::StatusCode::kUnknown, status.code());
    EXPECT_EQ("Hello World 666", status.message());
  }

  {
    const std::string sufix(100000, 'A');
    const std::string fmt = "%s %s %d: " + sufix;
    const std::string result = "Hello World 666: " + sufix;

    bbt::Status status(bbt::StatusCode::kUnknown, fmt.c_str(), "Hello", "World",
                       666);
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(bbt::StatusCode::kUnknown, status.code());
    EXPECT_EQ(result, status.message()) << "Large string size";
  }

  {
    bbt::Status status(bbt::StatusCode::kUnknown, "msg1", "msg2");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(bbt::StatusCode::kUnknown, status.code());
    EXPECT_EQ("msg1", status.message());
  }
}

TEST(Status, MoveConstructor) {
  {
    Status ok = OkStatus();
    Status ok2 = std::move(ok);

    ASSERT_TRUE(ok2.ok());
  }

  {
    Status status = NotFoundError("custom NotFound status message");
    Status status2 = std::move(status);

    ASSERT_TRUE(IsNotFound(status2));
    ASSERT_EQ("NOT_FOUND: custom NotFound status message", status2.ToString());
  }

  {
    Status self_moved = UnknownError("custom Unknow status message");

    // Needed to bypass compiler warning about explicit move-assignment.
    Status& self_moved_reference = self_moved;
    self_moved_reference = std::move(self_moved);
  }
}

TEST(Status, Update) {
  {
    Status st = OkStatus();
    st.Update(NotFoundError("msg1"));
    ASSERT_EQ("NOT_FOUND: msg1", st.ToString());

    st.Update(InvalidArgumentError("msg2"));
    ASSERT_EQ("NOT_FOUND: msg1", st.ToString());
  }

  // Copy
  {
    Status st = OkStatus();
    const Status err = NotFoundError("msg");
    st.Update(err);
    ASSERT_EQ(st.ToString(), err.ToString());
  }

  // Move
  {
    Status st = OkStatus();
    Status err = NotFoundError("msg");
    st.Update(std::move(err));
    ASSERT_EQ("NOT_FOUND: msg", st.ToString());
    ASSERT_TRUE(err.ok());
  }
}

TEST(Status, InsertionOperator) {
  std::ostringstream oss;
  Status st = OkStatus();
  Status err = NotFoundError("msg");

  oss << st << ", " << err;
  EXPECT_EQ(oss.str(), "OK, NOT_FOUND: msg");
}

TEST(Status, StatusMessageCStringTest) {
  {
    bbt::Status status = bbt::OkStatus();
    EXPECT_EQ(status.message(), "");
    EXPECT_STREQ(bbt::StatusMessageAsCStr(status), "");
    EXPECT_EQ(status.message(), bbt::StatusMessageAsCStr(status));
    EXPECT_NE(bbt::StatusMessageAsCStr(status), nullptr);
  }
  {
    bbt::Status status;
    EXPECT_EQ(status.message(), "");
    EXPECT_NE(bbt::StatusMessageAsCStr(status), nullptr);
    EXPECT_STREQ(bbt::StatusMessageAsCStr(status), "");
  }
  {
    bbt::Status status(bbt::StatusCode::kInternal, "message");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(bbt::StatusCode::kInternal, status.code());
    EXPECT_EQ("message", status.message());
    EXPECT_STREQ("message", bbt::StatusMessageAsCStr(status));
  }
}

TEST(StatusErrno, ErrnoToStatusCode) {
  EXPECT_EQ(bbt::ErrnoToStatusCode(0), bbt::StatusCode::kOk);

  // Spot-check a few errno values.
  EXPECT_EQ(bbt::ErrnoToStatusCode(EINVAL), bbt::StatusCode::kInvalidArgument);
  EXPECT_EQ(bbt::ErrnoToStatusCode(ENOENT), bbt::StatusCode::kNotFound);

  // We'll pick a very large number so it hopefully doesn't collide to errno.
  EXPECT_EQ(bbt::ErrnoToStatusCode(19980927), bbt::StatusCode::kUnknown);
}

TEST(StatusErrno, ErrnoToStatus) {
  bbt::Status status = bbt::ErrnoToStatus(ENOENT, "Cannot open 'path'");
  EXPECT_EQ(status.code(), bbt::StatusCode::kNotFound);
  EXPECT_EQ(status.message(), "Cannot open 'path': No such file or directory");
}
