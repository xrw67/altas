#include "bbt/base/status.h"

#include <utility>

#include <gmock/gmock.h>

using namespace bbt;

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
    ASSERT_EQ("NotFound: custom NotFound status message", status2.ToString());
  }

  {
    Status self_moved = IOError("custom IOError status message");

    // Needed to bypass compiler warning about explicit move-assignment.
    Status& self_moved_reference = self_moved;
    self_moved_reference = std::move(self_moved);
  }
}

TEST(Status, Update) {
  {
    Status st = OkStatus();
    st.Update(NotFoundError("msg1"));
    ASSERT_EQ("NotFound: msg1", st.ToString());

    st.Update(InvalidArgumentError("msg2"));
    ASSERT_EQ("NotFound: msg1", st.ToString());
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
    ASSERT_EQ("NotFound: msg", st.ToString());
    ASSERT_TRUE(err.ok());
  }
}