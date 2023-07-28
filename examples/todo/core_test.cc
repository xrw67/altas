#include "gmock/gmock.h"

#include "core.h"

namespace {

struct TodoDataTest : public ::testing::Test {
  todo::Data data;

  void SetUp()
  {
  }

  void TearDown()
  {
  }
};

TEST_F(TodoDataTest, should_not_add_empty_item) {
  int id;
  EXPECT_EQ(data.Add("", &id).code(), bbt::StatusCode::kInvalidArgument);
}

TEST_F(TodoDataTest, should_add_normal_item) {
  int id = 0;
  EXPECT_TRUE(data.Add("My first item", &id));
  EXPECT_EQ(id, 1);
}

TEST_F(TodoDataTest, should_add_repeat_item) {
  int id1 = 0;
  int id2 = 0;
  EXPECT_TRUE(data.Add("My first item", &id1));
  EXPECT_TRUE(data.Add("My first item", &id2));
  EXPECT_EQ(id1, 1);
  EXPECT_EQ(id2, 2);
}

TEST_F(TodoDataTest, should_list_items) {
  int id1 = 0;
  int id2 = 0;
  EXPECT_TRUE(data.Add("My first item", &id1));
  EXPECT_TRUE(data.Add("My second item", &id2));

  auto items = data.items();
  EXPECT_EQ(items.size(), 2);

  auto it = items.begin();
  {
    auto item = *it;
    EXPECT_EQ(item->id, 1);
    EXPECT_EQ(item->text, "My first item");
    EXPECT_TRUE(item->update_at.IsValid());
  }
  {
    auto item = *++it;
    EXPECT_EQ(item->id, 2);
    EXPECT_EQ(item->text, "My second item");
    EXPECT_TRUE(item->update_at.IsValid());
  }
}

TEST_F(TodoDataTest, should_show_specified_item) {
  int id = 0;
  EXPECT_TRUE(data.Add("My first item", &id));

  auto item = data.Show(id);
  EXPECT_EQ(item->id, id);
  EXPECT_EQ(item->text, "My first item");

  item = data.Show(99999);
  EXPECT_FALSE(item);
}

TEST_F(TodoDataTest, should_delete_item) {
  int id1 = 0;
  int id2 = 0;
  EXPECT_TRUE(data.Add("My first item", &id1));
  EXPECT_TRUE(data.Add("My first item", &id2));

  EXPECT_TRUE(data.Delete(id2));
  EXPECT_EQ(data.Delete(id2).code(), bbt::StatusCode::kNotFound);
  EXPECT_EQ(data.items().size(), 1);

  EXPECT_TRUE(data.Delete(id1));
  EXPECT_EQ(data.Delete(id1).code(), bbt::StatusCode::kNotFound);
  EXPECT_EQ(data.items().size(), 0);
}

TEST_F(TodoDataTest, should_update_item) {
  int id = 0;
  EXPECT_TRUE(data.Add("My first item", &id));

  EXPECT_EQ(data.Update(id, "").code(), bbt::StatusCode::kInvalidArgument);
  EXPECT_TRUE(data.Update(id, "My second item"));
  EXPECT_TRUE(data.Update(id, "My third item"));
  EXPECT_EQ(data.Update(999, "My four item").code(),
            bbt::StatusCode::kNotFound);

  auto item = data.Show(id);
  EXPECT_EQ(item->text, "My third item");
}

}  // namespace