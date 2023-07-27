#include "core.h"

namespace todo {

bbt::Status Data::Add(bbt::string_view text, int* id) {
  if (text.empty()) return bbt::InvalidArgumentError("empty item");

  int new_id = GetNextID();

  auto st = Insert(new_id, text);
  if (st && id) *id = new_id;
  return st;
}

bbt::Status Data::Delete(int id) {
  for (auto it = items_.begin(); it != items_.end(); it++) {
    auto item = *it;
    if (item->id == id) {
      items_.erase(it);
      return bbt::OkStatus();
    }
  }
  return bbt::NotFoundError("not found");
}

bbt::Status Data::Update(int id, bbt::string_view text) {
  if (text.empty()) return bbt::InvalidArgumentError("empty item");

  auto item = Find(id);
  if (item) {
    item->text = text.str();
    return bbt::OkStatus();
  }

  return bbt::NotFoundError("not found");
}

const Data::ItemPtr Data::Show(int id) const {
  for (auto i : items_) {
    if (i->id == id) return i;
  }
  return {};
}

int Data::GetNextID() {
  int id = next_id_.fetch_add(1);
  if (id == 0) id = next_id_.fetch_add(1);

  return id;
}

Data::ItemPtr Data::Find(int id) {
  for (auto it = items_.begin(); it != items_.end(); it++) {
    auto item = *it;
    if (item->id == id) {
      return item;
    }
  }
  return {};
}

bbt::Status Data::Insert(int id, bbt::string_view text) {
  ItemPtr item(new Item());
  item->id = id;
  item->text = text.str();
  item->update_at = bbt::Timestamp::Now();

  items_.push_back(item);

  return bbt::OkStatus();
}

}  // namespace todo