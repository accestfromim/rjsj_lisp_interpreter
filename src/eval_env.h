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
        dict = Main_dict;
    }

public:
   // ValuePtr reinterpretDefinedValue(ValuePtr definedValue);
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(ValuePtr);
    ValuePtr apply(ValuePtr proc,const std::vector<ValuePtr>& args);
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
