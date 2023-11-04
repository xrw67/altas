#include "gmock/gmock.h"

#include <vector>

#include "cppboot/base/json.h"
#include "cppboot/base/string_view.h"

namespace {

static const char* text =
    "{\n"
    "  \"answer\": {\n"
    "    \"everything\": 42\n"
    "  },\n"
    "  \"happy\": true,\n"
    "  \"list\": [\n"
    "    1,\n"
    "    0,\n"
    "    2\n"
    "  ],\n"
    "  \"name\": \"Niels\",\n"
    "  \"nothing\": null,\n"
    "  \"object\": {\n"
    "    \"currency\": \"USD\",\n"
    "    \"value\": 42.99\n"
    "  },\n"
    "  \"pi\": 3.141\n"
    "}";

TEST(Json, Parse) {
  cppboot::json j = cppboot::json::parse(text);

  ASSERT_TRUE(j.is_object());

  ASSERT_EQ(j["pi"], 3.141);
  ASSERT_EQ(j["happy"], true);
  ASSERT_EQ(j["name"], "Niels");
  ASSERT_EQ(j["nothing"], nullptr);
  ASSERT_EQ(j["answer"]["everything"], 42);

  ASSERT_TRUE(j.contains("list"));
  const auto& j_list = j["list"];
  ASSERT_TRUE(j_list.is_array());
  ASSERT_EQ(j_list[0], 1);
  ASSERT_EQ(j_list[1], 0);
  ASSERT_EQ(j_list[2], 2);

  ASSERT_EQ(j["object"]["currency"], "USD");
  ASSERT_EQ(j["object"]["value"], 42.99);
}

TEST(Json, Serializer) {
  cppboot::json j;

  j["pi"] = 3.141;
  j["happy"] = true;
  j["name"] = "Niels";
  j["nothing"] = nullptr;
  j["answer"]["everything"] = 42;
  j["list"] = std::vector<int>({1, 0, 2});
  j["object"]["currency"] = "USD";
  j["object"]["value"] = 42.99;

  std::string serialized = j.dump(2, ' ');
  ASSERT_EQ(serialized, text);
}

TEST(Json, StingView) {
  cppboot::json j;

  const char* val = "value666";
  j["name"] = cppboot::string_view(val, 5);
  std::string serialized = j.dump();
  ASSERT_EQ(serialized, "{\"name\":\"value\"}");
}

}  // namespace