#include <cstdio>
#include <cstring>
#include "Spliter.h"
#include "httplib.h"
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* fetch(void* arg)
{
    char* url = (char*)arg;

    // TODO: url fix
    Spliter spliter(url);
    spliter.exec();

    // TODO: tolower
    if (spliter.get_proto() && strcmp(spliter.get_proto(), "http") != 0)
    {
        pthread_mutex_lock(&mutex);
        spliter.print();
        printf("Sorry, I can only deal http.\n");
        printf("<<----------------------\n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    
    Http_handle http_handle(spliter.get_domin(), spliter.get_port(), spliter.get_path());
    if (http_handle.request() == -1)
    {
        pthread_mutex_lock(&mutex);
        spliter.print();
        printf("connect error\n");
        printf("<<----------------------\n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    
    pthread_mutex_lock(&mutex);
    spliter.print();
    http_handle.print_abstract();
    printf("<<----------------------\n");
    pthread_mutex_unlock(&mutex);
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
    sleep(10);
    return 0;
}

