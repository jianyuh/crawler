#include "config.h"

#include "ul_conf.h"

Config::Config(char* filename)
{
    _load_fd = ul_initconf(1024);
    if (-1 == ul_readconf("./", filename, (ul_confdata_t*)_load_fd)) {
        ul_freeconf((ul_confdata_t*)_load_fd);
        _load_fd = NULL;
    }
    return;
}

Config::~Config()
{
    if (NULL != _load_fd) {
        ul_freeconf((ul_confdata_t*)_load_fd);
    }
    return;
}

bool Config::isload()
{
    if (NULL == _load_fd) {
        return false;
    }
    return true;
}

char* Config::getstr(char* key)
{
    _buffer[0] = '\0';
    if (1 == ul_getconfnstr((ul_confdata_t*)_load_fd, key, _buffer, MAXVALUELEN)) {
        return _buffer;
    }
    return NULL;
}

int Config::getint(char* key)
{
    int value;
    if (1 == ul_getconfint((ul_confdata_t*)_load_fd, key, &value)) {
        return value;
    }
    return -1;
}
