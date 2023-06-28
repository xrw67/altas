#include "bbt/base/string_view.h"

#include <gmock/gmock.h>

using namespace bbt;

TEST(StringView, Ctor) {
  {
    // Null.
    string_view s10;
    EXPECT_TRUE(s10.data() == nullptr);
    EXPECT_EQ(0, s10.length());
  }

  {
    // const char* without length.
    const char* hello = "hello";
    string_view s20(hello);
    EXPECT_TRUE(s20.data() == hello);
    EXPECT_EQ(5, s20.length());

    // const char* with length.
    string_view s21(hello, 4);
    EXPECT_TRUE(s21.data() == hello);
    EXPECT_EQ(4, s21.length());

    // Not recommended, but valid C++
    string_view s22(hello, 6);
    EXPECT_TRUE(s22.data() == hello);
    EXPECT_EQ(6, s22.length());
  }

  {
    // std::string.
    std::string hola = "hola";
    string_view s30(hola);
    EXPECT_TRUE(s30.data() == hola.data());
    EXPECT_EQ(4, s30.length());

    // std::string with embedded '\0'.
    hola.push_back('\0');
    hola.append("h2");
    hola.push_back('\0');
    string_view s31(hola);
    EXPECT_TRUE(s31.data() == hola.data());
    EXPECT_EQ(8, s31.length());
  }
}

TEST(StringViewTest, Swap) {
  string_view a("a");
  string_view b("bbb");
  EXPECT_TRUE(noexcept(a.swap(b)));
  a.swap(b);
  EXPECT_EQ(a, "bbb");
  EXPECT_EQ(b, "a");
  a.swap(b);
  EXPECT_EQ(a, "a");
  EXPECT_EQ(b, "bbb");
}

// TEST(StringViewTest, STLComparator) {
//   std::string s1("foo");
//   std::string s2("bar");
//   std::string s3("baz");

//   string_view p1(s1);
//   string_view p2(s2);
//   string_view p3(s3);

//   typedef std::map<string_view, int> TestMap;
//   TestMap map;

//   map.insert(std::make_pair(p1, 0));
//   map.insert(std::make_pair(p2, 1));
//   map.insert(std::make_pair(p3, 2));
//   EXPECT_EQ(map.size(), 3);

//   TestMap::const_iterator iter = map.begin();
//   EXPECT_EQ(iter->second, 1);
//   ++iter;
//   EXPECT_EQ(iter->second, 2);
//   ++iter;
//   EXPECT_EQ(iter->second, 0);
//   ++iter;
//   EXPECT_TRUE(iter == map.end());

//   TestMap::iterator new_iter = map.find("zot");
//   EXPECT_TRUE(new_iter == map.end());

//   new_iter = map.find("bar");
//   EXPECT_TRUE(new_iter != map.end());

//   map.erase(new_iter);
//   EXPECT_EQ(map.size(), 2);

//   iter = map.begin();
//   EXPECT_EQ(iter->second, 2);
//   ++iter;
//   EXPECT_EQ(iter->second, 0);
//   ++iter;
//   EXPECT_TRUE(iter == map.end());
// }
