#include "value.h"
#include<string>
#include<sstream>
#include<iomanip>

std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

std::string NumericValue::toString() const {
    if (value - int(value) < 1e-6) 
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

void makeList(std::string& tgt) {
    for (int i = 0; i < tgt.length(); i++)
        if (tgt.at(i) == '\"')
            while (tgt.at(++i) != '\"') /*do nothing*/;
        else if (tgt.at(i) == '(' || tgt.at(i) == ')')
            tgt.erase(i--, 1);
    tgt.insert(0, "(");
    tgt.push_back(')');
}

std::string PairValue::toString() const {
    std::string s;
    s.append(car()->toString());
    if (cdr()->getType() == ValueType::PAIR_VALUE)
        s.append(std::string(" ") + cdr()->toString());
    else if (cdr()->getType() != ValueType::NIL_VALUE)
        s.append(" . "+cdr()->toString());
    makeList(s);
    return s;
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result{this->car()};

    if (this->cdr()->getType() == ValueType::NIL_VALUE) {
        return result;
    } else if (this->cdr()->getType() != ValueType::PAIR_VALUE) {
        result.push_back(this->cdr());
        return result;
    } else {
        std::vector<ValuePtr> _cdr=static_cast<PairValue&>(*this->cdr()).toVector();
        for (auto i : _cdr) result.push_back(i);
    }
    return result;
}

std::string BuiltinProcValue::toString() const {
    return std::string("#<procedure>");
}
