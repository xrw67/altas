#include "gmock/gmock.h"
#include "cppboot/adv/module.h"

using namespace cppboot;

namespace {

struct DB {
  virtual int Query(const char* sql) = 0;
};

struct MockDB : public DB {
  MOCK_METHOD(int, Query, (const char* sql), (override));
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
