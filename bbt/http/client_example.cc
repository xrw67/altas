#include "bbt/http/client.h"
#include "bbt/base/fmt.h"
#include "bbt/http/response.h"

int main(int argc, char *argv[]) {

    bbt::http::Response reply;

    auto st = bbt::http::Get(argv[1], "http", &reply);



    return 0;
}