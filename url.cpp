#include <cstring>

#include "url.h"

void rstrip(char* str)
{
    int len = strlen(str);
    if (len == 0) return;
    len--;
    while(len >= 0 && (str[len] == ' ' || str[len] == '\n' || str[len] == '\t' || str[len] == '\r')) len--;
    str[len+1] = '\0';
    return;
}
