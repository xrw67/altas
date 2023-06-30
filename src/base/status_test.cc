#include "bbt/base/status.h"

#include <utility>

#include <gmock/gmock.h>

using namespace bbt;

TEST(Status, MoveConstructor) {
  {
    Status ok = Status::Ok();
    Status ok2 = std::move(ok);

    ASSERT_TRUE(ok2.ok());
  }

  {
    Status status = Status::NotFound("custom NotFound status message");
    Status status2 = std::move(status);

    ASSERT_TRUE(status2.IsNotFound());
    ASSERT_EQ("NotFound: custom NotFound status message", status2.ToString());
  }

  {
    Status self_moved = Status::IOError("custom IOError status message");

    // Needed to bypass compiler warning about explicit move-assignment.
    Status& self_moved_reference = self_moved;
    self_moved_reference = std::move(self_moved);
  }
}

TEST(Status, Update) {
  {
    Status st = Status::Ok();
    st.Update(Status::NotFound("msg1"));
    ASSERT_EQ("NotFound: msg1", st.ToString());

    st.Update(Status::InvalidArgument("msg2"));
    ASSERT_EQ("NotFound: msg1", st.ToString());
  }

  // Copy
  {
    Status st = Status::Ok();
    const Status err = Status::NotFound("msg");
    st.Update(err);
    ASSERT_EQ(st.ToString(), err.ToString());
  }

  // Move
  {
    Status st = Status::Ok();
    Status err = Status::NotFound("msg");
    st.Update(std::move(err));
    ASSERT_EQ("NotFound: msg", st.ToString());
    ASSERT_TRUE(err.ok());
  }
}