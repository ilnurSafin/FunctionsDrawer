#ifndef BINARYOPERATION_H
#define BINARYOPERATION_H

#include "operation.h"

class BinaryOperation : public Operation
{
public:
    virtual Operation* clone()const=0;
    virtual Operation* clone(double* register1,double* register2)const=0;
    virtual char symbolOfOperation()const = 0;
    ~BinaryOperation() override
    {
    }
};

#endif // BINARYOPERATION_H
