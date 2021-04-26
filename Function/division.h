#ifndef DIVISION_H
#define DIVISION_H

#include "binaryoperation.h"

class Division : public BinaryOperation
{
public:
    Division(double*l, double*r)
        :lhs(l),rhs(r)
    {
    }

    void compute() override
    {
        //необходимо добавить проверку деления на ноль
        *lhs = *lhs / *rhs;
    }
    Operation* clone() const override
    {
        return new Division(lhs,rhs);
    }
    Operation* clone(double* register1,double* register2) const override
    {
        return new Division(register1,register2);
    }
    char symbolOfOperation()const override
    {
        return '/';
    }
    ~Division()
    {
    }

private:
    double*lhs;
    double*rhs;
};

#endif // DIVISION_H
