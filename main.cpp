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
    spliter.print();

    printf("%d\n", spliter.get_port());

    return 0;
}
