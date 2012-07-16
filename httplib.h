#ifndef _HTTPLIB_H_
#define _HTTPLIB_H_

#define BUFFERSIZE 2048

class Http_handle
{
private:
    char *host, *path;
    int port, fd;
    char buffer[BUFFERSIZE];
    int sock_connect();
public:
    Http_handle(char* host, int port, char* path);
    ~Http_handle();
    int request();
    void print_abstract();
};

#endif /* _HTTPLIB_H_ */
