#ifndef CALLINGFUNCTION_H
#define CALLINGFUNCTION_H

#include "operation.h"

typedef double (*pToF)(double);

class CallingFunction : public Operation
{
public:
    CallingFunction(double*reg, pToF f)
        :_register(reg), fForCalling(f)
    {
    }

    void compute() override
    {
        *_register = fForCalling(*_register);
    }

    ~CallingFunction() override
    {
    }

private:
    double*_register;
    pToF fForCalling;
};

#endif // CALLINGFUNCTION_H
