#include "bbt/base/fmt.h"
#include "bbt/http/server.h"
#include "bbt/appkit/args.h"

int main(int argc, char *argv[])
{
    bbt::Args args;

    args.AddString('l', "listen", "0.0.0.0:13500", "Listen address");

    auto st = args.Parse(argc, argv);
    if (!st) {
        bbt::println("{}", st);
    }

    bbt::http::Server server;




    return 0;
}