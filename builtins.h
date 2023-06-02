#ifndef BUILTINS_H
#define BUILTINS_H
#include <unordered_map>
#include "value.h"
extern std::unordered_map<std::string, ValuePtr> Main_dict;
using arguementType=const std::vector<ValuePtr>&;
ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr minus(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr multiply(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr devide(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr list(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr list_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr displayln(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr LispExit(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr atom_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr boolean_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr integer_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr number_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr null_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr pair_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr procedure_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr string_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr symbol_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr listLength(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _cdr(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _car(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _append(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _abs(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _expt(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _quotient(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _modulo(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _remainder(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr num_equal(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr num_less(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr num_greater(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr num_less_or_equal(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr num_greater_or_equal(const std::vector<ValuePtr>& params,
                              EvalEnv& env);
ValuePtr even_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr odd_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr zero_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _eval(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _apply(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr equal_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr eq_htn(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _error(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _map(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _filter(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _reduce(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr _not(const std::vector<ValuePtr>& params, EvalEnv& env);



#endif
