#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H

#include "binaryoperation.h"

class Multiplication : public BinaryOperation
{
public:
    Multiplication(double*l, double*r)
        :lhs(l),rhs(r)
    {
    }

    void compute() override
    {
        *lhs = *lhs * *rhs;
    }
    Operation* clone() const override
    {
        return new Multiplication(lhs,rhs);
    }
    Operation* clone(double* register1,double* register2) const override
    {
        return new Multiplication(register1,register2);
    }
    char symbolOfOperation()const override
    {
        return '*';
    }
    ~Multiplication()
    {
    }

private:
    double*lhs;
    double*rhs;
};

#endif // MULTIPLICATION_H
