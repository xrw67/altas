#include <iostream>

#include "cppboot/adv/args.h"

int main(int argc, char* argv[]) {
  cppboot::Args args;

  args.AddBool('d', "debug", "Enable debug mode");
  args.AddBool('v', "version", "Print version information and quit");
  args.AddString('c', "config", "/etc/myapp.ini",
                 "Location of client config files");
  args.AddString('r', "remote", "127.0.0.1", "Remote address");
  args.AddLong('l', "listen", 8080, "Listen port");

  auto st = args.Parse(argc, argv);
  if (!st) {
    std::cout << "Error: " << st.ToString() << std::endl;
    std::cout << args.Help() << std::endl;
    return -1;
  }

  if (args.GetBool("help")) {
    std::cout << args.Help() << std::endl;
    return 0;
  }

  if (args.GetBool("version")) {
    std::cout << "Version: 1.2.3" << std::endl;
    return 0;
  }

  std::cout << "Config: " << args.GetString("config") << std::endl;
  std::cout << "Remote: " << args.GetString("remote") << std::endl;
  std::cout << "Listen: " << args.GetLong("listen") << std::endl;

  return 0;
}