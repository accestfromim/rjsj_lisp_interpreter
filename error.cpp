#include<string>
#include"error.h"

void ThrowErrorForStrictProc(const std::string& name, int n) {
    throw LispError("Proc \'" + name + "\' Just Allow " + std::to_string(n) +
                    " Arguements");
}
