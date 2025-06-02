#ifndef FILES_H
#define FILES_H

class StackAdapter {
private:
    //std::stack<char> data;
    int parens{0};
    bool afterBackSlash{0};

public:
    bool inQuote{0};

public:
    void push(char c);
    bool empty();
    int getNum();
};
void ReadFile(char* fileName);


#endif
