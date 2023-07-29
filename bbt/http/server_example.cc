#include "bbt/http/server.h"
#include "bbt/http/request.h"
#include "bbt/http/response.h"
#include "bbt/base/fmt.h"

int main(int argc, char* argv[]) {
  // if (argc < 2) {
  //   printf("Usage: %s 8080\n", argv[0]);
  //   return -1;
  // }

  bbt::http::Server server;

  server.Handle("/", [](const bbt::http::Request& req, bbt::http::Response* rep) {
    
  });

  auto st = server.ListenAndServe("0.0.0.0", "12345");
  if (!st) {
    bbt::println("{}", st);
  }

  bbt::println("exit");
  return 0;
}
