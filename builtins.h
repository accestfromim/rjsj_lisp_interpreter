#ifndef BUILTINS_H
#define BUILTINS_H

#include "value.h"
#include "error.h"
using arguementType=const std::vector<ValuePtr>&;
ValuePtr add(arguementType params);
ValuePtr minus(arguementType params);
ValuePtr print(arguementType params);
ValuePtr multiply(arguementType params);
ValuePtr devide(arguementType params);
ValuePtr cons(arguementType params);
ValuePtr list(arguementType params);
ValuePtr list_htn(arguementType params);
ValuePtr display(arguementType params);
ValuePtr LispExit(arguementType params);
ValuePtr newline(arguementType params);
ValuePtr atom_htn(arguementType params);
ValuePtr boolean_htn(arguementType params);
ValuePtr integer_htn(arguementType params);
ValuePtr number_htn(arguementType params);
ValuePtr null_htn(arguementType params);
ValuePtr pair_htn(arguementType params);
ValuePtr procedure_htn(arguementType params);
ValuePtr string_htn(arguementType params);
ValuePtr symbol_htn(arguementType params);
ValuePtr listLength(arguementType params);
ValuePtr _cdr(arguementType params);
ValuePtr _car(arguementType params);
ValuePtr _append(arguementType params);



//ValuePtr addVal = std::make_shared<BuiltinProcValue>(&add);





#endif
