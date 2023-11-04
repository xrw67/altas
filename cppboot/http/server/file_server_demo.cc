#include <iostream>
#include <string>
#include "cppboot/base/fmt.h"
#include "cppboot/base/status.h"

#include "cppboot/http/server/file_server.h"
#include "cppboot/http/server.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << argv[0] << " [port] [root dir]" << std::endl;
    return -1;
  }

  cppboot::http::FileServer fileserver(argv[2]);
  cppboot::http::Server server;
  server.Handle("/",
                [&](const cppboot::http::Request& req, cppboot::http::Response* resp) {
                  fileserver.ServeHttp(req, resp);
                });

  auto st = server.Listen("0.0.0.0", argv[1]);
  if (!st) {
    cppboot::println("{}", st);
    return -1;
  }
  server.Serve();
  return 0;
}