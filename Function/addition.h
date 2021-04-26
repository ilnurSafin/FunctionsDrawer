#ifndef ADDITION_H
#define ADDITION_H

#include "binaryoperation.h"

class Addition : public BinaryOperation
{
public:
    Addition(double*l, double*r)
        :lhs(l),rhs(r)
    {
    }

    void compute() override
    {
        *lhs = *lhs + *rhs;
    }

    Operation* clone() const override
    {
        return new Addition(lhs,rhs);
    }
    Operation* clone(double* register1,double* register2) const override
    {
        return new Addition(register1,register2);
    }
    char symbolOfOperation()const override
    {
        return '+';
    }
    ~Addition()
    {
    }
private:
    double*lhs;
    double*rhs;
};


#endif // ADDITION_H
