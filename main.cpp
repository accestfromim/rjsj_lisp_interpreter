#include <iostream>
#include <string>
#include "./tokenizer.h"
#include "parser.h"
#include "eval_env.h"
#include "rjsj_test.hpp"
#include "files.h"

struct TestCtx {
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};

int main(int argc,char** argv) {
    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5,Lv5Extra, Lv6,Lv7);
    
    //////////////文件模式
    if (argc != 1 && argc != 2) {
        std::cerr << "Too Many Arguements!" << std::endl;
        std::exit(-1);
    } else if (argc == 2) {
        try {
            ReadFile(argv[1]);
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::exit(-1);
        }
        return 0;
    }
    ////////////////FileMode
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    while (true) {
        try {
            std::cout << ">>> " ;
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            if (parser.empty()) continue;
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
            std::cout << result->toString() << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
