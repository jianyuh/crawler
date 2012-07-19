#ifndef _HTTPLIB_H_
#define _HTTPLIB_H_

#define BUFFERSIZE 2048

class Http_handle
{
public:
    Http_handle(char* host, int port, char* path);
    Http_handle(char* url);
    ~Http_handle();
    void reset(char* host, int port, char* path);
    void reset(char* url);
    int get_socket();
    int socket_connect();
    int request();
    void socket_close();
    void print_abstract();
    char* read();
private:
    char* url;
    char *host, *path;
    int port, sockfd, clen, hlen, rec;
    char* html;
    char buffer[BUFFERSIZE];
    int sock_connect();
    void sep();
    int get_clen();
    int get_code();
};

#endif /* _HTTPLIB_H_ */
