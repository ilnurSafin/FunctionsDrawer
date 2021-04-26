#ifndef REMAINDER_H
#define REMAINDER_H

#include "binaryoperation.h"
#include <cmath>

class Remainder : public BinaryOperation
{
public:
    Remainder(double*l, double*r)
        :lhs(l),rhs(r)
    {
    }

    void compute() override
    {
        //добавить проверку деления на ноль
        *lhs = std::fmod(*lhs,*rhs);
    }
    Operation* clone() const override
    {
        return new Remainder(lhs,rhs);
    }
    Operation* clone(double* register1,double* register2) const override
    {
        return new Remainder(register1,register2);
    }
    char symbolOfOperation()const override
    {
        return '%';
    }
    ~Remainder()
    {
    }
private:
    double*lhs;
    double*rhs;
};

#endif // REMAINDER_H
