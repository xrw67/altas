#include "gmock/gmock.h"
#include "cppboot/http/form_data.h"

namespace {

using cppboot::http::FormData;

TEST(FormData, Encode) {
  FormData f;

  f.Set("str-key", "value1");
  f.Set("int-key1", std::to_string(1001));
  f.Set("int-key2", std::to_string(-1001));
  f.Set("bool-key", "true");

  ASSERT_EQ(f.Encode(),
            "bool-key=true&int-key1=1001&int-key2=-1001&str-key=value1");
}

TEST(FormData, ParseQuery) {
  FormData f;

  auto st =
      f.Parse("bool-key=true&int-key1=1001&int-key2=-1001&str-key=value1");
  ASSERT_TRUE(st) << st.ToString();
  ASSERT_EQ(f.Get("str-key"), "value1");
  ASSERT_EQ(f.Get("int-key1"), "1001");
  ASSERT_EQ(f.Get("int-key2"), "-1001");
  ASSERT_EQ(f.Get("bool-key"), "true");
  ASSERT_EQ(f.Get("no-key"), "");
}

}  // namespace