#ifndef TODO_CORE_H_
#define TODO_CORE_H_

#include <list>
#include <atomic>
#include <memory>

#include "bbt/base/status.h"
#include "bbt/base/string_view.h"
#include "bbt/base/timestamp.h"

namespace todo {

struct Item {
  int id;
  std::string text;
  bbt::Timestamp update_at;
};

typedef std::shared_ptr<Item> ItemPtr;
typedef std::list<ItemPtr> ItemList;

class Repository {
 public:
  virtual bbt::Status LoadAllItems(ItemList* items, int* id_max) = 0;
  virtual bbt::Status SaveAllItems(const ItemList& items) = 0;

 protected:
  virtual ~Repository() {}
};

Repository* CreateFileRepository(bbt::string_view filename);

class Data {
 public:
  Data() : next_id_(0) {}

  void set_repository(Repository* repository);

  bbt::Status Add(bbt::string_view text, int* id);
  bbt::Status Delete(int id);
  bbt::Status Update(int id, bbt::string_view text);

  const ItemPtr Show(int id) const;
  const ItemList& items() const { return items_; };

 private:
  int GetNextID();
  ItemPtr Find(int id);

  Repository* repository_;
  std::atomic_int next_id_;
  ItemList items_;
};

}  // namespace todo

#endif
