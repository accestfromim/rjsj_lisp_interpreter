#include <fstream>
#include <stack>

#include "files.h"
#include "eval_env.h"
#include "parser.h"
#include "tokenizer.h"

void ReadFile(char* fileName) {
    std::ifstream ifile(fileName);
    if (!ifile.is_open()) throw std::runtime_error("No Shch File");
    StackAdapter paren;
    std::string line;
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    char c{};
    if (ifile.eof()) return;
    while (1) {
        ifile.get(c);
        if (ifile.eof()) break;
        if (!paren.inQuote)
            if (c == ';') {
                while (1) {
                    ifile.get(c);
                    if (c == '\n') break;
                }
            }
        if (c != '\n') line.push_back(c);
        if (c == '\n' && paren.empty()) {
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            if (parser.empty()) continue;
            auto value = parser.parse();
            env->eval(std::move(value));
            line.clear();

        } else if (c == '(' || c == '\"' || c =='\\') {
            paren.push(c);
        } else if (c == ')') {
            paren.push(c);
            if (paren.empty()) {
                auto tokens = Tokenizer::tokenize(line);
                Parser parser(std::move(tokens));
                if (parser.empty()) continue;
                auto value = parser.parse();
                env->eval(std::move(value));
                line.clear();
            }
        }
    }
}

void StackAdapter::push(char c) {
    switch (c) {
        case '\\': 
            afterBackSlash = !afterBackSlash;
            break;
        case '\"': 
            if (afterBackSlash) {
                afterBackSlash = false;
                break;
            }
            inQuote = !inQuote;
            break;
        case '(':
            afterBackSlash = false;
            if (!inQuote) parens += 1;
            break;
        case ')':
            afterBackSlash = false;
            if (inQuote) break;
            if (parens == 0) throw LispError("Right Paren Without Left Paren");

            if (inQuote) {
                break;
            } else {
                parens -= 1;
            }
            break;
        default: 
            afterBackSlash = false;
    }
}

bool StackAdapter::empty() {
    return parens == 0;
}

int StackAdapter::getNum() {
    return parens;
}
