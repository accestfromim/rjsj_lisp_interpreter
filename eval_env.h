#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include"value.h"
#include<unordered_map>
class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> dict;

public:
    ValuePtr reinterpretDefinedValue(ValuePtr definedValue);
    ValuePtr eval(ValuePtr expr);
};

#endif
