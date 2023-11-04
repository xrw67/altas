#ifndef TODO_UI_H_
#define TODO_UI_H_

#include "cppboot/html/document.h"

std::string do_list(void);

class HtmlUi {
 public:
  HtmlUi() : doc_("My TODO") {
    AddCommandBox();
  }

  const cppboot::html::Document& doc() {
    // 收尾
    auto body = doc_.body();
    if (!cmd_.empty())
      body->AddChild(new cppboot::html::Element("p", "CMD: " + cmd_));

    body->AddChild(new cppboot::html::Element("p", "LIST:"));
    body->AddChild(new cppboot::html::Element("p", do_list()));
    return doc_;
  }

  void set_cmd(const std::string& cmd) { cmd_ = cmd; }

 private:
  void AddCommandBox() {
    auto body = doc_.body();
    auto form = body->AddChild(new cppboot::html::Form("/todo"));
    form->AddChild(new cppboot::html::Input("cmd", "请输入命令：", ""));
    form->AddChild(new cppboot::html::SubmitButton("执行"));

    body->AddChild(new cppboot::html::Element("hr"));
  }

  cppboot::html::Document doc_;
  std::string cmd_;
};

#endif  // TODO_UI_H_