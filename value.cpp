#include "value.h"
#include<string>
#include<sstream>
#include<iomanip>

std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

std::string NumericValue::toString() const {
    if (value - int(value) < 1e-10) 
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
            tgt.erase(i, 1), --i;
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
