#ifndef SUBTRACTION_H
#define SUBTRACTION_H

#include "binaryoperation.h"

class Subtraction : public BinaryOperation
{
public:
    Subtraction(double*l, double*r)
        :lhs(l),rhs(r)
    {
    }

    void compute() override
    {
        *lhs = *lhs - *rhs;
    }
    Operation* clone() const override
    {
        return new Subtraction(lhs,rhs);
    }
    Operation* clone(double* register1,double* register2) const override
    {
        return new Subtraction(register1,register2);
    }
    char symbolOfOperation()const override
    {
        return '-';
    }
    ~Subtraction()
    {
    }
private:
    double*lhs;
    double*rhs;
};


#endif // SUBTRACTION_H
