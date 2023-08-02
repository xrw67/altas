#include <string>

#include "bbt/base/fmt.h"
#include "bbt/base/status.h"
#include "bbt/base/str_util.h"
#include "bbt/http/server.h"
#include "bbt/html/document.h"
#include "bbt/appkit/args.h"

#include "core.h"

bbt::Status do_add(const char* text);
std::string do_list(void);
void do_delete(int id);
void do_modify(int id, const char* new_text);

class HtmlUi {
 public:
  HtmlUi() : doc_("My TODO") {
    // 命令行
    auto body = doc_.body();

    auto form = body->AddChild(new bbt::html::Form("/todo"));
    form->AddChild(new bbt::html::Input("cmd", "请输入命令：", ""));
    form->AddChild(new bbt::html::SubmitButton("执行"));

    body->AddChild(new bbt::html::Element("hr"));
  }

  const bbt::html::Document& doc() {
    // 收尾
    auto body = doc_.body();
    if (!cmd_.empty())
      body->AddChild(new bbt::html::Element("p", "CMD: " + cmd_));

    body->AddChild(new bbt::html::Element("p", "LIST:"));
    body->AddChild(new bbt::html::Element("p", do_list()));
    return doc_;
  }

  void set_cmd(const std::string& cmd) { cmd_ = cmd; }

 private:
  bbt::html::Document doc_;
  std::string cmd_;
};

void DoCommand(HtmlUi& ui, const std::string& cmdline) {
  auto sep = cmdline.find_first_of(' ');

  if (sep != std::string::npos) {
    auto cmd = cmdline.substr(0, sep);
    if (cmd == "add") {
      do_add(cmdline.substr(sep).c_str());
    } else if (cmd == "del") {
      do_delete(std::stoi(cmdline.substr(sep)));
    }
  }
}

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
  server.Handle("/todo", [](const Request& req, Response* resp) {
    HtmlUi ui;

    auto cmd = req.Param("cmd");
    if (!cmd.empty()) {
      ui.set_cmd(cmd);
      DoCommand(ui, cmd);
    }

    resp->WriteHtml(Response::ok, ui.doc());
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
      result += bbt::format("<p>{}\t{}</p>", item->id, item->text);
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
