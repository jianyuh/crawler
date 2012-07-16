#include "httplib.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

static char* empty = "";
static char* format = "GET /%s HTTP/1.1\r\nAccept: html/text\r\nHost: %s:%d\r\nUser-Agent: Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1\r\nConnection: Close\r\n\r\n\0";

Http_handle::Http_handle(char* host, int port, char* path)
{
    if (host)
        this->host = host;
    else
        this->host = empty;

    if (port == 0)
        this->port = 80;
    else
        this->port = port;

    if (path)
        this->path = path;
    else
        this->path = empty;
    return;
}

Http_handle::~Http_handle()
{
}

int Http_handle::sock_connect()
{
    struct hostent *hp;
    hp = gethostbyname(host);
    if (hp == NULL)
    {
        printf("Error while gethostbyname\n");
        exit(-1);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error while create socket\n");
        exit(-1);
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = *((struct in_addr*)hp->h_addr);
    addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1)
    {
        printf("Error while connect!\n");
        exit(-1);
    }
    return sockfd;
}

int Http_handle::request()
{
    fd = sock_connect();
    sprintf(buffer, format, path, host, port);
    
    int len = 0;
    len = send(fd, (void*)buffer, strlen(buffer), 0);
    len = recv(fd, (void*)buffer, BUFFERSIZE, 0);
    close(fd);
}

void Http_handle::print_abstract()
{
    // TODO
    printf("%s\n", buffer);
}
