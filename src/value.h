#ifndef VALUE_H
#define VALUE_H
#include<iostream>
#include<vector>
#include <memory>
#include"error.h"


enum class ValueType {
	BOOLEAN_VALUE,
	NUMERIC_VALUE,
	STRING_VALUE,
	NIL_VALUE,
	SYMBOL_VALUE,
	PAIR_VALUE,
	BUILTINPROC_VALUE,
	LAMBDA_VALUE
};
class Value;
class EvalEnv;
using ValuePtr = std::shared_ptr<Value>;
using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&,EvalEnv&);

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

class NilValue : public Value {
public:
	NilValue() : Value(ValueType::NIL_VALUE) {}
	void getValue() const = delete;
	std::string toString() const override;
};

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

class BuiltinProcValue : public Value {
private:
	BuiltinFuncType* func{nullptr}; 


public:
	BuiltinProcValue(BuiltinFuncType* f):func{f},Value(ValueType::BUILTINPROC_VALUE) {}

	BuiltinFuncType* getFunc() const {
		return func;
	}
	std::string toString() const override;
	/*operator NumericValue() {
		throw SyntaxError("Can\'t turn Proc to Numeric");
		return NumericValue(0);
	}*/
};
class LambdaValue : public Value {
private:
	std::vector<std::string> params;
	std::vector<ValuePtr> body;
	std::shared_ptr<EvalEnv> itsParentEnv;
	// [...]
public:
	LambdaValue(const std::vector<std::string>& params,
				const std::vector<ValuePtr>& body, std::shared_ptr<EvalEnv> env)
		: Value(ValueType::LAMBDA_VALUE), params{params}, body{body}, itsParentEnv{env} {}
	std::string toString() const override {
		return std::string("#<procedure>");
	};
	ValuePtr apply(const std::vector<ValuePtr>& args);
};
#endif
