#ifndef _DICT_H_
#define _DICT_H_

class Dict
{
public:
    explicit Dict(int size);
    Dict(char* path, char* filename, int h_num);
    int save(char* path, char* filename);
    int add(char* url);
    bool query(char* url);
    ~Dict();
private:
    void* _wordDict;
};

#endif /* _DICT_H_ */
