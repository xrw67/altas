#include "cppboot/base/status.h"

#include <utility>

#include "gmock/gmock.h"

#include "cppboot/base/fmt.h"
#include "cppboot/base/str_util.h"

using namespace cppboot;

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
  cppboot::StatusCode code;
  using Creator = cppboot::Status (*)(cppboot::string_view);
  using Classifier = bool (*)(const cppboot::Status&);
  Creator creator;
  Classifier classifier;
};

constexpr ErrorTest kErrorTests[]{
    {cppboot::StatusCode::kCancelled, cppboot::CancelledError,
     cppboot::IsCancelled},
    {cppboot::StatusCode::kUnknown, cppboot::UnknownError, cppboot::IsUnknown},
    {cppboot::StatusCode::kDeadlineExceeded, cppboot::DeadlineExceededError,
     cppboot::IsDeadlineExceeded},
    {cppboot::StatusCode::kInvalidArgument, cppboot::InvalidArgumentError,
     cppboot::IsInvalidArgument},
    {cppboot::StatusCode::kNotFound, cppboot::NotFoundError,
     cppboot::IsNotFound},
    {cppboot::StatusCode::kAlreadyExists, cppboot::AlreadyExistsError,
     cppboot::IsAlreadyExists},
    {cppboot::StatusCode::kPermissionDenied, cppboot::PermissionDeniedError,
     cppboot::IsPermissionDenied},
    {cppboot::StatusCode::kResourceExhausted, cppboot::ResourceExhaustedError,
     cppboot::IsResourceExhausted},
    {cppboot::StatusCode::kFailedPrecondition, cppboot::FailedPreconditionError,
     cppboot::IsFailedPrecondition},
    {cppboot::StatusCode::kAborted, cppboot::AbortedError, cppboot::IsAborted},
    {cppboot::StatusCode::kOutOfRange, cppboot::OutOfRangeError,
     cppboot::IsOutOfRange},
    {cppboot::StatusCode::kUnimplemented, cppboot::UnimplementedError,
     cppboot::IsUnimplemented},
    {cppboot::StatusCode::kUnavailable, cppboot::UnavailableError,
     cppboot::IsUnavailable},
};

TEST(Status, CreateAndClassify) {
  for (const auto& test : kErrorTests) {
    SCOPED_TRACE(cppboot::StatusCodeToString(test.code));

    // Ensure that the creator does, in fact, create status objects with the
    // expected error code and message.
    std::string message =
        cppboot::format("error code {} test message", test.code);
    cppboot::Status status = test.creator(message);
    EXPECT_EQ(test.code, status.code());
    EXPECT_EQ(message, status.message());

    // Ensure that the classifier returns true for a status produced by the
    // creator.
    EXPECT_TRUE(test.classifier(status));

    // Ensure that the classifier returns false for status with a different
    // code.
    for (const auto& other : kErrorTests) {
      if (other.code != test.code) {
        EXPECT_FALSE(test.classifier(cppboot::Status(other.code, "")))
            << " other.code = " << other.code;
      }
    }
  }
}

TEST(Status, DefaultConstructor) {
  cppboot::Status status;
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(cppboot::StatusCode::kOk, status.code());
  EXPECT_EQ("", status.message());
}

TEST(Status, OkStatus) {
  cppboot::Status status = cppboot::OkStatus();
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(cppboot::StatusCode::kOk, status.code());
  EXPECT_EQ("", status.message());
}

TEST(Status, ConstructorWithCodeMessage) {
  {
    cppboot::Status status(cppboot::StatusCode::kCancelled, "");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(cppboot::StatusCode::kCancelled, status.code());
    EXPECT_EQ("", status.message());
  }
  {
    cppboot::Status status(cppboot::StatusCode::kUnknown, "message");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(cppboot::StatusCode::kUnknown, status.code());
    EXPECT_EQ("message", status.message());
  }
}

TEST(Status, ConstructorWithFormatMessage) {
  {
    cppboot::Status status(cppboot::StatusCode::kUnknown, "%s %s %d", "Hello",
                           "World", 666);
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(cppboot::StatusCode::kUnknown, status.code());
    EXPECT_EQ("Hello World 666", status.message());
  }

  {
    const std::string sufix(100000, 'A');
    const std::string fmt = "%s %s %d: " + sufix;
    const std::string result = "Hello World 666: " + sufix;

    cppboot::Status status(cppboot::StatusCode::kUnknown, fmt.c_str(), "Hello",
                           "World", 666);
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(cppboot::StatusCode::kUnknown, status.code());
    EXPECT_EQ(result, status.message()) << "Large string size";
  }

  {
    cppboot::Status status(cppboot::StatusCode::kUnknown, "msg1", "msg2");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(cppboot::StatusCode::kUnknown, status.code());
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
    cppboot::Status status = cppboot::OkStatus();
    EXPECT_EQ(status.message(), "");
    EXPECT_STREQ(cppboot::StatusMessageAsCStr(status), "");
    EXPECT_EQ(status.message(), cppboot::StatusMessageAsCStr(status));
    EXPECT_NE(cppboot::StatusMessageAsCStr(status), nullptr);
  }
  {
    cppboot::Status status;
    EXPECT_EQ(status.message(), "");
    EXPECT_NE(cppboot::StatusMessageAsCStr(status), nullptr);
    EXPECT_STREQ(cppboot::StatusMessageAsCStr(status), "");
  }
  {
    cppboot::Status status(cppboot::StatusCode::kInternal, "message");
    EXPECT_FALSE(status.ok());
    EXPECT_EQ(cppboot::StatusCode::kInternal, status.code());
    EXPECT_EQ("message", status.message());
    EXPECT_STREQ("message", cppboot::StatusMessageAsCStr(status));
  }
}

TEST(StatusErrno, ErrnoToStatusCode) {
  EXPECT_EQ(cppboot::ErrnoToStatusCode(0), cppboot::StatusCode::kOk);

  // Spot-check a few errno values.
  EXPECT_EQ(cppboot::ErrnoToStatusCode(EINVAL),
            cppboot::StatusCode::kInvalidArgument);
  EXPECT_EQ(cppboot::ErrnoToStatusCode(ENOENT), cppboot::StatusCode::kNotFound);

  // We'll pick a very large number so it hopefully doesn't collide to errno.
  EXPECT_EQ(cppboot::ErrnoToStatusCode(19980927),
            cppboot::StatusCode::kUnknown);
}

TEST(StatusErrno, ErrnoToStatus) {
  cppboot::Status status = cppboot::ErrnoToStatus(ENOENT, "Cannot open 'path'");
  EXPECT_EQ(status.code(), cppboot::StatusCode::kNotFound);
  EXPECT_EQ(status.message(), "Cannot open 'path': No such file or directory");
}
