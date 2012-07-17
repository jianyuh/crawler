#ifndef _HTTPLIB_H_
#define _HTTPLIB_H_

#define BUFFERSIZE 2048

class Http_handle
{
private:
    char *host, *path;
    int port, sockfd, clen, hlen, rec;
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
    int get_socket();
    int socket_connect();
    int request();
    void socket_close();
    void print_abstract();
    char* read();
};

#endif /* _HTTPLIB_H_ */
