#include <iostream>
#include <string>
#include "bbt/base/fmt.h"
#include "bbt/base/status.h"

#include "bbt/http/server/file_server.h"
#include "bbt/http/server.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << argv[0] << " [port] [root dir]" << std::endl;
    return -1;
  }

  bbt::http::FileServer fileserver(argv[2]);
  bbt::http::Server server;
  server.Handle("/",
                [&](const bbt::http::Request& req, bbt::http::Response* resp) {
                  fileserver.ServeHttp(req, resp);
                });

  auto st = server.Listen("0.0.0.0", argv[1]);
  if (!st) {
    bbt::println("{}", st);
    return -1;
  }
  server.Serve();
  return 0;
}