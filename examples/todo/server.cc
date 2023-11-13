#include <string>

#include "cppboot/base/fmt.h"
#include "cppboot/base/status.h"
#include "cppboot/base/str_util.h"
#include "cppboot/http/server.h"

#include "cppboot/adv/args.h"

#include "core.h"
#include "ui.h"

cppboot::Status do_add(const char* text);
std::string do_list(void);
void do_delete(int id);
void do_modify(int id, const char* new_text);

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
  using cppboot::html::Element;
  using cppboot::http::Request;
  using cppboot::http::Response;

  cppboot::Args args;

  args.AddLong('p', "port", 12345, "Listen port");

  auto st = args.Parse(argc, argv);
  if (!st) {
    cppboot::println("{}", st);
  }

  cppboot::http::Server server;
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

cppboot::Status do_add(const char* text) {
  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    int id = 0;
    return data.Add(text, &id);
  }
  return cppboot::InvalidArgumentError("can't open respository");
}

std::string do_list(void) {
  std::string result;

  auto repo = todo::CreateFileRepository("/tmp/todo-data.json");
  if (repo) {
    todo::Data data;
    data.set_repository(repo);
    auto items = data.items();
    for (auto item : items) {
      result += cppboot::format("<p>{}\t{}</p>", item->id, item->text);
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
