#include "forms.h"

#include "builtins.h"

const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm}, {"quote", quoteForm},
    {"if", ifForm},         {"and", andForm},
    {"or", orForm},         {"lambda", lambdaForm},
    {"cond", condForm},     {"begin", beginForm},
    {"let", letForm},       {"quasiquote", quasiquoteForm}};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) throw LispError("Too Less Arguements");
    if (args.at(0)->getType() != ValueType::SYMBOL_VALUE &&
        args.at(0)->getType() != ValueType::PAIR_VALUE)
        throw LispError("This Type of Value Can't be Defined");

    if (args.at(0)->getType() == ValueType::SYMBOL_VALUE) {
        if (args.size() != 2)
            throw LispError("SpecForm \'define\' Must Accept Two arguements");
        ValuePtr value = env.eval(args.at(1));
        auto hasDefined = env.dict.find(args.at(0)->toString());
        if (hasDefined != env.dict.end()) env.dict.erase(hasDefined);
        env.dict.insert(std::pair<std::string, ValuePtr>(args.at(0)->toString(),
                                                         value));
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
    if (args.size() == 0) throw SyntaxError("More Words Needed");

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
    if (args.size() == 1 && args.at(0)->getType() == ValueType::NIL_VALUE) {
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
    if (args.size() < 2) throw LispError("More Arguements Needed: \'lambda\'");

    ValuePtr params = args.at(0);
    std::vector<ValuePtr> body = args;
    body.erase(body.begin());
    if (params->getType() != ValueType::PAIR_VALUE) {
        return std::make_shared<LambdaValue>(std::vector<std::string>(), body,
                                             env.shared_from_this());
    } else {
        std::vector<ValuePtr> vec = static_cast<PairValue&>(*params).toVector();
        std::vector<std::string> paras;
        for (ValuePtr ele : vec) {
            if (ele->getType() != ValueType::SYMBOL_VALUE)
                throw LispError("Expect Symblo in Lambda Parameter");
            paras.push_back(ele->toString());
        }
        return std::make_shared<LambdaValue>(paras, body,
                                             env.shared_from_this());
    }
}

ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (ValuePtr child : args) {
        if (child->getType() != ValueType::PAIR_VALUE)
            throw LispError("\'cond\' Must Followed by Lists");

        ValuePtr condition = static_cast<PairValue&>(*child).car();
        if (condition->toString() == "else") {
            if (child == args.at(args.size() - 1)) {
                ValuePtr cdr = static_cast<PairValue&>(*child).cdr();
                if (cdr->getType() == ValueType::NIL_VALUE) {
                    throw LispError("Nothing After \'else\'");
                } else {
                    // assert(cdr是对子)
                    std::vector<ValuePtr> vec =
                        static_cast<PairValue&>(*cdr).toVector();
                    for (ValuePtr item : vec) {
                        ValuePtr result = env.eval(item);
                        if (item == vec.at(vec.size() - 1)) return result;
                    }
                }
            } else {
                throw LispError("Symbol \'else\' Must be at Last");
            }
        } else {
            ValuePtr value = env.eval(condition);
            if (value->toString() != "#f") {
                ValuePtr cdr = static_cast<PairValue&>(*child).cdr();
                if (cdr->getType() == ValueType::NIL_VALUE) {
                    return value;
                } else {
                    //assert(cdr是对子)
                    std::vector<ValuePtr> vec =
                        static_cast<PairValue&>(*cdr).toVector();
                    for (ValuePtr item : vec) {
                        ValuePtr result = env.eval(item);
                        if (item == vec.at(vec.size() - 1))
                            return result;
                    }
                }
            } else {
                continue;
            }
        }
    }
}

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) 
        throw LispError("At Least One Arguement Needed in \'begin\' Proc");

    for (ValuePtr item : args) {
        ValuePtr value = env.eval(item);
        if (item == args.at(args.size() - 1)) return value;  
    }
}

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2)
        throw LispError("\'let\' Must Followed by no Less Than Two Arguements");

    ValuePtr _args = args.at(0);
    std::vector<ValuePtr> body = args;
    body.erase(body.begin());
    if (_args->getType() == ValueType::NIL_VALUE) {
        return beginForm(body, env);
    } else {
        if (_args->getType() != ValueType::PAIR_VALUE)
            throw LispError("let : bad syntax");

        std::vector<ValuePtr> vec = static_cast<PairValue&>(*_args).toVector();
        std::vector<ValuePtr> name, value;
        for (ValuePtr item : vec) {
            if (item->getType() != ValueType::PAIR_VALUE)
                throw LispError("let : bad syntax");
            std::vector<ValuePtr> nameValuePair =
                static_cast<PairValue&>(*item).toVector();
            if (nameValuePair.size() != 2) 
                throw LispError("let : bad syntax");
            
            name.push_back(nameValuePair.at(0));
            value.push_back(env.eval(nameValuePair.at(1)));
        }
        ValuePtr nameListPtr = list(name, env);
        body.insert(body.begin(), nameListPtr);
        ValuePtr proc = lambdaForm(body, env);
       /* ValuePtr valueListPtr = list(value, env);
        ValuePtr procList =
            std::make_shared<PairValue>(proc, std::make_shared<NilValue>());
        ValuePtr sentence = _append(std::vector<ValuePtr>{procList,valueListPtr},env);*/
        return env.apply(proc, value);
    }
}
//(let ((x 2)) ((begin (define x (+ x 1)) +) 3 (begin (define x (+ x 1)) x)))
ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    //assert(args.size()==1)
    if (args.size() == 0) 
        throw LispError("More Words Needed");

    if (args.at(0)->getType() != ValueType::PAIR_VALUE) {
        return args.at(0);
    } else {
        std::vector<ValuePtr> vec =
            static_cast<PairValue&>(*args.at(0)).toVector();
        auto iter = vec.begin();
        while (iter != vec.end()) {
            if ((*iter)->getType() != ValueType::PAIR_VALUE) {
                ++iter;
                continue;
            } else {
                if (static_cast<PairValue&>(*(*iter)).car()->toString() ==
                    "unquote") {
                    *iter = env.eval(static_cast<PairValue&>(*static_cast<PairValue&>(*(*iter)).cdr()).car());
                } else {
                    std::vector<ValuePtr> childvec =
                        static_cast<PairValue&>(*(*iter)).toVector();
                    *iter = quasiquoteForm(childvec, env);
                }

            }
            ++iter;
        }
        return list(vec, env);
    }
}
