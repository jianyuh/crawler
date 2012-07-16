#include <cstdio>
#include "Spliter.h"


int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        printf("need a url!\n");
        return 0;
    }

    Spliter spliter(argv[1]);
    spliter.exec();

    return 0;
}
