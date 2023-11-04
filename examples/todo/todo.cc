#include <stdio.h>
#include "core.h"

#include "cppboot/base/fmt.h"

void do_add(const char* text);
void do_list(void);
void do_show(int id);
void do_delete(int id);
void do_modify(int id, const char* new_text);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "invalid parameters!\n");
    return -1;
  }

  std::string cmd(argv[1]);
  if (cmd == "add") {
    do_add(argv[2]);
  } else if (cmd == "list") {
    do_list();
  } else if (cmd == "show") {
    do_show(atoi(argv[2]));
  } else if (cmd == "delete") {
    do_delete(atoi(argv[2]));
  } else if (cmd == "modify") {
    if (argc < 4) {
      fprintf(stderr, "invalid parameters!\n");
      return -1;
    }
    do_modify(atoi(argv[2]), argv[3]);
  }

  return 0;
}

void do_add(const char* text) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    int id = 0;
    auto st = data.Add(text, &id);
    if (st) {
      cppboot::println("Add Done:");
      cppboot::println("{}\t{}", id, text);
    }
    cppboot::println("{}", st);
  }
}

void do_list(void) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto items = data.items();
    for (auto item : items) {
      cppboot::println("{}\t{}", item->id, item->text);
    }
  }
}

void do_show(int id) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto item = data.Show(id);
    if (item) {
      cppboot::println("{}\t{}", item->id, item->text);
    } else {
      cppboot::println("Not found");
    }
  }
}

void do_delete(int id) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto st = data.Delete(id);
    cppboot::println("{}", st);
  }
}

void do_modify(int id, const char* new_text) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto st = data.Update(id, new_text);
    if (st) {
      cppboot::println("Update Done:");
      cppboot::println("{}\t{}", id, new_text);
    }
    cppboot::println("{}", st);
  }
}
