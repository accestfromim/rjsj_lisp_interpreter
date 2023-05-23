#include "files.h"

#include <fstream>
#include <stack>

#include "eval_env.h"
#include "parser.h"
#include "tokenizer.h"
class StackAdapter {
private:
    std::stack<char> data;

public:
    void push(char c) {
        switch (c) {
            case '(': data.push(c); break;
            case ')':
                if (data.empty())
                    throw std::runtime_error("More \')\' in the File");
                data.pop();
                break;
            default: throw std::runtime_error("Inner System Error");
        }
    }
    bool empty() {
        return data.empty();
    }
};
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
        if (c != '\n') line.push_back(c);
        if (c == '\n' && paren.empty()) {
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            if (parser.empty()) continue;
            auto value = parser.parse();
            env->eval(std::move(value));
            line.clear();
            
        } else if (c == '(') {
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
