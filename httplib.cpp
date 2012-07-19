#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include "httplib.h"

static char* empty = "";
static char* format = "GET /%s HTTP/1.1\r\nAccept: html/text\r\nHost: %s:%d\r\nUser-Agent: Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1\r\nConnection: Close\r\n\r\n";

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
    html = buffer;
    return;
}

Http_handle::Http_handle(char* url)
{
    this->url = strdup(url);
    return;
}

Http_handle::~Http_handle()
{
    if (NULL != url)
        free(url);
    return;
}

void Http_handle::reset(char* url)
{
    if (NULL != this->url)
        free(this->url);
    this->url = strdup(url);
    return;
}

void Http_handle::reset(char* host, int port, char* path)
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
    html = buffer;
    return;
}

int Http_handle::get_socket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return sockfd;
}

int Http_handle::socket_connect()
{
    struct hostent *hp;
    hp = gethostbyname(host);
    if (hp == NULL)
        return -1;
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = *((struct in_addr*)hp->h_addr);
    addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1)
        return -1;
    return 0;
}

int Http_handle::request()
{
    sprintf(buffer, format, path, host, port);
    
    int len = 0;
    len = send(sockfd, (void*)buffer, strlen(buffer), 0);
    len = recv(sockfd, (void*)buffer, BUFFERSIZE, 0);
    rec = clen = len;
    return 0;
}

void Http_handle::socket_close()
{
    close(sockfd);
    return;
}

void Http_handle::print_abstract()
{
    sep();
    printf("Code : %d\n", get_code());
    printf("Content-Length: %d\n", get_clen());
}

void Http_handle::sep()
{
    int count = 0;
    char *p = buffer;
    while (count < rec)
    {
        p++;
        count++;
        if (*p == '\n')
            if (*(p+1) == '\r' || *(p+1) == '\n')
                break;
    }
    while (*p == '\r' || *p == '\n')
        p++;
    html = p;
    hlen = p - buffer;
    clen = rec - hlen;
    return;
}

char* Http_handle::read()
{
    return html;
}

int Http_handle::get_clen()
{
    int count = 0;
    int len = 0;
    char *p = buffer;
    while (true)
    {
        count++;
        if (count > hlen)
            return clen;
        if (*p != 'C')
        {
            p++;
            continue;
        }
        if (strncmp(p, "Content-Length", 14) != 0)
        {
            p++;
            continue;
        }
        else
            break;
    }
    p += 15;
    while (*p == ' ') p++;
    while (*p != '\r' && *p != '\n' && *p != ' ')
    {
        len *= 10;
        len += *p - '0';
        p++;
    }
    return len;
}

int Http_handle::get_code()
{
    int code = 0;
    char *p = buffer;
    while (*p != ' ') p++;
    p++;
    while (*p != ' ')
    {
        code *= 10;
        code += *p - '0';
        p++;
    }
    return code;
}
