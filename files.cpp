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
        if (paren.empty() || (!paren.empty() && paren.top() != '\"'))
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

        } else if (c == '(' || c == '\"') {
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
        case '\"':
            if (data.empty()) {
                data.push(c);
            } else if (data.top() == '\"') {
                data.pop();
            } else {
                data.push(c);
            }
            break;
        case '(':
            if (data.empty()) {
                data.push(c);
            } else if (data.top() == '\"') {
                break;
            } else {
                data.push(c);
            }
            break;
        case ')':
            if (data.empty()) throw LispError("Right Paren Without Left Paren");

            if (data.top() == '\"') {
                break;
            } else {
                data.pop();
            }
            break;
        default: break;
    }
}

bool StackAdapter::empty() {
    return data.empty();
}

int StackAdapter::getNum() {
    return data.size();
}

char StackAdapter::top() {
    return data.top();
}
