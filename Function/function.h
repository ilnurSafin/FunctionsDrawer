#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <list>
#include <string>
#include <map>

#include "operation.h"
#include "binaryoperation.h"

#include "powering.h"
#include "division.h"
#include "multiplication.h"
#include "remainder.h"
#include "addition.h"
#include "subtraction.h"

#include "indexwithwhatsindexed.h"

class Function
{
public:
    Function(std::string textOfFunction, std::string nameOfVariable="x");
    ~Function();
    Function& operator=(const Function& other);
    Function(const Function& other);
    bool setNameOfVariable(std::string nameOfVariable);
    std::string variableName()const;
    double operator ()(double x);
    const Function& operator=(const std::string& textOfFunction);
    const std::string & textOfFunction()const;
    static bool textOfFunctionIsOkWithNullingCounterOfTempVariables(std::string textOfFunction, std::string nameOfVariable);
    static void addBinaryOperation(BinaryOperation* operation, size_t priority);
    static void addNestedFunction(const std::string& nameOfFunction,double (*pointerToFunction)(double));

    class ErrorInTextOfFunction
    {
    public:
        ErrorInTextOfFunction(const std::string& error)
            :textOfError(error)
        {
        }
        const std::string& what()const
        {
            return textOfError;
        }
    private:
        std::string textOfError;
    };
private:
    void generateOperations(std::string textOfFunction);
    void addOperations(std::string& textOfFunction, const std::vector<BinaryOperation *> &vOperators);
private:
    class NestedFunction
    {
    public:
        NestedFunction(const std::string name, double (*pToF)(double))
            :nameOfFunction(name), f(pToF)
        {
        }
        operator std::string()
        {
            return nameOfFunction;
        }

        std::string nameOfFunction;
        double (*f)(double);
    };

    static bool textOfFunctionIsOk(std::string textOfFunction, std::string nameOfVariable);
    static bool operandIsOk(std::string textOfOperand, std::string nameOfVariable);
    static bool operatorsIsOk(std::string &textOfFunction, std::list<std::string> operators, std::string nameOfVariable);
    static std::string deleteAmountSpacers(std::string s);
    static IndexWithWhatsIndexed lessedIndexOfFromList(std::string& s, std::list<std::string>, int pos=0);
private:
    std::string name_of_var;
    static std::vector<NestedFunction> availableFunctions;
    static int counterOfTempVariables;

    std::vector<Operation*> operations;
    std::map<std::string,double> variables;
    double register1=0;
    double register2=0;
    std::string sourceText;

    static Powering powering;

    static Multiplication multiplication;
    static Division division;
    static Remainder remainder;

    static Addition addition;
    static Subtraction subtraction;

    static std::vector< std::vector<BinaryOperation*> > binaryOperations;//операциям каждой н-ой строке соответсвует н-ый приоритет
};

#endif // FUNCTION_H
