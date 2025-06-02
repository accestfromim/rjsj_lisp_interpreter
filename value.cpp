#include<string>
#include<sstream>
#include<iomanip>
#include<cmath>
#include "value.h"
#include"eval_env.h"


std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

std::string NumericValue::toString() const {
    if (std::abs(value - int(value)) < 1e-6) 
        return std::to_string(int(value));
    else 
        return std::to_string(value);
}

std::string StringValue::toString() const {
    std::stringstream s;
    s << std::quoted(value);
    return s.str();
}

std::string NilValue::toString() const {
    return std::string("()");
}

std::string SymbolValue::toString() const {
    return value;
}

//void makeList(std::string& tgt) {
//    for (int i = 0; i < tgt.length(); i++)
//        if (tgt.at(i) == '\"')
//            while (tgt.at(++i) != '\"') /*do nothing*/;
//        else if (tgt.at(i) == '(' || tgt.at(i) == ')')
//            tgt.erase(i--, 1);
//    tgt.insert(0, "(");
//    tgt.push_back(')');
//}

std::string PairValue::toString() const {
    std::string s;
    s.append(car()->toString());
    if (cdr()->getType() == ValueType::PAIR_VALUE) {
        std::string _cdr = cdr()->toString();
        _cdr.erase(_cdr.begin());
        _cdr.pop_back();
        s.append(std::string(" ") + _cdr);
    } else if (cdr()->getType() != ValueType::NIL_VALUE)
        s.append(" . " + cdr()->toString());
    s.insert(s.begin(), '(');
    s.push_back(')');
    //makeList(s);
    return s;
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result{this->car()};

    if (this->cdr()->getType() == ValueType::NIL_VALUE) {
        //result.push_back(this->cdr());
        return result;
    } else if (this->cdr()->getType() != ValueType::PAIR_VALUE) {
        throw LispError("Illegal Use of Dot");
        /*result.push_back(this->cdr());
        return result;*/
    } else {
        std::string quote_htn = static_cast<PairValue&>(*this->cdr()).car()->toString();
        if (quote_htn == "quote" || quote_htn == "quasiquote" ||
            quote_htn == "unquote") {
            result.push_back(this->cdr());
        } else {
            std::vector<ValuePtr> _cdr =
                static_cast<PairValue&>(*this->cdr()).toVector();
            for (auto i : _cdr) result.push_back(i);
        }
    }
    return result;
}

std::string BuiltinProcValue::toString() const {
    return std::string("#<procedure>");
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) {
    std::shared_ptr<EvalEnv> itsEnv = itsParentEnv->createChild(params, args);
    for (int i = 0; i < body.size() - 1; ++i) 
        itsEnv->eval(body.at(i));
    return itsEnv->eval(body.at(body.size() - 1));
}
