#ifndef ASSINGMENTOPERANDTOREGISTER_H
#define ASSINGMENTOPERANDTOREGISTER_H

#include <map>
#include <string>
#include "operation.h"

class AssingmentOperandToRegister : public Operation
{
public:
    AssingmentOperandToRegister(double* reg,std::string operand,const std::map<std::string,double>& vars)
        :_register(reg),variables(vars)
    {
        if(operand[0]>='0' && operand[0]<='9')
        {
            operandIsConst = true;
            valueOfConst   = std::stod(operand);
        }
        else
            identifier = operand;
    }
    void compute() override
    {
        if(operandIsConst)
            *_register = valueOfConst;
        else
            *_register = variables.at(identifier);
    }
    ~AssingmentOperandToRegister()
    {
    }

private:
    double*_register;

    bool operandIsConst=false;
    double valueOfConst=0.0;

    std::string identifier;
    const std::map<std::string,double>& variables;
};

#endif // ASSINGMENTOPERANDTOREGISTER_H
