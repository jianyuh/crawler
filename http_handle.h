#ifndef _HTTPLIB_H_
#define _HTTPLIB_H_

class Http_handle
{
public:
    Http_handle();
    ~Http_handle();
    int reset(char* url);
    int doit();
    void set_timeout(int timeout);
    int set_maxsize(int maxsize);
    int get_code();
    int get_len();
    char* get_content();
private:
    int url_parse();
    int get_ip();
    int timeout;
    int maxsize;
    char* url;
    char host[255];
    char port[10];
    char path[255];
    char ip[16];
    void* res;
};

#endif /* _HTTPLIB_H_ */
