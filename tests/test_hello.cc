#include "gmock/gmock.h"
#include "bbt/appkit/module.h"

using namespace bbt;

namespace {

struct DB {
  virtual void Query(const char* sql) = 0;
};

struct MockDB : public DB {
  MOCK_METHOD(void, Query, (const char* sql), (override));
};

struct DBClient {
  DB* db;

  explicit DBClient(DB* db) : db(db) {}

  void Do() { db->Query("SELECT * FROM t1;"); }
};

using ::testing::AtLeast;

TEST(MockTest, Example) {
  MockDB mock;
  DBClient client(&mock);

  EXPECT_CALL(mock, Query("SELECT * FROM t1;")).Times(AtLeast(1));

  client.Do();
}

}  // namespace
