#include "core.h"

#include <fstream>

#include "cppboot/base/json.h"
#include "cppboot/base/fs.h"

namespace todo {

void Data::set_repository(Repository* repository) {
  repository_ = repository;
  if (repository_) {
    int id_max = 0;
    repository_->LoadAllItems(&items_, &id_max);
    next_id_ = id_max + 1;
  }
}

cppboot::Status Data::Add(cppboot::string_view text, int* id) {
  if (text.empty()) return cppboot::InvalidArgumentError("empty item");

  int new_id = GetNextID();

  ItemPtr item(new Item());
  item->id = new_id;
  item->text = text.str();
  item->update_at = cppboot::Timestamp::Now();
  items_.push_back(item);

  if (repository_) {
    auto st = repository_->SaveAllItems(items_);
    if (!st) return st;
  }

  if (id) *id = new_id;
  return cppboot::OkStatus();
}

cppboot::Status Data::Delete(int id) {
  for (auto it = items_.begin(); it != items_.end(); it++) {
    auto item = *it;
    if (item->id == id) {
      items_.erase(it);
      if (repository_) repository_->SaveAllItems(items_);
      return cppboot::OkStatus();
    }
  }
  return cppboot::NotFoundError("not found");
}

cppboot::Status Data::Update(int id, cppboot::string_view text) {
  if (text.empty()) return cppboot::InvalidArgumentError("empty item");

  auto item = Find(id);
  if (item) {
    item->text = text.str();
    if (repository_) repository_->SaveAllItems(items_);
    return cppboot::OkStatus();
  }

  return cppboot::NotFoundError("not found");
}

const ItemPtr Data::Show(int id) const {
  for (auto i : items_) {
    if (i->id == id) return i;
  }
  return {};
}

int Data::GetNextID() {
  int id = next_id_.fetch_add(1);
  if (id <= 0) id = next_id_.fetch_add(1);

  return id;
}

ItemPtr Data::Find(int id) {
  for (auto it = items_.begin(); it != items_.end(); it++) {
    auto item = *it;
    if (item->id == id) {
      return item;
    }
  }
  return {};
}

//
// FileRepository
//

struct FileRepository : public Repository {
  std::string filename_;
  FileRepository(cppboot::string_view filename) : filename_(filename.str()) {}
  ~FileRepository() {}

  cppboot::Status LoadAllItems(ItemList* items, int* id_max) {
    int max = 0;
    items->clear();

    std::ifstream f(filename_);
    if (!f.is_open()) return cppboot::InvalidArgumentError("file not found");

    cppboot::json j = cppboot::json::parse(f);
    if (j.is_object()) {
      auto j_items = j["items"];
      if (j_items.is_array()) {
        for (auto j_item : j_items) {
          ItemPtr p(new Item());
          p->id = j_item["id"];
          p->text = j_item["text"];
          p->update_at = cppboot::Timestamp::Now();
          items->push_back(p);

          if (max < p->id) max = p->id;
        }
      }
    }

    if (id_max) *id_max = max;
    return cppboot::OkStatus();
  }

  cppboot::Status SaveAllItems(const ItemList& items) {
    cppboot::json j_items;

    for (auto i : items) {
      cppboot::json j;

      j["id"] = i->id;
      j["text"] = i->text;
      j["update_at"] = i->update_at.ToFormattedString();
      j_items.push_back(j);
    }

    cppboot::json root;
    root["create"] = cppboot::Timestamp::Now().ToFormattedString();
    root["items"] = j_items;

    cppboot::WriteFile(filename_, root.dump(2, ' '));

    return cppboot::OkStatus();
  }
};

Repository* CreateFileRepository(cppboot::string_view filename) {
  return new FileRepository(filename);
}

}  // namespace todo