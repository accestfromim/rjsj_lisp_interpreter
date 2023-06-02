#include <iostream>
#include <string>
//#include <Windows.h>
#include "./tokenizer.h"
#include "parser.h"
#include "eval_env.h"
#include "rjsj_test.hpp"
#include "files.h"
#include "error.h"
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
    //RJSJ_TEST(TestCtx, Quasiquote);
    //RJSJ_TEST(TestCtx, Errors);
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
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
            StackAdapter paren;
            char c{};
            if (std::cin.eof()) {
                std::exit(0);
            }
            while (1) {
                std::cin.get(c);
                if (std::cin.eof()) std::exit(0);
                if (!paren.inQuote)
                    if (c == ';') {
                        while (1) {
                            std::cin.get(c);
                            if (c == '\n') break;
                        }
                    }
                if (c != '\n') {
                    paren.push(c);
                    line.push_back(c);
                }
                else {
                    if (paren.empty()) {
                        auto tokens = Tokenizer::tokenize(line);
                        Parser parser(std::move(tokens));
                        if (parser.empty()) break;
                        auto value = parser.parse();
                        auto result = env->eval(std::move(value));
                        std::cout << result->toString() << std::endl;
                        break;
                    } else {
                        if (paren.inQuote)
                            throw SyntaxError("String Can't be Seperated");
                        line.push_back(' ');
                        std::cout << "... ";
                        for (int i = 1; i < line.size()/2; ++i)
                            std::cout << ' ';
                    }
                }
            }
            /*std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            if (parser.empty()) continue;
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
            std::cout << result->toString() << std::endl;*/

        } catch (std::runtime_error& e) {
            std::cout << "\033[31m";
            std::cerr << "Error: " << e.what() << std::endl;
            std::cout<<"\033[0m";
        }
    }
}
// 9d39aa10-48e4-4dac-ba9c-d8feeac63454
