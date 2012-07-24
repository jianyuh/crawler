#ifndef _CONFIG_H_
#define _CONFIG_H_

#define MAXVALUELEN 255

class Config
{
public:
    explicit Config(char* filename);
    ~Config();
    bool isload();
    char* getstr(char* key);
    int getint(char* key);
private:
    void* _load_fd;
    char _buffer[MAXVALUELEN];
};

#endif /* _CONFIG_H_ */
