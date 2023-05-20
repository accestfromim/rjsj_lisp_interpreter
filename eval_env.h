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
    }
   // ValuePtr reinterpretDefinedValue(ValuePtr definedValue);
    ValuePtr eval(ValuePtr expr);
};

#endif
