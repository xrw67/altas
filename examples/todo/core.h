#ifndef TODO_CORE_H_
#define TODO_CORE_H_

#include <list>
#include <atomic>
#include <memory>

#include "cppboot/base/status.h"
#include "cppboot/base/string_view.h"
#include "cppboot/base/timestamp.h"

namespace todo {

struct Item {
  int id;
  std::string text;
  cppboot::Timestamp update_at;
};

typedef std::shared_ptr<Item> ItemPtr;
typedef std::list<ItemPtr> ItemList;

class Repository {
 public:
  virtual cppboot::Status LoadAllItems(ItemList* items, int* id_max) = 0;
  virtual cppboot::Status SaveAllItems(const ItemList& items) = 0;

 protected:
  virtual ~Repository() {}
};

Repository* CreateFileRepository(cppboot::string_view filename);

class Data {
 public:
  Data() : repository_(nullptr), next_id_(0) {}

  void set_repository(Repository* repository);

  cppboot::Status Add(cppboot::string_view text, int* id);
  cppboot::Status Delete(int id);
  cppboot::Status Update(int id, cppboot::string_view text);

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
