#include <cstdio>
#include <cstring>
#include "Spliter.h"
#include "httplib.h"
#include <pthread.h>
#include <unistd.h>


void* fetch(void* arg)
{
    char* url = (char*)arg;

    // TODO: url fix
    Spliter spliter(url);
    spliter.exec();

    // TODO: tolower
    if (spliter.get_proto() && strcmp(spliter.get_proto(), "http") != 0)
    {
        spliter.print();
        printf("Sorry, I can only deal http.\n");
        return NULL;
    }
    
    Http_handle http_handle(spliter.get_domin(), spliter.get_port(), spliter.get_path());
    spliter.print();
    printf("\n");
    http_handle.request();
    printf("<<----------------------\n");
    return NULL;
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        // fetch((void*)argv[i]);
        pthread_t id;
        pthread_create(&id, NULL, fetch, (void*)argv[i]);
    }
    sleep(2);
    return 0;
}

