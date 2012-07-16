#include "httplib.h"

Http_handle::Http_handle(char* host, int port, char* path)
{
    this->host = host;
    this->port = port;
    this->path = path;
    return;
}

Http_handle::~Http_handle()
{
}
