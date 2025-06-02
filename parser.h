#ifndef PARSER_H
#define PARSER_H
#include"tokenizer.h"
#include"value.h"
#include"error.h"

class Parser {
private:
    std::deque<TokenPtr> Token_data;

public:
    Parser(std::deque<TokenPtr> tokens);
    bool empty() {
        return Token_data.empty();
    }
    ValuePtr parse();
    ValuePtr parseTails();
};

#endif
