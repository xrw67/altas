#include "bbt/appkit/args.h"

#include "gmock/gmock.h"

namespace {

TEST(Args, BoolFlag) {
  {
    bbt::Args args;
    args.AddBool('d', "debug", "Enable debug mode");

    ASSERT_FALSE(args.GetBool("debug")) << "Default";

    // Happy
    ASSERT_TRUE(args.Parse("-d").ok());
    ASSERT_TRUE(args.GetBool("debug"));

    ASSERT_TRUE(args.Parse("--debug").ok());
    ASSERT_TRUE(args.GetBool("debug"));

    ASSERT_TRUE(args.Parse("-d --debug -d -d").ok());
    ASSERT_TRUE(args.GetBool("debug"));

    // Not found
    ASSERT_FALSE(args.GetBool("debug1"));

    // Bad
    ASSERT_TRUE(args.Parse("").ok());
    ASSERT_FALSE(args.GetBool("debug"));

    ASSERT_FALSE(args.Parse("-d true").ok());

    ASSERT_FALSE(args.GetBool(NULL));
    ASSERT_FALSE(args.GetBool(""));
  }
}

TEST(Args, StringFlag) {
  bbt::Args args;
  args.AddString('f', "file", "1.txt", "Setup file path");

  ASSERT_EQ(args.GetString("file"), "1.txt") << "Default path";

  // Happy
  ASSERT_TRUE(args.Parse("-f 2.txt").ok());
  ASSERT_EQ(args.GetString("file"), "2.txt");

  ASSERT_TRUE(args.Parse("--file 3.txt").ok());
  ASSERT_EQ(args.GetString("file"), "3.txt");

  // Repeat
  ASSERT_TRUE(args.Parse("-f 1.txt --file 2.txt -f 3.txt").ok());
  ASSERT_EQ(args.GetString("file"), "3.txt");

  // Bad
  ASSERT_FALSE(args.Parse("-file 2.txt").ok());
  ASSERT_FALSE(args.Parse("-f").ok());
  ASSERT_FALSE(args.Parse("--file").ok());

  ASSERT_FALSE(args.Parse("--file 2.txt 3.txt").ok());
}

TEST(Args, LongFlag) {
  bbt::Args args;
  args.AddLong('p', "port", 80, "Listen port");

  ASSERT_EQ(args.GetLong("port"), 80) << "Default path";

  // Happy
  ASSERT_TRUE(args.Parse("-p 443").ok());
  ASSERT_EQ(args.GetLong("port"), 443);

  ASSERT_TRUE(args.Parse("--port 4433").ok());
  ASSERT_EQ(args.GetLong("port"), 4433);

  // Repeat
  ASSERT_TRUE(args.Parse("-p 123 --port 234 -p 345").ok());
  ASSERT_EQ(args.GetLong("port"), 345);

  // Bad
  ASSERT_FALSE(args.Parse("-port 1234").ok());
  ASSERT_FALSE(args.Parse("-p").ok());
  ASSERT_FALSE(args.Parse("--port").ok());
}

TEST(Args, MultiFlags) {
  bbt::Args args;
  args.AddBool('d', "debug", "Enable debug mode");
  args.AddString('f', "file", "1.txt", "Setup file path");
  args.AddLong('p', "port", 80, "Listen port");

  // Happy
  ASSERT_TRUE(args.Parse("  -d  --file /path/1.txt -p 443  ").ok());
  ASSERT_TRUE(args.GetBool("debug"));
  ASSERT_EQ(args.GetString("file"), "/path/1.txt");
  ASSERT_EQ(args.GetLong("port"), 443);

  // Bad
  ASSERT_FALSE(args.Parse("  -a  --bcde /path/1.txt -p 443  ").ok());
  ASSERT_TRUE(args.Parse("  -d  --file /path/1.txt").ok());
  ASSERT_EQ(args.GetLong("port"), 80);

  ASSERT_FALSE(args.Parse("--d").ok());
  ASSERT_FALSE(args.Parse("-debug").ok());
  ASSERT_FALSE(args.Parse("-").ok());
  ASSERT_FALSE(args.Parse("--").ok());
}

TEST(Args, Argc_Argv) {
  bbt::Args args;
  args.AddBool('d', "debug", "Enable debug mode");
  args.AddString('f', "file", "1.txt", "Setup file path");
  args.AddLong('p', "port", 80, "Listen port");

  // Happy
  const char* const argv[] = {" -d ", " --file", "2.txt",
                              "-p",   "443",     NULL};
  ASSERT_TRUE(args.Parse(5, argv).ok());
  ASSERT_TRUE(args.GetBool("debug"));
  ASSERT_EQ(args.GetString("file"), "2.txt");
  ASSERT_EQ(args.GetLong("port"), 443);
}

}  // namespace