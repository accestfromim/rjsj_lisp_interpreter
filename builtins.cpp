#include<cmath>

#include "builtins.h"


ValuePtr add(arguementType params) {
    double result = 0;
    for (const auto& i : params) {
        if (i->getType() != ValueType::NUMERIC_VALUE) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += static_cast<NumericValue&>(*i).getValue();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr minus(arguementType params) {
    double result = 0;
    if (params.size() == 0)
        throw LispError("Proc - Allow at Least One Agruement");

    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Cannot Minus a non-numeric value");
    result = 2*static_cast<NumericValue&>(*params.at(0)).getValue();
    if (params.size() == 1) {
        return std::make_shared<NumericValue>(-result/2);
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

ValuePtr print(arguementType params) {
    if (params.size() != 1) {
        throw LispError("Print Just Allow One Element");
    } else {
        std::cout << params.at(0)->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr devide(arguementType params) {
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

ValuePtr multiply(arguementType params) {
    double result = 1;
    for (const auto& i : params) {
        if (i->getType() != ValueType::NUMERIC_VALUE) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result *= static_cast<NumericValue&>(*i).getValue();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr cons(arguementType params) {
    if (params.size() < 2) 
        throw LispError("Proc \'cons\' Accept at Least Two Arguements");
    
    return std::make_shared<PairValue>(params.at(0),params.at(1));
}

ValuePtr list(arguementType params) {
    if (params.size() == 0) return std::make_shared<NilValue>();
    std::vector<ValuePtr> cdr = params;
    cdr.erase(cdr.begin());
    return std::make_shared<PairValue>(params.at(0),list(cdr));
}

ValuePtr list_htn(arguementType params) {
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

ValuePtr display(arguementType params) {
    if (params.size() != 1) 
        throw LispError("Proc \'display\' Allow Just One Arguement");
    if (params.at(0)->getType() == ValueType::STRING_VALUE) {
        std::cout << static_cast<StringValue&>(*params.at(0)).getValue()<<std::endl;
    } else {
        std::cout << params.at(0)->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr LispExit(arguementType params) {
    if (params.size() > 1) 
        throw LispError("Too Much Arguements Given");

    std::exit(0);
    return std::make_shared<NilValue>();
}

ValuePtr newline(arguementType params) {
    if (params.size() != 0)
        throw LispError("Proc \'newline\' Doesn't Allow Any Arguements");

    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr atom_htn(arguementType params) {
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

ValuePtr boolean_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'boolean?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::BOOLEAN_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr integer_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'integer?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::NUMERIC_VALUE) {
        double integer = static_cast<NumericValue&>(*params.at(0)).getValue();

        if (static_cast<NumericValue&>(*params.at(0)).getValue() -
                static_cast<int>(integer) < 1e-6)
            return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
    
}

ValuePtr number_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'number?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::NUMERIC_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr null_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'null?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::NIL_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr pair_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'pair?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::PAIR_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr procedure_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'procedure?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::BUILTINPROC_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr string_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'string?\' Just Allow One Arguement");

    if (params.at(0)->getType() == ValueType::STRING_VALUE) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr symbol_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'symbol?\' Just Allow One Arguement");

    
    return std::make_shared<BooleanValue>(false);
    
}

ValuePtr listLength(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'length\' Just Allow One Arguement");

    if (!(static_cast<BooleanValue&>(*list_htn(params)).getValue()))
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

ValuePtr _cdr(arguementType params) {
    if (params.size() != 1) 
        throw LispError("Proc \'cdr\' Just Allow One Arguement");

    if (params.at(0)->getType() != ValueType::PAIR_VALUE)
        throw LispError("Proc \'cdr\' Just Allow Arguement with Type Pair or List Except Nil");

    return static_cast<PairValue&>(*params.at(0)).cdr();

}

ValuePtr _car(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'car\' Just Allow One Arguement");

    if (params.at(0)->getType() != ValueType::PAIR_VALUE)
        throw LispError(
            "Proc \'car\' Just Allow Arguement with Type Pair or List Except Nil");

    return static_cast<PairValue&>(*params.at(0)).car();
}

ValuePtr _append(arguementType params) {
    if (params.size() == 0) {
        return std::make_shared<NilValue>();
    } else {
        for (ValuePtr i : params)
            if (!list_htn(std::vector<ValuePtr>{i})) 
                throw LispError("Proc \'append\' Can Just Apply to Lists");
        ValuePtr car = params.at(0);
        if (car->getType() == ValueType::NIL_VALUE) {
            std::vector<ValuePtr> new_one = params;
            new_one.erase(new_one.begin());
            return _append(new_one);
        } else {
            ValuePtr list_car = static_cast<PairValue&>(*car).car();
            std::vector<ValuePtr> new_one = params;
            new_one.erase(new_one.begin());
            new_one.insert(new_one.begin(),
                           static_cast<PairValue&>(*car).cdr());
            return std::make_shared<PairValue>(list_car, _append(new_one));
        }
    }
}

ValuePtr _abs(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'abs\' Just Allow One Arguement");
    if (params.at(0)->getType() != ValueType::NUMERIC_VALUE)
        throw LispError("Proc \'abs\' Just Allow Numeric Arguement");

    double value = static_cast<NumericValue&>(*params.at(0)).getValue();
    return std::make_shared<NumericValue>(std::abs(value));
}

ValuePtr _expt(arguementType params) {
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

ValuePtr _quotient(arguementType params) {
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

ValuePtr _modulo(arguementType params) {
    if (params.size() != 2)
        throw LispError("Proc \'modulo\' Just Allow Two Arguements");

    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)}))
              .getValue()))
        throw LispError("Proc \'modulo\' Can Just Apply to Integers");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(1)}))
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

ValuePtr _remainder(arguementType params) {
    if (params.size() != 2)
        throw LispError("Proc \'remainder\' Just Allow Two Arguements");

    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)}))
              .getValue()))
        throw LispError("Proc \'remainder\' Can Just Apply to Integers");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(1)}))
              .getValue()))
        throw LispError("Proc \'remainder\' Can Just Apply to Integers");

    int value1 = static_cast<NumericValue&>(*params.at(0)).getValue();
    int value2 = static_cast<NumericValue&>(*params.at(1)).getValue();
    if (value2 == 0) 
        throw LispError("Devided by Zero");

    return std::make_shared<NumericValue>(value1-value2*static_cast<NumericValue&>(*_quotient(params)).getValue());
}

ValuePtr num_equal(arguementType params) {
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

ValuePtr num_less(arguementType params) {
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

ValuePtr num_greater(arguementType params) {
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

ValuePtr num_less_or_equal(arguementType params) {
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

ValuePtr num_greater_or_equal(arguementType params) {
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

ValuePtr even_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'even?\' Just Allow One Arguements");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)}))
              .getValue()))
        throw LispError("Proc \'even\' Can Just Apply to Integers");

    int value = static_cast<NumericValue&>(*params.at(0)).getValue();
    if (value % 2 == 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr odd_htn(arguementType params) {
    if (params.size() != 1)
        throw LispError("Proc \'odd?\' Just Allow One Arguements");
    if (!(static_cast<BooleanValue&>(
              *integer_htn(std::vector<ValuePtr>{params.at(0)}))
              .getValue()))
        throw LispError("Proc \'odd\' Can Just Apply to Integers");

    int value = static_cast<NumericValue&>(*params.at(0)).getValue();
    if (value % 2 != 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr zero_htn(arguementType params) {
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

