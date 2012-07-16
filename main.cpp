#include <cstdio>
#include <cstring>
#include "Spliter.h"
#include "httplib.h"


int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        printf("Need an url!\n");
        return 0;
    }

    Spliter spliter(argv[1]);
    spliter.exec();
    spliter.print();

    // TODO: tolower
    if (spliter.get_proto() && strcmp(spliter.get_proto(), "http") != 0)
    {
        printf("Sorry, I can only deal http.\n");
        return 0;
    }
    
    printf("%s\n", spliter.get_domin());
    Http_handle http_handle(spliter.get_domin(), spliter.get_port(), spliter.get_path());
    http_handle.request();
    http_handle.print_html();
    return 0;
}








