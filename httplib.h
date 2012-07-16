#ifndef _HTTPLIB_H_
#define _HTTPLIB_H_

class Http_handle
{
private:
    char *host, *path;
    int port;
public:
    Http_handle(char* host, int port, char* path);
    ~Http_handle();
};

#endif /* _HTTPLIB_H_ */
