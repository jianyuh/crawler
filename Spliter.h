#ifndef _SPLITER_H_
#define _SPLITER_H_

enum States
{
    PROTO, DOMIN, PORT, PATH, END
};

class Spliter
{
private:
    int proto, domin, port, path, pos;
    bool has_proto, has_domin, has_port, has_path;
    States state;
    char* url;
    int handle(int index);
    void print();
public:
    Spliter(char* url);
    ~Spliter();
    void exec();
};

#endif /* _SPLITER_H_ */
