#include "builtins.h"

ValuePtr add(const std::vector<ValuePtr>& params) {
    double result = 0;
    for (const auto& i : params) {
        if (i->getType() != ValueType::NUMERIC_VALUE) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += static_cast<NumericValue&>(*i).getValue();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("Print Just Allow One Element");
    } else {
        std::cout << params.at(0)->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}
