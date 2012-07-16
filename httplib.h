#ifndef _HTTPLIB_H_
#define _HTTPLIB_H_

#define BUFFERSIZE 2048

class Http_handle
{
private:
    char *host, *path;
    int port, fd, clen;
    char* html;
    char buffer[BUFFERSIZE];
    int sock_connect();
    void sep();
    int get_clen();
    int get_code();
public:
    Http_handle(char* host, int port, char* path);
    ~Http_handle();
    void reset(char* host, int port, char* path);
    int request();
    void print_abstract();
    char* read();
};

#endif /* _HTTPLIB_H_ */
