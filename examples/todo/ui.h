#ifndef TODO_UI_H_
#define TODO_UI_H_

#include "bbt/html/document.h"

std::string do_list(void);

class HtmlUi {
 public:
  HtmlUi() : doc_("My TODO") {
    AddCommandBox();
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
  void AddCommandBox() {
    auto body = doc_.body();
    auto form = body->AddChild(new bbt::html::Form("/todo"));
    form->AddChild(new bbt::html::Input("cmd", "请输入命令：", ""));
    form->AddChild(new bbt::html::SubmitButton("执行"));

    body->AddChild(new bbt::html::Element("hr"));
  }

  bbt::html::Document doc_;
  std::string cmd_;
};

#endif  // TODO_UI_H_