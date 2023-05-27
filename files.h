#ifndef FILES_H
#define FILES_H

#include<stack>
class StackAdapter {
private:
    std::stack<char> data;

public:
    void push(char c);
    bool empty();
    int getNum();
    char top();
};
void ReadFile(char* fileName);


#endif
