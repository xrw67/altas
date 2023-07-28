#include "gmock/gmock.h"

#include "core.h"

namespace {

struct MockRepository : public todo::Repository {
  bbt::Status LoadAllItems(todo::ItemList* items, int* id_max) {
    return bbt::OkStatus();
  }

  bbt::Status SaveAllItems(const todo::ItemList& items) {
    return bbt::OkStatus();
  }
};

struct TodoDataTest : public ::testing::Test {
  MockRepository repo;
  todo::Data data;

  void SetUp()
  {
    data.set_repository(&repo);
  }

  void TearDown()
  {

  }
};

TEST_F(TodoDataTest, should_not_add_empty_item) {
  int id;
  ASSERT_EQ(data.Add("", &id).code(), bbt::StatusCode::kInvalidArgument);
}

TEST_F(TodoDataTest, should_add_normal_item) {
  int id = 0;
  ASSERT_TRUE(data.Add("My first item", &id));
  ASSERT_EQ(id, 1);
}

TEST_F(TodoDataTest, should_add_repeat_item) {
  int id1 = 0;
  int id2 = 0;
  ASSERT_TRUE(data.Add("My first item", &id1));
  ASSERT_TRUE(data.Add("My first item", &id2));
  ASSERT_EQ(id1, 1);
  ASSERT_EQ(id2, 2);
}

TEST_F(TodoDataTest, should_list_items) {
  int id1 = 0;
  int id2 = 0;
  ASSERT_TRUE(data.Add("My first item", &id1));
  ASSERT_TRUE(data.Add("My second item", &id2));

  auto items = data.items();
  ASSERT_EQ(items.size(), 2);

  auto it = items.begin();
  {
    auto item = *it;
    ASSERT_EQ(item->id, 1);
    ASSERT_EQ(item->text, "My first item");
    ASSERT_TRUE(item->update_at.IsValid());
  }
  {
    auto item = *++it;
    ASSERT_EQ(item->id, 2);
    ASSERT_EQ(item->text, "My second item");
    ASSERT_TRUE(item->update_at.IsValid());
  }
}

TEST_F(TodoDataTest, should_show_specified_item) {
  int id = 0;
  ASSERT_TRUE(data.Add("My first item", &id));

  auto item = data.Show(id);
  ASSERT_EQ(item->id, id);
  ASSERT_EQ(item->text, "My first item");

  item = data.Show(99999);
  ASSERT_FALSE(item);
}

TEST_F(TodoDataTest, should_delete_item) {
  int id1 = 0;
  int id2 = 0;
  ASSERT_TRUE(data.Add("My first item", &id1));
  ASSERT_TRUE(data.Add("My first item", &id2));

  ASSERT_TRUE(data.Delete(id2));
  ASSERT_EQ(data.Delete(id2).code(), bbt::StatusCode::kNotFound);
  ASSERT_EQ(data.items().size(), 1);

  ASSERT_TRUE(data.Delete(id1));
  ASSERT_EQ(data.Delete(id1).code(), bbt::StatusCode::kNotFound);
  ASSERT_EQ(data.items().size(), 0);
}

TEST_F(TodoDataTest, should_update_item) {
  int id = 0;
  ASSERT_TRUE(data.Add("My first item", &id));

  ASSERT_EQ(data.Update(id, "").code(), bbt::StatusCode::kInvalidArgument);
  ASSERT_TRUE(data.Update(id, "My second item"));
  ASSERT_TRUE(data.Update(id, "My third item"));
  ASSERT_EQ(data.Update(999, "My four item").code(),
            bbt::StatusCode::kNotFound);

  auto item = data.Show(id);
  ASSERT_EQ(item->text, "My third item");
}

}  // namespace