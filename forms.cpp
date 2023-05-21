#include "forms.h"
#include"builtins.h"


const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm}, {"quote", quoteForm}, {"if", ifForm},
    {"and", andForm},       {"or", orForm},       {"lambda", lambdaForm}};


ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) 
        throw LispError("Too Less Arguements");
    if (args.at(0)->getType() != ValueType::SYMBOL_VALUE &&
        args.at(0)->getType() != ValueType::PAIR_VALUE) 
        throw LispError("This Type of Value Can't be Defined");

    if (args.at(0)->getType() == ValueType::SYMBOL_VALUE) {
        if (args.size() != 2)
            throw LispError("SpecForm \'define\' Must Accept Two arguements");
        auto hasDefined = env.dict.find(args.at(0)->toString());
        if (hasDefined != env.dict.end()) env.dict.erase(hasDefined);
        env.dict.insert(std::pair<std::string, ValuePtr>(args.at(0)->toString(),
                                                         env.eval(args.at(1))));
        return std::make_shared<NilValue>();
    } else {
        ValuePtr name = static_cast<PairValue&>(*args.at(0)).car();
        if (name->getType() != ValueType::SYMBOL_VALUE)
            throw LispError("This Type of Value Can't be Defined");

        auto hasDefined = env.dict.find(name->toString());
        if (hasDefined != env.dict.end()) env.dict.erase(hasDefined);

        std::vector<ValuePtr> new_args = args;
        new_args.erase(new_args.begin());
        new_args.insert(new_args.begin(),
                        static_cast<PairValue&>(*args.at(0)).cdr());

        ValuePtr lambda = lambdaForm(new_args, env);
        env.dict.insert(
            std::pair<std::string, ValuePtr>(name->toString(), lambda));

        return std::make_shared<NilValue>();
    }
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) 
        throw SyntaxError("More Words Needed");

    return args.at(0);
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 3)
        throw LispError("SpecForm \'if\' Just Allow 3 Arguements");

    if (env.eval(args.at(0))->toString() != "#f") {  // 不是#f，其他都行
        return env.eval(args.at(1));
    } else {
        return env.eval(args.at(2));
    }
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 1 && args.at(0)->getType()==ValueType::NIL_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        for (int i = 0; i < args.size() - 1; ++i)
            if (env.eval(args.at(i))->toString() == "#f")
                return std::make_shared<BooleanValue>(false);
        return env.eval(args.at(args.size() - 1));
    }
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 1 && args.at(0)->getType() == ValueType::NIL_VALUE) {
        return std::make_shared<BooleanValue>(false);
    } else {
        for (int i = 0; i < args.size(); ++i) {
            ValuePtr arg = env.eval(args.at(i));
            if (arg->toString() != "#f") {
                return arg;
            }
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) 
        throw LispError("More Arguements Needed: \'lambda\'");

    ValuePtr params = args.at(0);
    if (params->getType() != ValueType::PAIR_VALUE) {
        return std::make_shared<LambdaValue>(std::vector<std::string>(),
                                             args.at(args.size() - 1));   
    } else {
        std::vector<ValuePtr> vec = static_cast<PairValue&>(*params).toVector();
        std::vector<std::string> paras;
        for (ValuePtr ele : vec) {
            if (ele->getType() != ValueType::SYMBOL_VALUE)
                throw LispError("Expect Symblo in Lambda Parameter");
            paras.push_back(ele->toString());
        }
        return std::make_shared<LambdaValue>(paras,
                                             args.at(args.size() - 1));   
    }
}
