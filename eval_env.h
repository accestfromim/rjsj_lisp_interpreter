#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include<unordered_map>

#include"value.h"
#include"builtins.h"

class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> dict;

public:
    EvalEnv() {
        dict.insert(std::pair<std::string, ValuePtr>(
            "+", std::make_shared<BuiltinProcValue>(&add)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "print", std::make_shared<BuiltinProcValue>(&print)));
    }
   // ValuePtr reinterpretDefinedValue(ValuePtr definedValue);
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr);
    ValuePtr apply(ValuePtr proc,std::vector<ValuePtr>& args);
};

#endif
