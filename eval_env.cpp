#include "eval_env.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>

#include "error.h"
#include "forms.h"
// 允许接收对子作为求值的eval
// ValuePtr EvalEnv::reinterpretDefinedValue(ValuePtr definedValue) {
//     if (definedValue->getType() == ValueType::BOOLEAN_VALUE ||
//         definedValue->getType() == ValueType::NUMERIC_VALUE ||
//         definedValue->getType() == ValueType::STRING_VALUE) {
//         return definedValue;
//     } else
//     if (definedValue->getType() == ValueType::PAIR_VALUE) {
//         if (static_cast<PairValue&>(*definedValue).cdr()->getType() ==
//             ValueType::NIL_VALUE)
//             return
//             reinterpretDefinedValue(static_cast<PairValue&>(*definedValue).car());
//          else {
//             return definedValue;
//         }
//     } else if (definedValue->getType() == ValueType::SYMBOL_VALUE) {
//         // assert(static_cast<SymbolValue&>(*expr).getValue() == "accest");
//         auto value =
//         dict.find(static_cast<SymbolValue&>(*definedValue).getValue()); if
//         (value != dict.end())  // return value->second;
//             return reinterpretDefinedValue(value->second);
//         else
//             throw LispError("Not Defined Symbol \'" +
//             definedValue->toString() +
//                             "\'");
//     } else if (definedValue->getType() == ValueType::NIL_VALUE) {
//         throw LispError("Evaluating nil is prohibited.");
//     } else {
//         throw LispError("Inner System Error");
//     }
// }

/// <summary>
///
/// </summary>
/// <param name="expr"></param>
/// <returns></returns>
ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->getType() == ValueType::BOOLEAN_VALUE ||
        expr->getType() == ValueType::NUMERIC_VALUE ||
        expr->getType() == ValueType::STRING_VALUE) {
        return expr;
    } else if (expr->getType() == ValueType::PAIR_VALUE) {
        ValuePtr procedure = static_cast<PairValue&>(*expr).car();
        if (procedure->getType() == ValueType::PAIR_VALUE) {
            procedure = eval(procedure);
            if (procedure->getType() != ValueType::BUILTINPROC_VALUE &&
                procedure->getType() != ValueType::LAMBDA_VALUE)
                throw LispError("Must Start With a Procedure");

            std::vector<ValuePtr> args =
                evalList(static_cast<PairValue&>(*expr).cdr());
            return apply(procedure, args);
        }

        if (procedure->getType() != ValueType::SYMBOL_VALUE) {
            throw LispError("Must Start With a Procedure");
        } else if (SPECIAL_FORMS.find(procedure->toString()) !=
                   SPECIAL_FORMS.end()) {
            SpecialFormType* specForm =
                SPECIAL_FORMS.find(procedure->toString())->second;
            ValuePtr cdr = static_cast<PairValue&>(*expr).cdr();
            if (cdr->getType() == ValueType::PAIR_VALUE) {
                return specForm(static_cast<PairValue&>(*cdr).toVector(),
                                *this);
            } else {
                return specForm(std::vector<ValuePtr>{cdr}, *this);
            }
            //(static_cast<SymbolValue&>(*procedure).getValue() ==
            //           "define") {
            //    auto definePair = static_cast<PairValue&>(*expr).cdr();
            //    // // // // // // // // // // //
            //    if (definePair->getType() != ValueType::PAIR_VALUE)
            //        throw LispError("Procedure \'Define\' Must Has Two
            //        Argruments");

            //    if (static_cast<PairValue&>(*definePair).cdr()->getType()
            //    ==
            //        ValueType::NIL_VALUE)
            //        throw LispError("Procedure \'Define\' Must Has Two
            //        Argruments");

            //    if (static_cast<PairValue&>(*definePair).car()->getType()
            //    !=
            //        ValueType::SYMBOL_VALUE)
            //        throw LispError("This Type of Value Can't be
            //        Defined");
            //    // // // // // // // // // // // //
            //    std::vector<ValuePtr> vec =
            //        static_cast<PairValue&>(*definePair).toVector();
            //    auto hasDefined = dict.find(vec.at(0)->toString());
            //    if (hasDefined != dict.end()) dict.erase(hasDefined);
            //    dict.insert(std::pair<std::string,
            //    ValuePtr>(vec.at(0)->toString(),
            //                                                 eval(vec.at(1))));
            //    // auto hasDefined =
            //    dict.find(static_cast<PairValue&>(*definePair)
            //    //                                 .car()
            //    //                                 ->toString()
            //    /*getvalue()*/);
            //    // if (hasDefined != dict.end()) dict.erase(hasDefined);

            //    // auto evalable =
            //    static_cast<PairValue&>(*definePair).cdr();
            //    // assert(evalable->getType() == ValueType::PAIR_VALUE);

            //    // dict.insert(std::pair<std::string, ValuePtr>(
            //    //     static_cast<SymbolValue&>(
            //    //         *static_cast<PairValue&>(*definePair).car())
            //    //         .getValue(),
            //    //     reinterpretDefinedValue(evalable)));
            //    //  assert(dict.find("accest") != dict.end());
            //    return std::make_shared<NilValue>();
        } else {
            ValuePtr proc = eval(static_cast<PairValue&>(*expr).car());
            std::vector<ValuePtr> args =
                evalList(static_cast<PairValue&>(*expr).cdr());
            return apply(proc, args);
            // throw LispError("Eval::eval::IF_PAIR:IF_DEFINE:ELSE");
        }
    } else if (expr->getType() == ValueType::SYMBOL_VALUE) {
        // assert(static_cast<SymbolValue&>(*expr).getValue() == "accest");
        ValuePtr value =
            lookupBinding(static_cast<SymbolValue&>(*expr).getValue());
        if (value != nullptr)  // return value->second;
            return value;
        else {
            throw LispError("Not Defined Symbol \'" + expr->toString() +
                            "\'");  // getValue看起来标准些，不过实际上一样
        }
    } else if (expr->getType() == ValueType::BUILTINPROC_VALUE) {
        return expr;
    } else if (expr->getType() == ValueType::NIL_VALUE) {
        throw LispError("Evaluating nil is prohibited.");
    } else {
        throw LispError("Unimplemented");
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    if (expr->getType() == ValueType::PAIR_VALUE) {
        std::ranges::transform(static_cast<PairValue&>(*expr).toVector(),
                               std::back_inserter(result),
                               [this](ValuePtr v) { return this->eval(v); });
    } else {
        if (expr->getType() == ValueType::NIL_VALUE) return result;
        std::ranges::transform(std::vector<ValuePtr>{expr},
                               std::back_inserter(result),
                               [this](ValuePtr v) { return this->eval(v); });
    }
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr>& args) {
    if (proc->getType() == ValueType::BUILTINPROC_VALUE) {
        return static_cast<BuiltinProcValue&>(*proc).getFunc()(args);
    } else if (proc->getType() == ValueType::LAMBDA_VALUE) {
        return static_cast<LambdaValue&>(*proc).apply(args);
    } else {
        throw LispError("Must Start With a Procedure");
    }
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(
    const std::vector<std::string>& params, const std::vector<ValuePtr>& args) {
    if (params.size() != args.size())
        throw LispError("Number of Arguements is not Right: " +
                        std::to_string(static_cast<int>(params.size())) +
                        " Needed");

    std::unordered_map<std::string, ValuePtr> map;
    for (int i = 0; i < params.size(); ++i)
        map.insert(std::pair<std::string, ValuePtr>(params.at(i), args.at(i)));
    return EvalEnv::createGlobal(map, this->shared_from_this());
}

ValuePtr EvalEnv::lookupBinding(const std::string& name) const {
    auto found = dict.find(name);
    if (found != dict.end()) {
        return found->second;
    } else {
        if (parent == nullptr) {
            return nullptr;
        }

        return parent->lookupBinding(name);
    }
}
