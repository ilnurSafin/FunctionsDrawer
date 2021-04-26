#ifndef ASSINGMENTREGISTERTOVARIABLE_H
#define ASSINGMENTREGISTERTOVARIABLE_H

#include <string>
#include "operation.h"

class AssingmentRegisterToVariable : public Operation
{
public:
    AssingmentRegisterToVariable(double* reg,std::string operand,std::map<std::string,double>& vars)
        :_register(reg),variables(vars), identifier(operand)
    {
    }
    void compute() override
    {
        variables[identifier] = *_register;
    }
    ~AssingmentRegisterToVariable() override
    {
    }

private:
    double*_register;

    std::string identifier;
    std::map<std::string,double>& variables;
};


#endif // ASSINGMENTREGISTERTOVARIABLE_H
