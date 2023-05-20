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
