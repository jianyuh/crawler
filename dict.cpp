#include "dict.h"

#include <cstring>

#include "ul_dict.h"
#include "ul_sign.h"

Dict::Dict(int size)
{
    _wordDict = db_creat(size, 0);
    return;
}

Dict::Dict(char* path, char* filename, int h_num)
{
    _wordDict = db_load(path, filename, h_num);
    return;
}

Dict::~Dict()
{
    db_del((Sdict_build*)_wordDict);
    _wordDict = NULL;
    return;
}

int Dict::save(char* path, char* filename)
{
    if (1 != db_save((Sdict_build*)_wordDict, path, filename)) {
        return -1;
    }
    return 0;
}

int Dict::add(char* url)
{
    unsigned sign1, sign2;
    Sdict_snode snode;

    creat_sign_f64(url, strlen(url), &sign1, &sign2);
    snode.sign1 = sign1;
    snode.sign2 = sign2;
    snode.code = -1;
    snode.other = 0;
    if (-1 == db_op1((Sdict_build*)_wordDict, &snode, ADD)) {
        return -1;
    }
    return 0;
}

bool Dict::query(char* url)
{
    unsigned sign1, sign2;
    Sdict_snode snode;

    creat_sign_f64(url, strlen(url), &sign1, &sign2);
    snode.sign1 = sign1;
    snode.sign2 = sign2;
    snode.code = -1;
    db_op1((Sdict_build*)_wordDict, &snode, SEEK);
    if (-1 != snode.code) {
        return true;
    }
    return false;
}
