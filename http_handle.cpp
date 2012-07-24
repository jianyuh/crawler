#include "http_handle.h"

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ul_url.h"
#include "uln_http.h"

Http_handle::Http_handle()
{
    maxsize = 2048;
    timeout = 5000;
    host[0] = '\0';
    port[0] = '\0';
    path[0] = '\0';
    ip[0] = '\0';
    res = NULL;
    url = NULL;
    return;
}

Http_handle::~Http_handle()
{
    uln_http_freeres((uln_response_t*)res);
    return;
}

int Http_handle::reset(char* url)
{
    host[0] = '\0';
    port[0] = '\0';
    path[0] = '\0';
    ip[0] = '\0';
    this->url = url;
    if (NULL != res) {
        uln_http_freeres((uln_response_t*)res);
    }
    res = uln_http_initres();
    if (NULL == res) {
        return -1;
    }
    return 0;
}

int Http_handle::doit()
{
    if (0 != url_parse()) {
        return -1;                      // -1:URL ERROR
    }
    if (0 != get_ip()) {
        return -2;                      // -2:GET IP ERROR
    }
    if (uln_http_crawlpage(url, ip, maxsize, (uln_response_t*)res, timeout, 0) != 1)
    {
        return -3;                      // -3:CRAWL ERROR
    }
    return 0;
}

int Http_handle::url_parse()
{
    if (1 != ul_parse_url(url, host, port, path)) {
        return -1;
    }
    return 0;
}

int Http_handle::get_ip()
{
    struct hostent *hp;
    hp = gethostbyname(host);
    if (NULL == hp)
        return -1;
    sprintf(ip, "%s\n", inet_ntoa(*((struct in_addr*)hp->h_addr)));
    return 0;
}

void Http_handle::set_timeout(int timeout)
{
    this->timeout = timeout;
    return;
}

int Http_handle::set_maxsize(int maxsize)
{
    this->maxsize = maxsize;
    return 0;
}

int Http_handle::get_code()
{
    if (NULL == res)
    {
        return -1;
    }
    return ((uln_response_t*)res)->m_status_code;
}

int Http_handle::get_len()
{
    if (NULL == res)
    {
        return -1;
    }
    return ((uln_response_t*)res)->m_len;
}

char* Http_handle::get_content()
{
    if (NULL == res)
    {
        return NULL;
    }
    return ((uln_response_t*)res)->m_buf;
}
