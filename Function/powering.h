#ifndef POWERING_H
#define POWERING_H

#include <cmath>
#include "binaryoperation.h"

class Powering : public BinaryOperation
{
public:
    Powering(double*reg1,double*reg2)
        :_register1(reg1),_register2(reg2)
    {
    }

    void compute() override
    {
        *_register1 = std::pow(*_register1, *_register2);
    }
    Operation* clone() const override
    {
        return new Powering(_register1,_register2);
    }
    Operation* clone(double* register1,double* register2) const override
    {
        return new Powering(register1,register2);
    }
    char symbolOfOperation()const override
    {
        return '^';
    }
    ~Powering()
    {
    }
private:
    double*_register1;
    double*_register2;
};


#endif // POWERING_H
