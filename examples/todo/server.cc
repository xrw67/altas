#include "bbt/appkit/args.h"

int main(int argc, char *argv[])
{
    bbt::Args args;

    args.AddString('a', "addr", "0.0.0.0", "Listen address");
    return 0;
}