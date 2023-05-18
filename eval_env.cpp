#include "eval_env.h"
#include "error.h"
#include<cassert>

//允许接收对子作为求值的eval
ValuePtr EvalEnv::reinterpretDefinedValue(ValuePtr definedValue) {
    if (definedValue->getType() == ValueType::BOOLEAN_VALUE ||
        definedValue->getType() == ValueType::NUMERIC_VALUE ||
        definedValue->getType() == ValueType::STRING_VALUE) {
        return definedValue;
    } else 
    if (definedValue->getType() == ValueType::PAIR_VALUE) {
        if (static_cast<PairValue&>(*definedValue).cdr()->getType() ==
            ValueType::NIL_VALUE)
            return reinterpretDefinedValue(static_cast<PairValue&>(*definedValue).car());
        else
            return definedValue;
    } else if (definedValue->getType() == ValueType::SYMBOL_VALUE) {
        // assert(static_cast<SymbolValue&>(*expr).getValue() == "accest");
        auto value = dict.find(static_cast<SymbolValue&>(*definedValue).getValue());
        if (value != dict.end())  // return value->second;
            return reinterpretDefinedValue(value->second);
        else
            throw LispError("Not Defined Symbol \'" + definedValue->toString() +
                            "\'");
    } else if (definedValue->getType() == ValueType::NIL_VALUE) {
        throw LispError("Evaluating nil is prohibited.");
    } else {
        throw LispError("Inner System Error");
    }
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->getType()==ValueType::BOOLEAN_VALUE
        ||expr->getType()==ValueType::NUMERIC_VALUE
        ||expr->getType()==ValueType::STRING_VALUE) {
        return expr;
    } else if (expr->getType()==ValueType::PAIR_VALUE) {
        auto procedure = static_cast<PairValue&>(*expr).car();
        if (procedure->getType() != ValueType::SYMBOL_VALUE)
            throw LispError("Must Start With a procedure");//对子的car必须是过程
        if (static_cast<SymbolValue&>(*procedure).getValue() == "define") {
            auto definePair = static_cast<PairValue&>(*expr).cdr();

            if (definePair->getType() != ValueType::PAIR_VALUE)
                throw LispError("Procedure \'Define\' Must Has Two Argruments");

            if (static_cast<PairValue&>(*definePair).cdr()->getType() ==
                ValueType::NIL_VALUE)
                throw LispError("Procedure \'Define\' Must Has Two Argruments");

            if (static_cast<PairValue&>(*definePair).car()->getType() !=
                ValueType::SYMBOL_VALUE)
                throw LispError("This Type of Value Can't be Defined");

            auto hasDefined = dict.find(static_cast<PairValue&>(*definePair)
                                            .car()
                                            ->toString() /*getvalue()*/);
            if (hasDefined != dict.end()) dict.erase(hasDefined);

            auto evalable = static_cast<PairValue&>(*definePair).cdr();
            assert(evalable->getType() == ValueType::PAIR_VALUE);

            // reinterpretDefinedValue(evalable);

            dict.insert(std::pair<std::string, ValuePtr>(
                static_cast<SymbolValue&>(
                    *static_cast<PairValue&>(*definePair).car())
                    .getValue(),
                reinterpretDefinedValue(evalable)));
            // assert(dict.find("accest") != dict.end());
            return std::make_shared<NilValue>();
        } else {
            throw LispError("Inner System Error");
        }
    } else if (expr->getType()==ValueType::SYMBOL_VALUE) {
        //assert(static_cast<SymbolValue&>(*expr).getValue() == "accest");
        auto value = dict.find(static_cast<SymbolValue&>(*expr).getValue());
        if (value != dict.end()) //return value->second;
            return reinterpretDefinedValue(value->second);
        else {
            throw LispError("Not Defined Symbol \'" + expr->toString() +
                            "\'");  // getValue看起来标准些，不过实际上一样
        }
    } else if (expr->getType() == ValueType::NIL_VALUE) {
        throw LispError("Evaluating nil is prohibited.");
    } else {
        throw LispError("Unimplemented");
    }
}
