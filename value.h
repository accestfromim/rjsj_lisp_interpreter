#ifndef VALUE_H
#define VALUE_H
#include<iostream>
#include<vector>

#include"error.h"

enum class ValueType {
    BOOLEAN_VALUE,
    NUMERIC_VALUE,
    STRING_VALUE,
    NIL_VALUE,
    SYMBOL_VALUE,
    PAIR_VALUE,
    BUILTINPROC_VALUE
};
class Value;
using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);
//基类在这
class Value {
private:
    ValueType type;

protected:
    Value(ValueType type) : type{type} {}

public:
    ValueType getType() const{
        return type;
    }
    virtual std::string toString() const = 0;
};
//bool 数据类型
class BooleanValue : public Value {
private:
    bool value;

public:
    BooleanValue(bool v) : Value(ValueType::BOOLEAN_VALUE), value{v} {}
    bool getValue() const{
        return value;
    }
    std::string toString() const override;
};
//数 数据类型
class NumericValue : public Value {
private:
    double value;

public:
    NumericValue(double v) : Value(ValueType::NUMERIC_VALUE), value{v} {}
    double getValue() const{
        return value;
    }
    std::string toString() const override;
};
//string 数据类型
class StringValue : public Value {
private:
    std::string value;

public:
    StringValue(const std::string& v)
        : Value(ValueType::STRING_VALUE), value{v} {}
    const std::string& getValue() const {
        return value;
    }
    std::string toString() const override;
};
//空表
class NilValue : public Value {
public:
    NilValue() : Value(ValueType::NIL_VALUE) {}
    void getValue() const = delete;
    std::string toString() const override;
};
//符号 数据类型
class SymbolValue : public Value {
private:
    std::string value;

public:
    SymbolValue(const std::string& v)
        : Value(ValueType::SYMBOL_VALUE), value{v} {}
    const std::string& getValue()const {
        return value;
    }
    std::string toString() const override;
};
//对子 数据类型
class PairValue :public Value {
private:
    ValuePtr lhs;
    ValuePtr rhs;

public:
    PairValue(const ValuePtr& l, const ValuePtr& r)
        : Value(ValueType::PAIR_VALUE), lhs{l}, rhs{r} {}
    const ValuePtr& car() const{
        return lhs;
        //return const_cast<ValuePtr&>(lhs);
    }
    const ValuePtr& cdr() const{
        return rhs;
        //return const_cast<ValuePtr&>(rhs);
    }
    std::string toString() const override;
    std::vector<ValuePtr> toVector() const;
};
//内置过程类
class BuiltinProcValue : public Value {
private:
    BuiltinFuncType* func{nullptr}; 
    // [...]

public:
    BuiltinProcValue(BuiltinFuncType* f):func{f},Value(ValueType::BUILTINPROC_VALUE) {}
    // 直接返回 #<procedure> 就可以，我们不做更多要求。
    BuiltinFuncType* getFunc() const {
        return func;
    }
    std::string toString() const override;
    /*operator NumericValue() {
        throw SyntaxError("Can\'t turn Proc to Numeric");
        return NumericValue(0);
    }*/
};
#endif
