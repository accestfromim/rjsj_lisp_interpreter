#include <iostream>
#include <string>
#include "./value.h"
#include "./tokenizer.h"
#include "parser.h"
#include "rjsj_test.hpp"

struct TestCtx {
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        return value->toString();
    }
};

int main() {
    RJSJ_TEST(TestCtx, Lv2, Lv2Only);
    //ValuePtr a = std::make_shared<NumericValue>(42);
    //ValuePtr b = std::make_shared<BooleanValue>(false);
    //ValuePtr c = std::make_shared<SymbolValue>("eq?");
    //ValuePtr d = std::make_shared<StringValue>("Hello");
    //ValuePtr e = std::make_shared<NilValue>();
    //ValuePtr f = std::make_shared<PairValue>(
    //    c, std::make_shared<PairValue>(a, std::make_shared<PairValue>(d, e)));
    //std::cout << a->toString() << '\n'
    //          << b->toString() << '\n'
    //          << c->toString() << '\n'
    //          << d->toString() << '\n'
    //          << e->toString() << '\n'
    //          << f->toString() << std::endl;
    //while (true) {
    //    try {
    //        std::cout << ">>> " ;
    //        std::string line;
    //        std::getline(std::cin, line);
    //        if (std::cin.eof()) {
    //            std::exit(0);
    //        }
    //        auto tokens = Tokenizer::tokenize(line);
    //        Parser parser(std::move(tokens));
    //        if (parser.empty()) continue;
    //        auto value = parser.parse();
    //        std::cout << value->toString() << std::endl;
    //        /*for (auto& token : tokens) {
    //            std::cout << *token << std::endl;
    //        }*/
    //    } catch (std::runtime_error& e) {
    //        std::cerr << "Error: " << e.what() << std::endl;
    //    }
    //}
}
