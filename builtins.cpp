#include<cmath>

#include "builtins.h"
#include "eval_env.h"

ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env) {
    double result = 0;
    for (const auto& i : params) {
        if (i->getType() != ValueType::NUMERIC_VALUE) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += static_cast<NumericValue&>(*i).getValue();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr minus(const std::vector<ValuePtr>& params, EvalEnv& env) {
    double result = 0;
    if (params.size() == 0)
        throw LispError("Proc - Allow at Least One Agruement");

    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Cannot Minus a non-numeric value");
    result = 2 * static_cast<NumericValue&>(*params.at(0)).getValue();
    if (params.size() == 1) {
        return std::make_shared<NumericValue>(-(result/2));
    } else {
        for (const auto& i : params) {
            if (i->getType() != ValueType::NUMERIC_VALUE) {
                throw LispError("Cannot Minus a non-numeric value.");
            }
            result -= static_cast<NumericValue&>(*i).getValue();
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1) {
        throw LispError("Print Just Allow One Element");
    } else {
        std::cout << params.at(0)->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr devide(const std::vector<ValuePtr>& params, EvalEnv& env) {
    double result = 1;
    if (params.size() == 0)
        throw LispError("Proc / Allow at Least One Agruement");

    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Cannot Devide a non-numeric value");
    result = static_cast<NumericValue&>(*params.at(0)).getValue() *
             static_cast<NumericValue&>(*params.at(0)).getValue();
    if (params.size() == 1) {
        if (result == 0)
            throw LispError("Devided by Zero");
        return std::make_shared<NumericValue>(1/sqrt(result));
    } else {
        for (const auto& i : params) {
            if (i->getType() != ValueType::NUMERIC_VALUE) {
                throw LispError("Cannot Devide a non-numeric value.");
            }
            if (static_cast<NumericValue&>(*i).getValue() == 0)
                throw LispError("Devided by Zero");
            result /= static_cast<NumericValue&>(*i).getValue();
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr multiply(const std::vector<ValuePtr>& params, EvalEnv& env) {
    double result = 1;
    for (const auto& i : params) {
        if (i->getType() != ValueType::NUMERIC_VALUE) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        result *= static_cast<NumericValue&>(*i).getValue();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2) 
        throw LispError("Proc \'cons\' just Accept Two Arguements");
    
    return std::make_shared<PairValue>(params.at(0),params.at(1));
}

ValuePtr list(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() == 0) return std::make_shared<NilValue>();
    std::vector<ValuePtr> cdr = params;
    cdr.erase(cdr.begin());
    return std::make_shared<PairValue>(params.at(0),list(cdr,env));
}

ValuePtr list_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'list?\' Just Allow One arguement");
    if (params.at(0)->getType() == ValueType::NIL_VALUE) {
        return std::make_shared<BooleanValue>(true);
    }else if (params.at(0)->getType() != ValueType::PAIR_VALUE) {
        return std::make_shared<BooleanValue>(false);
    } else {
        ValuePtr now = static_cast<PairValue&>(*params.at(0)).cdr();
        while (1) {
            if (now->getType() == ValueType::NIL_VALUE) {
                return std::make_shared<BooleanValue>(true);
            } else if (now->getType() != ValueType::PAIR_VALUE) {
                return std::make_shared<BooleanValue>(false);
            } else {
                now = static_cast<PairValue&>(*now).cdr();
            }
        }
    }
}

ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1) 
        throw LispError("Proc \'display\' Allow Just One Arguement");
    if (params.at(0)->getType() == ValueType::STRING_VALUE) {
        std::cout << static_cast<StringValue&>(*params.at(0)).getValue()<<std::endl;
    } else {
        std::cout << params.at(0)->toString();
    }
    return std::make_shared<NilValue>();
}

ValuePtr displayln(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'displayln\' Allow Just One Arguement");
    if (params.at(0)->getType() == ValueType::STRING_VALUE) {
        std::cout << static_cast<StringValue&>(*params.at(0)).getValue()
                  << std::endl;
    } else {
        std::cout << params.at(0)->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr LispExit(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() > 1) 
        throw LispError("Too Much Arguements Given");

    std::exit(0);
    return std::make_shared<NilValue>();
}

ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 0)
        throw LispError("Proc \'newline\' Doesn't Allow Any Arguements");

    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr atom_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'atom?\' Just Allow One Arguement");

    ValueType type = params.at(0)->getType();
    if (type == ValueType::BOOLEAN_VALUE || type == ValueType::STRING_VALUE ||
        type == ValueType::NUMERIC_VALUE||type==ValueType::SYMBOL_VALUE||type==ValueType::NIL_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }

}

ValuePtr boolean_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'boolean?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::BOOLEAN_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr integer_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'integer?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::NUMERIC_VALUE) {
        double integer = static_cast<NumericValue&>(*params.at(0)).getValue();

        if (std::abs(static_cast<NumericValue&>(*params.at(0)).getValue() -
                static_cast<int>(integer)) < 1e-6)
            return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
    
}

ValuePtr number_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'number?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::NUMERIC_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr null_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'null?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::NIL_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr pair_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'pair?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::PAIR_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr procedure_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'procedure?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::BUILTINPROC_VALUE ||
        params.at(0)->getType() == ValueType::LAMBDA_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr string_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'string?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::STRING_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr symbol_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'symbol?\' Just Allow One Arguement");

    
    if (params.at(0)->getType() == ValueType::SYMBOL_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
    
}

ValuePtr listLength(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'length\' Just Allow One Arguement");

    if (!(static_cast<BooleanValue&>(*list_htn(params,env)).getValue()))
        throw LispError("Proc \'length\' Just Allow List as Arguement");

    int len = 0;
    ValuePtr now = params.at(0);
    while (1) {
        if (now->getType() == ValueType::NIL_VALUE) {
            return std::make_shared<NumericValue>(len);
        } else {
            ++len;
            now = static_cast<PairValue&>(*now).cdr();
        }
    }
    throw LispError("Inner System error");
}

ValuePtr _cdr(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1) 
        throw LispError("Proc \'cdr\' Just Allow One Arguement");

    if (params.at(0)->getType() != ValueType::PAIR_VALUE)
        throw LispError("Proc \'cdr\' Just Allow Arguement with Type Pair or List Except Nil");

    return static_cast<PairValue&>(*params.at(0)).cdr();

}

ValuePtr _car(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'car\' Just Allow One Arguement");

    if (params.at(0)->getType() != ValueType::PAIR_VALUE)
        throw LispError(
            "Proc \'car\' Just Allow Arguement with Type Pair or List Except Nil");

    return static_cast<PairValue&>(*params.at(0)).car();
}

ValuePtr _append(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() == 0) {
        return std::make_shared<NilValue>();
    } else {
        for (ValuePtr i : params)
            if (!static_cast<BooleanValue&>(*list_htn(std::vector<ValuePtr>{i},env)).getValue()) 
                throw LispError("Proc \'append\' Can Just Apply to Lists");
        ValuePtr car = params.at(0);
        if (car->getType() == ValueType::NIL_VALUE) {
            std::vector<ValuePtr> new_one = params;
            new_one.erase(new_one.begin());
            return _append(new_one,env);
        } else {
            ValuePtr list_car = static_cast<PairValue&>(*car).car();
            std::vector<ValuePtr> new_one = params;
            new_one.erase(new_one.begin());
            new_one.insert(new_one.begin(),
                           static_cast<PairValue&>(*car).cdr());
            return std::make_shared<PairValue>(list_car, _append(new_one,env));
        }
    }
}

ValuePtr _abs(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'abs\' Just Allow One Arguement");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'abs\' Just Allow Numeric Arguement");

    double value = static_cast<NumericValue&>(*params.at(0)).getValue();
    return std::make_shared<NumericValue>(std::abs(value));
}

ValuePtr _expt(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'expt\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'expt\' Just Allow Numeric Arguements");
    if (params.at(1)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'expt\' Just Allow Numeric Arguements");
    double value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    double value2 = static_cast<NumericValue&>(*params.at(1)).getValue();
    if (value1 == 0 && value2 < 0) 
        throw LispError("Devided by Zero");

    return std::make_shared<NumericValue>(std::pow(value1,value2));
}

ValuePtr _quotient(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'quotient\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'quotient\' Just Allow Numeric Arguements");
    if (params.at(1)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'quotient\' Just Allow Numeric Arguements");
    double value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    double value2 = static_cast<NumericValue&>(*params.at(1)).getValue();
    if (value2 == 0) 
        throw LispError("Devided by Zero");
    return std::make_shared<NumericValue>((int)(value1/value2));
}

ValuePtr _modulo(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'modulo\' Just Allow Two Arguements");

    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)},env))
              .getValue()))
        throw LispError("Proc \'modulo\' Can Just Apply to Integers");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(1)},env))
              .getValue()))
        throw LispError("Proc \'modulo\' Can Just Apply to Integers");

    int value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    int value2 = static_cast<NumericValue&>(*params.at(1)).getValue();
    if (value2 == 0) 
        throw LispError("Devided by Zero");
    
    if (value1 == 0) {
        return std::make_shared<NumericValue>(value2);
    } else {
        int mod = std::abs(value1) % std::abs(value2);
        if (value1 > 0 && value2 > 0) {
            return std::make_shared<NumericValue>(mod);
        } else if (value1 > 0 && value2 < 0) {
            return std::make_shared<NumericValue>(mod+value2);
        } else if (value1 < 0 && value2 > 0) {
            return std::make_shared<NumericValue>(value2 - mod);
        } else {
            return std::make_shared<NumericValue>(-mod);
        }
    }
    throw LispError("Inner System Error");
}

ValuePtr _remainder(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'remainder\' Just Allow Two Arguements");

    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)},env))
              .getValue()))
        throw LispError("Proc \'remainder\' Can Just Apply to Integers");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(1)},env))
              .getValue()))
        throw LispError("Proc \'remainder\' Can Just Apply to Integers");

    int value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    int value2 = static_cast<NumericValue&>(*params.at(1)).getValue();
    if (value2 == 0) 
        throw LispError("Devided by Zero");

    return std::make_shared<NumericValue>(value1-value2*static_cast<NumericValue&>(*_quotient(params,env)).getValue());
}

ValuePtr num_equal(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'=\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'=\' Just Allow Numeric Arguements");
    if (params.at(1)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'=\' Just Allow Numeric Arguements");
    double value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    double value2 = static_cast<NumericValue&>(*params.at(1)).getValue();

    if (std::abs(value1 - value2) < 1e-6) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr num_less(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'<\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'<\' Just Allow Numeric Arguements");
    if (params.at(1)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'<\' Just Allow Numeric Arguements");
    double value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    double value2 = static_cast<NumericValue&>(*params.at(1)).getValue();

    if (value1 - value2 < -1e-6) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr num_greater(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'>\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'>\' Just Allow Numeric Arguements");
    if (params.at(1)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'>\' Just Allow Numeric Arguements");
    double value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    double value2 = static_cast<NumericValue&>(*params.at(1)).getValue();

    if (value1 - value2 > 1e-6) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr num_less_or_equal(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'<=\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'<=\' Just Allow Numeric Arguements");
    if (params.at(1)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'<=\' Just Allow Numeric Arguements");
    double value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    double value2 = static_cast<NumericValue&>(*params.at(1)).getValue();

    if (value1 - value2 < 1e-6) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr num_greater_or_equal(const std::vector<ValuePtr>& params,
                              EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'>=\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'>=\' Just Allow Numeric Arguements");
    if (params.at(1)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'>=\' Just Allow Numeric Arguements");
    double value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    double value2 = static_cast<NumericValue&>(*params.at(1)).getValue();

    if (value1 - value2 > -1e-6) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr even_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'even?\' Just Allow One Arguements");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)},env))
              .getValue()))
        throw LispError("Proc \'even\' Can Just Apply to Integers");

    int value = static_cast<NumericValue&>(*params.at(0)).getValue();
    if (value % 2 == 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr odd_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'odd?\' Just Allow One Arguements");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)},env))
              .getValue()))
        throw LispError("Proc \'odd\' Can Just Apply to Integers");

    int value = static_cast<NumericValue&>(*params.at(0)).getValue();
    if (value % 2 != 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr zero_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'zero?\' Just Allow Two Arguements");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'zero?\' Just Allow Numeric Arguements");
    
    double value = static_cast<NumericValue&>(*params.at(0)).getValue();

    if (std::abs(value) < 1e-6) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr _eval(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1) 
        throw LispError("Proc \'_eval\' just Allow 1 Arguement");

    return env.eval(params.at(0));
}

ValuePtr _apply(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'apply\' just Allow Two Arguements");

    std::vector<ValuePtr> to_apply = params;
    ValuePtr proc = to_apply.at(0);
    if (to_apply.at(1)->getType() == ValueType::PAIR_VALUE) {
        to_apply = static_cast<PairValue&>(*to_apply.at(1)).toVector();
    } else if (to_apply.at(1)->getType() == ValueType::NIL_VALUE) {
        to_apply.clear();
    } else {
        to_apply.erase(to_apply.begin());
    }
    return env.apply(proc, to_apply);
}

ValuePtr equal_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size()!=2)
        throw LispError("Proc \'equal?\' just Allow Two Arguements");

    if (params.at(0)->getType() == ValueType::BUILTINPROC_VALUE ||
        params.at(0)->getType() == ValueType::LAMBDA_VALUE){
        if (params.at(0) == params.at(1)) {
            return std::make_shared<BooleanValue>(true);
        } else {
            return std::make_shared<BooleanValue>(false);
        }
    } else if(params.at(0)->toString()==params.at(1)->toString()){
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr eq_htn(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2)
        throw LispError("Proc \'equal?\' just Allow Two Arguements");

    if (params.at(0)->getType() == ValueType::PAIR_VALUE ||
        params.at(0)->getType() == ValueType::STRING_VALUE) {
        if (params.at(0) == params.at(1)) {
            return std::make_shared<BooleanValue>(true);
        } else {
            return std::make_shared<BooleanValue>(false);
        }
    } else {
        return equal_htn(params, env);
    }
}

ValuePtr _error(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 1)
        throw LispError("Proc \'error\' just Allow One Arguement");

    if (params.at(0)->getType() != ValueType::STRING_VALUE &&
        params.at(0)->getType() != ValueType::SYMBOL_VALUE)
        throw LispError("Proc \'error\' just Allow String Value or Symbol Value");

    throw LispError(params.at(0)->toString());
}

ValuePtr _map(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 2) 
        throw LispError("Proc \'map\' just Allow Two Arguements");

    ValuePtr proc = params.at(0);
    if (proc->getType() != ValueType::BUILTINPROC_VALUE &&
        proc->getType() != ValueType::LAMBDA_VALUE)
        throw LispError("Proc \'map\' must Allow a Procedure as First Arguement");

     if (!(static_cast<BooleanValue&>(
              *list_htn(std::vector<ValuePtr>{params.at(1)}, env))
              .getValue()))
        throw LispError("Proc \'map\' must Allow a List as Second Arguement");

     if (params.at(1)->getType() == ValueType::NIL_VALUE) {
        return std::make_shared<NilValue>();
     } else {
        std::vector<ValuePtr> items =
            static_cast<PairValue&>(*params.at(1)).toVector();
        std::vector<ValuePtr> result;
        for (ValuePtr item : items)
            result.push_back(env.apply(proc,std::vector<ValuePtr>{item}));
        return list(result, env);
     }
}

ValuePtr _filter(const std::vector<ValuePtr>& params, EvalEnv& env) {
     if (params.size() != 2)
        throw LispError("Proc \'filter\' just Allow Two Arguements");

     ValuePtr proc = params.at(0);
     if (proc->getType() != ValueType::BUILTINPROC_VALUE &&
         proc->getType() != ValueType::LAMBDA_VALUE)
        throw LispError(
            "Proc \'filter\' must Allow a Procedure as First Arguement");

     if (!(static_cast<BooleanValue&>(
               *list_htn(std::vector<ValuePtr>{params.at(1)}, env))
               .getValue()))
        throw LispError("Proc \'filter\' must Allow a List as Second Arguement");

     if (params.at(1)->getType() == ValueType::NIL_VALUE) {
        return std::make_shared<NilValue>();
     } else {
        std::vector<ValuePtr> items =
            static_cast<PairValue&>(*params.at(1)).toVector();
        std::vector<ValuePtr> result;
        for (ValuePtr item : items) {
            ValuePtr back = env.apply(proc, std::vector<ValuePtr>{item});
            if (static_cast<BooleanValue&>(*back).getValue())
                result.push_back(item);
        }
        return list(result, env);
     }
}

ValuePtr _reduce(const std::vector<ValuePtr>& params, EvalEnv& env) {
     if (params.size() != 2)
        throw LispError("Proc \'reduce\' just Allow Two Arguements");

     ValuePtr proc = params.at(0);
     if (proc->getType() != ValueType::BUILTINPROC_VALUE &&
         proc->getType() != ValueType::LAMBDA_VALUE)
        throw LispError(
            "Proc \'reduce\' must Allow a Procedure as First Arguement");

     if (!(static_cast<BooleanValue&>(
               *list_htn(std::vector<ValuePtr>{params.at(1)}, env))
               .getValue()))
        throw LispError("Proc \'reduce\' must Allow a List as Second Arguement");

     if (params.at(1)->getType() == ValueType::NIL_VALUE) 
        throw LispError(
            "Proc \'reduce\' doesn't Allow NilValue as Second Arguement");
     if (static_cast<NumericValue&>(
             *listLength(std::vector<ValuePtr>{params.at(1)}, env))
             .getValue() == 1) {
        return _car(std::vector<ValuePtr>{params.at(1)}, env);
     } else {
        return env.apply(
            proc,
            std::vector<ValuePtr>{
                _car(std::vector<ValuePtr>{params.at(1)}, env),
                _reduce(
                    std::vector<ValuePtr>{
                        proc, _cdr(std::vector<ValuePtr>{params.at(1)}, env)},
                    env)});
     }
}

ValuePtr _not(const std::vector<ValuePtr>& params, EvalEnv& env) {
     if (params.size() != 1) 
         throw LispError("Proc \'reduce\' just Allow One Arguements");

     if (params.at(0)->toString() == "#f") {
         return std::make_shared<BooleanValue>(true);
     } else {
         return std::make_shared<BooleanValue>(false);
     }
}

