#include "Spliter.h"
#include <cstdio>

Spliter::Spliter(char* url)
{
    this->url = url;
    proto = domin = port = path = pos = 0;
    has_proto = has_domin = has_port = has_path = false;
    state = PROTO;
    return;
}

void Spliter::reset(char* url)
{
    this->url = url;
    proto = domin = port = path = pos = 0;
    has_proto = has_domin = has_port = has_path = false;
    state = PROTO;
    return;
}

Spliter::~Spliter()
{
}

void Spliter::exec()
{
    while (state != END)
        pos = handle(pos);
}

void Spliter::print()
{
    if (has_proto)
        printf("Proto: %s\n", &url[proto]);
    if (has_domin)
        printf("Domin: %s\n", &url[domin]);
    if (has_port)
        printf("PORT : %s\n", &url[port]);
    if (has_path)
        printf("PATH : %s\n", &url[path]);
    return;
}

int Spliter::handle(int index)
{
    switch (state)
    {
    case PROTO:
        if (url[index] == '/')
        {
            has_domin = true;
            state = PATH;
            url[index++] = '\0';
            path = index;
        }
        else if (url[index] == ':' && url[index+1] == '/' && url[index+2] == '/')
        {
            has_proto = true;
            state = DOMIN;
            url[index] = '\0';
            index += 3;
            domin = index;
        }
        else if (url[index] == ':')
        {
            has_domin = true;
            has_port = true;
            url[index++] = '\0';
            state = PORT;
            port = index;
        }
        else if (url[index] == '\0')
        {
            has_domin = true;
            state = END;
        }
        else
            index++;
        break;
    case DOMIN:
        if (url[index] == ':')
        {
            has_domin = true;
            has_port = true;
            url[index++] = '\0';
            state = PORT;
            port = index;
        }
        else if (url[index] == '/')
        {
            has_domin = true;
            url[index++] = '\0';
            state = PATH;
            path = index;
        }
        else if (url[index] == '\0')
        {
            state = END;
            has_domin = true;
        }
        else
            index++;
        break;
    case PORT:
        if (url[index] == '/')
        {
            state = PATH;
            has_port = true;
            url[index] = '\0';
            index++;
            path = index;
        }
        else if (url[index] == '\0')
        {
            has_port = true;
            state = END;
        }
        else
            index++;
        break;
    case PATH:
        if (url[index] == '\0')
            state = END;
        else
        {
            has_path = true;
            state = END;
        }
        break;
    }
    return index;
}

char* Spliter::get_proto()
{
    if (has_proto)
        return &url[proto];
    else
        return NULL;
}

char* Spliter::get_domin()
{
    if (has_domin)
        return &url[domin];
    else
        return NULL;
}

int Spliter::get_port()
{
    int ans = 0;

    if (!has_port)
        return ans;

    char* p = &url[port];
    while (*p != '\0')
    {
        ans *= 10;
        ans += *p - '0';
        p++;
    }
    return ans;
}

char* Spliter::get_path()
{
    if (has_path)
        return &url[path];
    else
        return NULL;
}
