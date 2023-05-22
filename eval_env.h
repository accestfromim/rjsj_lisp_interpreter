#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include<unordered_map>

#include"value.h"
#include"builtins.h"

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
public:
    std::unordered_map<std::string, ValuePtr> dict;
    std::shared_ptr<EvalEnv> parent;

public:
    EvalEnv(const std::unordered_map<std::string, ValuePtr>& map,
            std::shared_ptr<EvalEnv> parent)
        : dict{map}, parent{parent} {}
    EvalEnv(std::shared_ptr<EvalEnv> parent = nullptr) : parent{parent} {
        dict.insert(std::pair<std::string, ValuePtr>(
            "+", std::make_shared<BuiltinProcValue>(&add)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "-", std::make_shared<BuiltinProcValue>(&minus)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "print", std::make_shared<BuiltinProcValue>(&print)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "*", std::make_shared<BuiltinProcValue>(&multiply)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "/", std::make_shared<BuiltinProcValue>(&devide)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "cons", std::make_shared<BuiltinProcValue>(&cons)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "list", std::make_shared<BuiltinProcValue>(&list)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "list?", std::make_shared<BuiltinProcValue>(&list_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "display", std::make_shared<BuiltinProcValue>(&display)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "exit", std::make_shared<BuiltinProcValue>(&LispExit)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "newline", std::make_shared<BuiltinProcValue>(&newline)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "atom?", std::make_shared<BuiltinProcValue>(&atom_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "boolean?", std::make_shared<BuiltinProcValue>(&boolean_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "integer?", std::make_shared<BuiltinProcValue>(&integer_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "number?", std::make_shared<BuiltinProcValue>(&number_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "null?", std::make_shared<BuiltinProcValue>(&null_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "pair?", std::make_shared<BuiltinProcValue>(&pair_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "procedure?", std::make_shared<BuiltinProcValue>(&procedure_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "string?", std::make_shared<BuiltinProcValue>(&string_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "symbol?", std::make_shared<BuiltinProcValue>(&symbol_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "length", std::make_shared<BuiltinProcValue>(&listLength)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "cdr", std::make_shared<BuiltinProcValue>(&_cdr)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "car", std::make_shared<BuiltinProcValue>(&_car)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "append", std::make_shared<BuiltinProcValue>(&_append)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "abs", std::make_shared<BuiltinProcValue>(&_abs)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "expt", std::make_shared<BuiltinProcValue>(&_expt)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "quotient", std::make_shared<BuiltinProcValue>(&_quotient)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "modulo", std::make_shared<BuiltinProcValue>(&_modulo)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "remainder", std::make_shared<BuiltinProcValue>(&_remainder)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "=", std::make_shared<BuiltinProcValue>(&num_equal)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "<", std::make_shared<BuiltinProcValue>(&num_less)));
        dict.insert(std::pair<std::string, ValuePtr>(
            ">", std::make_shared<BuiltinProcValue>(&num_greater)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "<=", std::make_shared<BuiltinProcValue>(&num_less_or_equal)));
        dict.insert(std::pair<std::string, ValuePtr>(
            ">=", std::make_shared<BuiltinProcValue>(&num_greater_or_equal)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "even?", std::make_shared<BuiltinProcValue>(&even_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "odd?", std::make_shared<BuiltinProcValue>(&odd_htn)));
        dict.insert(std::pair<std::string, ValuePtr>(
            "zero?", std::make_shared<BuiltinProcValue>(&zero_htn)));
    }

public:
   // ValuePtr reinterpretDefinedValue(ValuePtr definedValue);
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr);
    ValuePtr apply(ValuePtr proc,std::vector<ValuePtr>& args);
    ValuePtr lookupBinding(const std::string& name)const;
    static std::shared_ptr<EvalEnv> createGlobal() {
        return std::make_shared<EvalEnv>();
    }
    static std::shared_ptr<EvalEnv> createGlobal(
        const std::unordered_map<std::string, ValuePtr>& dict,
        std::shared_ptr<EvalEnv> parent) {
        return std::make_shared<EvalEnv>(dict,parent);
    }
    std::shared_ptr<EvalEnv> createChild(
        const std::vector<std::string>& params,
        const std::vector<ValuePtr>& args);
};

#endif
