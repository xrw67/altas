#include <string>

#include "bbt/base/fmt.h"
#include "bbt/base/status.h"
#include "bbt/http/server.h"
#include "bbt/html/document.h"
#include "bbt/appkit/args.h"

#include "core.h"

bbt::Status do_add(const char* text);
std::string do_list(void);
std::string do_show(int id);
void do_delete(int id);
void do_modify(int id, const char* new_text);

int main(int argc, char* argv[]) {
  using bbt::html::Element;
  using bbt::http::Request;
  using bbt::http::Response;

  bbt::Args args;

  args.AddLong('p', "port", 12345, "Listen port");

  auto st = args.Parse(argc, argv);
  if (!st) {
    bbt::println("{}", st);
  }

  bbt::http::Server server;
  server.Handle("/", [](const Request& req, Response* resp) {
    bbt::html::Document doc("Todo Application");
    {
      auto body = doc.body();
      body->AddChild(new Element("h2", "Todo App"));
      body->AddChild(new Element("p", "This is a todo app^_^"));
      body->AddChild(
          new bbt::html::Link("Contect Me!", "https://www.baidu.com"));

      {
        auto table = new bbt::html::Table(3);
        body->AddChild(table);
        table->SetHead({"head1", "head2", "head3"});
        table->AddRow({"row1-1", "row1-2", "row1-3"});
        table->AddRow({"row2-1", "row2-2", "row2-3"});
        table->AddRow({"row3-1", "row3-2"});
      }

      {
        auto form = body->AddChild(new bbt::html::Form("/items/show"));
        form->AddChild(new bbt::html::Input("id", "Please input ID:", "0"));
        form->AddChild(new bbt::html::Button("提交"));
      }
    }
    resp->WriteHtml(Response::ok, doc);
  });

  server.Handle("/items/add", [](const Request& req, Response* resp) {
    if (req.method == "POST" && !req.content.empty()) {
      do_add(req.content.c_str());
      resp->WriteText(Response::ok, "OK");
    }
  });

  server.Handle("/items/list", [](const Request& req, Response* resp) {
    if (req.method == "GET") {
      resp->WriteText(Response::ok, do_list());
    }
  });

  server.Handle("/items/show", [](const Request& req, Response* resp) {
    if (req.method == "GET") {
      auto id = std::stoi(req.Param("id"));
      resp->WriteText(Response::ok, do_show(id));
    }
  });
  server.Handle("/items/delete", [](const Request& req, Response* resp) {
    if (req.method == "POST") {
      auto id = std::stoi(req.Param("id"));
      do_delete(id);
      resp->WriteText(Response::ok, "OK");
    }
  });
  server.Handle("/items/modify", [](const Request& req, Response* resp) {
    if (req.method == "POST") {
      auto id = std::stoi(req.Param("id"));
      auto new_text = req.Param("text");
      do_modify(id, new_text.c_str());
      resp->WriteText(Response::ok, "OK");
    }
  });

  auto port = std::to_string(args.GetLong("port"));

  server.Listen("0.0.0.0", port);
  server.Serve();
  return 0;
}

bbt::Status do_add(const char* text) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    int id = 0;
    return data.Add(text, &id);
  }
  return bbt::InvalidArgumentError("can't open respository");
}

std::string do_list(void) {
  std::string result;

  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto items = data.items();
    for (auto item : items) {
      result += bbt::format("{}\t{}\n", item->id, item->text);
    }
  }
  return result;
}

std::string do_show(int id) {
  std::string result;

  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto item = data.Show(id);
    if (item) {
      result += bbt::format("{}\t{}\n", item->id, item->text);
    } else {
      result += bbt::format("Not found\n");
    }
  }
  return result;
}

void do_delete(int id) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto st = data.Delete(id);
    bbt::println("{}", st);
  }
}

void do_modify(int id, const char* new_text) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto st = data.Update(id, new_text);
    if (st) {
      bbt::println("Update Done:");
      bbt::println("{}\t{}", id, new_text);
    }
    bbt::println("{}", st);
  }
}
