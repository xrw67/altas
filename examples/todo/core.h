#ifndef TODO_CORE_H_
#define TODO_CORE_H_

#include <list>
#include <atomic>
#include <memory>

#include "bbt/base/status.h"
#include "bbt/base/string_view.h"
#include "bbt/base/timestamp.h"

namespace todo {

class Data {
 public:
  struct Item {
    int id;
    std::string text;
    bbt::Timestamp update_at;
  };

typedef std::shared_ptr<Item> ItemPtr;
  typedef std::list<ItemPtr> ItemList;

  Data() : next_id_(0) {}
  bbt::Status Add(bbt::string_view text, int* id);
  bbt::Status Delete(int id);
  bbt::Status Update(int id, bbt::string_view text);

  const ItemPtr Show(int id) const;
  const ItemList& items() const { return items_; };

 private:
  int GetNextID();
  ItemPtr Find(int id);
  bbt::Status Insert(int id, bbt::string_view text);

  std::atomic_int next_id_;
  ItemList items_;
};

}  // namespace todo

#endif
