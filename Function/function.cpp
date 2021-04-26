#include <string>
#include <iostream>
#include <cmath>
#include <regex>

#include "function.h"

#include "powering.h"
#include "division.h"
#include "multiplication.h"
#include "remainder.h"
#include "addition.h"
#include "subtraction.h"
#include "assingmentoperandtoregister.h"
#include "assingmentregistertovariable.h"
#include "callingfunction.h"

#include <QtCore>

std::vector<Function::NestedFunction> Function::availableFunctions = {{"cos",std::cos},{"sin",std::sin},{"tg",std::tan},/*{"ctg",...},*/{"arccos",std::acos},{"arcsin",std::asin},{"arctg",std::atan},/*{"arcctg",...},*/{"lg",std::log}/*,{"ln",...}*/,{"abs",std::fabs}};
int Function::counterOfTempVariables = 0;

Powering       Function::powering = Powering(0,0);

Multiplication Function::multiplication = Multiplication(0,0);
Division       Function::division = Division(0,0);
Remainder      Function::remainder = Remainder(0,0);

Addition       Function::addition = Addition(0,0);
Subtraction    Function::subtraction = Subtraction(0,0);

std::vector< std::vector<BinaryOperation*> > Function::binaryOperations = {{&Function::powering},{&Function::multiplication, &Function::division,&Function::remainder},{&Function::addition,&Function::subtraction}};//операциям каждой н-ой строке соответсвует н-ый приоритет



Function::Function(std::string textOfFunction, std::string nameOfVariable)
{
    if( ! setNameOfVariable(nameOfVariable))
      throw ErrorInTextOfFunction("name of variable is not correct");
    if( ! textOfFunctionIsOk(textOfFunction,name_of_var))
        throw ErrorInTextOfFunction("text of function is not correct");
    sourceText = textOfFunction;
    generateOperations(textOfFunction);
}

Function::~Function()
{
    for(Operation* operation : operations)
        delete operation;
}

Function& Function::operator=(const Function& other)
{
    for(Operation* operation : operations)
        delete operation;
    operations.clear();
    register1=0;
    register2=0;
    variables.clear();
    name_of_var = other.name_of_var;
    generateOperations(sourceText = other.sourceText);
    return *this;
}
Function::Function(const Function& other)
{
    register1=0;
    register2=0;
    name_of_var = other.name_of_var;
    generateOperations(sourceText = other.sourceText);
}

bool Function::setNameOfVariable(std::string nameOfVariable)
{
  nameOfVariable = deleteAmountSpacers(nameOfVariable);

  if (nameOfVariable.size()==0)
    return false;
  for(int i=0; i<nameOfVariable.size(); ++i)
  {
    const char char_of_nameOfVariable = nameOfVariable[i];
    bool       char_of_nameOfVariable_is_alfa=false;
    for(char c='a'; c<='z'; ++c)
    {
      if(char_of_nameOfVariable==c)
        char_of_nameOfVariable_is_alfa=true;
    }
    for(char c='A'; c<='Z'; ++c)
    {
      if(char_of_nameOfVariable==c)
        char_of_nameOfVariable_is_alfa=true;
    }
    if( ! char_of_nameOfVariable_is_alfa)
      return false;
  }

  name_of_var = nameOfVariable;
  return true;
}

std::string Function::variableName() const
{
    return name_of_var;
}

double Function::operator ()(double x)
{
    variables[name_of_var]=x;
    for(Operation* operation : operations)
    {
        operation->compute();
    }
    return register1;
}

const Function& Function::operator=(const std::string &textOfFunction)
{
    for(Operation* operation : operations)
        delete operation;
    operations.clear();
    register1=0;
    register2=0;
    variables.clear();
    if( ! textOfFunctionIsOk(textOfFunction,name_of_var))
        throw ErrorInTextOfFunction("text of function is not correct");
    generateOperations(sourceText = textOfFunction);
    return *this;
}

const std::string &Function::textOfFunction() const
{
    return sourceText;
}

void Function::generateOperations(std::string textOfFunction)
{
    /*if(textOfFunction.size()==0)
        return false;*/
    int indexOfBeginBound=0;
    int indexOfEndBound=0;

    //проходимся по всем скобкам с самым низким уровнем вложенности, 1-ым
    while((indexOfBeginBound = textOfFunction.find('(',indexOfEndBound))!= -1)
    {
        indexOfEndBound = indexOfBeginBound+1;
        int counterOfNesting=1;
        //поиск закрывающей скобки
        for(; indexOfEndBound < textOfFunction.size() && counterOfNesting != 0; indexOfEndBound++)
            //                                           заходя в цикл "уровень вложенности"==1, т.к. зашли в (,открывающую скобку, следовательно, когда уровень вложенности снова станет == 0, тогда мы нашли закрывающую скобку
        {
            char c = textOfFunction[indexOfEndBound];
            if(c=='(')
                counterOfNesting++;
            else if(c==')')
                counterOfNesting--;
        }
        //проверка, что закрывающая скобка есть, она сделала уровень вложенности 0
        /*if(counterOfNesting!=0)
            return false;*/
        //рекурсивное генерирование операций выражения в скобках
        generateOperations(textOfFunction.substr(indexOfBeginBound+1,indexOfEndBound-indexOfBeginBound-2).data());
        /*if( ! textOfFunctionIsOk(textOfFunction.substr(indexOfBeginBound+1,indexOfEndBound-indexOfBeginBound-2).data()))
            return false;*/

        //проверка, что вызываемая функция есть, если перед скобками надпись
        bool nameOfFunctionIsFounded=false;
        int indexOfNameOfFunction=indexOfBeginBound-1;
        for(int indexOfEndOfNameOfFunction=-1; indexOfNameOfFunction>=0 && !nameOfFunctionIsFounded || indexOfNameOfFunction>=-1 && nameOfFunctionIsFounded; indexOfNameOfFunction--)
        {
            if(!nameOfFunctionIsFounded)
            //ищем последний символ функции
            {
                char c = textOfFunction[indexOfNameOfFunction];
                if(c==' ')
                //пробелы пропускаем
                    continue;
                else if((c>='a' && c<='z') || (c>='A' && c<='Z'))
                {
                    //нашли последний символ имени функции
                    nameOfFunctionIsFounded=true;
                    //сохраняем индекс конца имени функции
                    indexOfEndOfNameOfFunction=indexOfNameOfFunction;
                }
                else
                //если перед скобками не символ, а цифра или знак, или что-то иное, то это просто измение приоритетов, не функция,выход из фикла проверки имени
                    break;
            }
            else
            //последний символ имени функции найден, поиск начала имени функции
            {
                if(indexOfNameOfFunction==-1 || !(textOfFunction[indexOfNameOfFunction]>='a' && textOfFunction[indexOfNameOfFunction]<='z' || textOfFunction[indexOfNameOfFunction]>='A' && textOfFunction[indexOfNameOfFunction]<='Z') )
                //если индекс отрицателен,т.е. не буква в качестве символа,характеризующая начало имени функции, не найдена ,т.е. это уже конец строки, или эта (не буква) найдена, то начало имени функции найдено
                {
                    std::string nameOfFunction = textOfFunction.substr(indexOfNameOfFunction+1,indexOfEndOfNameOfFunction-indexOfNameOfFunction);
                    //bool functionThatNeededIsFounded=false;
                    for(NestedFunction& availableFunction : availableFunctions)
                    //ищем имя функции среди доступных
                        if(availableFunction.nameOfFunction == nameOfFunction)
                        //если найдено запоминаем, что оно найдено и выходим
                        {
                            //functionThatNeededIsFounded=true;
                            operations.push_back(new CallingFunction(&register1,availableFunction.f));
                            break;
                        }

                   /* if(! functionThatNeededIsFounded)
                    // не найдено
                        return false;*/

                    //найдено, прерываем цикл поиска начала имени, так как имя нашли и проверили
                    break;
                }
            }
        }


        //замена скобок или вызова функции на временную переменную
        textOfFunction.erase(nameOfFunctionIsFounded ? indexOfNameOfFunction+1/*+1 так как искали не букву*/ : indexOfBeginBound,
                              nameOfFunctionIsFounded ? indexOfEndBound-indexOfNameOfFunction-1               : indexOfEndBound-indexOfBeginBound);

        std::string nameOfTempVariable = "tb"+std::to_string(counterOfTempVariables++)+"te";
        textOfFunction.insert(nameOfFunctionIsFounded ? indexOfNameOfFunction+1/*+1 так как искали не букву*/ : indexOfBeginBound , nameOfTempVariable.data());
        //так как поиск следующей открывающейся скобки будет происходить с индекса скобок, которые были удалены, меняем это индекс на следующий за именем временной переменной
        indexOfEndBound = nameOfFunctionIsFounded ? indexOfNameOfFunction+1/*+1 так как искали не букву*/+nameOfTempVariable.size() : indexOfBeginBound+nameOfTempVariable.size();

        operations.push_back(new AssingmentRegisterToVariable(&register1,std::string(nameOfTempVariable.data()),variables));
    }
    for(std::vector<BinaryOperation*>&  bOperationsSomePriority : binaryOperations)
        addOperations(textOfFunction,bOperationsSomePriority);

    operations.push_back(new AssingmentOperandToRegister(&register1,deleteAmountSpacers(std::string(textOfFunction.data())),variables));
    /*if(textOfFunction.indexOf(')',indexOfEndBound)!=-1)
    //проверка лишних закрывающих скобок
        return false;

    if( ! operatorsIsOk(textOfFunction,std::stringList() << "^"))
        return false;
    if( ! operatorsIsOk(textOfFunction,std::stringList() << "*" << "/" << "%"))
        return false;
    if( ! operatorsIsOk(textOfFunction,std::stringList() << "+" << "-"))
        return false;

    if( ! operandIsOk(textOfFunction))
        return false;*/

    //std::cout << textOfFunction << std::endl;
}

void Function::addOperations(std::string &textOfFunction, const std::vector<BinaryOperation *>& vOperators)
{
    std::list<std::string> operators;
    for(BinaryOperation* binaryOperator : vOperators)
    {
        operators.push_back(std::string(1,binaryOperator->symbolOfOperation()));
    }
    for(IndexWithWhatsIndexed indexOfPowerOperator=0; (indexOfPowerOperator=lessedIndexOfFromList(textOfFunction,operators,indexOfPowerOperator))!=-1;)
    {
        /*if(indexOfPowerOperator==0 || indexOfPowerOperator==textOfFunction.size()-1)
        //оператор стоит в самом конце или начале,т.е. априори нет одного из операндов
            return false;*/

        int indexOfLeftOperandFromPower=indexOfPowerOperator-1;//индекс первого символа левого операнда
        for(; indexOfLeftOperandFromPower>0 ;indexOfLeftOperandFromPower--)
        //поиск начала левого операнда
        {
            char c = textOfFunction[indexOfLeftOperandFromPower];
            if(c=='-'||c=='+'||c=='*'||c=='/'||c=='%'||c=='^')
            {
                indexOfLeftOperandFromPower++;
                break;
            }
        }
        int indexOfRightOperandFromPower=indexOfPowerOperator+1; //индекс символа следующего за последним символом правого операнда
        for(; indexOfRightOperandFromPower<textOfFunction.size() ;indexOfRightOperandFromPower++)
        //поиск конца правого операнда
        {
            char c = textOfFunction[indexOfRightOperandFromPower];
            if(c=='-'||c=='+'||c=='*'||c=='/'||c=='%'||c=='^')
            {
                break;
            }
        }
        std::string leftOperand  = textOfFunction.substr(indexOfLeftOperandFromPower, indexOfPowerOperator-indexOfLeftOperandFromPower);
        std::string rightOperand = textOfFunction.substr(indexOfPowerOperator+1,indexOfRightOperandFromPower-(indexOfPowerOperator+1));
        operations.push_back(new AssingmentOperandToRegister(&register1,deleteAmountSpacers(std::string(leftOperand.data())),variables));
        operations.push_back(new AssingmentOperandToRegister(&register2,deleteAmountSpacers(std::string(rightOperand.data())),variables));

        /*if( ! operandIsOk(std::string(leftOperand.data())) || ! operandIsOk(std::string(rightOperand.data())))
            return false;*/

        operations.push_back(vOperators[indexOfPowerOperator.whatIsIndexed]->clone(&register1,&register2));
        //замена возведения в степень на временную переменную
        textOfFunction.erase(indexOfLeftOperandFromPower,indexOfRightOperandFromPower-indexOfLeftOperandFromPower);
        std::string nameOfTempVariable = "tb"+std::to_string(counterOfTempVariables++)+"te";

        operations.push_back(new AssingmentRegisterToVariable(&register1,std::string(nameOfTempVariable.data()),variables));

        textOfFunction.insert(indexOfLeftOperandFromPower,nameOfTempVariable.data());
        //так как поиск следующей степни будет происходить с индекса текущей степни, которая была удалена, меняем это индекс на следующий за именем временной переменной
        indexOfPowerOperator = indexOfLeftOperandFromPower + nameOfTempVariable.size();

    }
}

bool Function::textOfFunctionIsOk(std::string textOfFunction, std::string nameOfVariable)
{
    if(textOfFunction.size()==0)
        return false;
    int indexOfBeginBound=0;
    int indexOfEndBound=0;

    //проходимся по всем скобкам с самым низким уровнем вложенности, 1-ым
    while((indexOfBeginBound = textOfFunction.find('(',indexOfEndBound))!= -1)
    {
        indexOfEndBound = indexOfBeginBound+1;
        int counterOfNesting=1;
        //поиск закрывающей скобки
        for(; indexOfEndBound < textOfFunction.size() && counterOfNesting != 0; indexOfEndBound++)
        {
            char c = textOfFunction[indexOfEndBound];
            if(c=='(')
                counterOfNesting++;
            else if(c==')')
                counterOfNesting--;
        }
        //проверка, что закрывающая скобка есть, она сделала уровень вложенности 0
        if(counterOfNesting!=0)
            return false;
        //рекурсивная проверка выражения в скобках
        if( ! textOfFunctionIsOk(textOfFunction.substr(indexOfBeginBound+1,indexOfEndBound-indexOfBeginBound-2).data(),nameOfVariable))
            return false;

        //проверка, что вызываемая функция есть, если перед скобками надпись
        bool nameOfFunctionIsFounded=false;
        int indexOfNameOfFunction=indexOfBeginBound-1;
        for(int indexOfEndOfNameOfFunction=-1; indexOfNameOfFunction>=0 && !nameOfFunctionIsFounded || indexOfNameOfFunction>=-1 && nameOfFunctionIsFounded; indexOfNameOfFunction--)
        {
            if(!nameOfFunctionIsFounded)
            //ищем последний символ функции
            {
                char c = textOfFunction[indexOfNameOfFunction];
                if(c==' ')
                //пробелы пропускаем
                    continue;
                else if((c>='a' && c<='z') || (c>='A' && c<='Z'))
                {
                    //нашли последний символ имени функции
                    nameOfFunctionIsFounded=true;
                    //сохраняем индекс конца имени функции
                    indexOfEndOfNameOfFunction=indexOfNameOfFunction;
                }
                else
                //если перед скобками не символ, а цифра или знак, или что-то иное, то это просто измение приоритетов, не функция,выход из фикла проверки имени
                    break;
            }
            else
            //последний символ имени функции найден, поиск начала имени функции
            {
                if(indexOfNameOfFunction==-1 || !(textOfFunction[indexOfNameOfFunction]>='a' && textOfFunction[indexOfNameOfFunction]<='z' || textOfFunction[indexOfNameOfFunction]>='A' && textOfFunction[indexOfNameOfFunction]<='Z') )
                //если индекс отрицателен,т.е. не буква в качестве символа,характеризующая начало имени функции, не найдена ,т.е. это уже конец строки, или эта (не буква) найдена, то начало имени функции найдено
                {
                    std::string nameOfFunction = textOfFunction.substr(indexOfNameOfFunction+1,indexOfEndOfNameOfFunction-indexOfNameOfFunction);
                    bool functionThatNeededIsFounded=false;
                    for(const std::string& availableFunction : availableFunctions)
                    //ищем имя функции среди доступных
                        if(availableFunction == nameOfFunction)
                        //если найдено запоминаем, что оно найдено и выходим
                        {
                            functionThatNeededIsFounded=true;
                            break;
                        }

                    if(! functionThatNeededIsFounded)
                    // не найдено
                        return false;

                    //найдено, прерываем цикл поиска начала имени, так как имя нашли и проверили
                    break;
                }
            }
        }


        //замена скобок или вызова функции на временную переменную
        textOfFunction.erase(nameOfFunctionIsFounded ? indexOfNameOfFunction+1/*+1 так как искали не букву*/ : indexOfBeginBound,
                              nameOfFunctionIsFounded ? indexOfEndBound-indexOfNameOfFunction-1               : indexOfEndBound-indexOfBeginBound);
        std::string nameOfTempVariable = "tb"+std::to_string(counterOfTempVariables++)+"te";
        textOfFunction.insert(nameOfFunctionIsFounded ? indexOfNameOfFunction+1/*+1 так как искали не букву*/ : indexOfBeginBound , nameOfTempVariable.data());
        //так как поиск следующей открывающейся скобки будет происходить с индекса скобок, которые были удалены, меняем это индекс на следующий за именем временной переменной
        indexOfEndBound = nameOfFunctionIsFounded ? indexOfNameOfFunction+1/*+1 так как искали не букву*/+nameOfTempVariable.size() : indexOfBeginBound+nameOfTempVariable.size();


    }
    if(textOfFunction.find(')',indexOfEndBound)!=-1)
    //проверка лишних закрывающих скобок
        return false;

    for(std::vector<BinaryOperation*>&  bOperationsSomePriority : binaryOperations)
    {
        std::list<std::string> listOfSymbolsOfOperators;
        for(BinaryOperation* operationOfThisPriority : bOperationsSomePriority)
            listOfSymbolsOfOperators.push_back(std::string(1,operationOfThisPriority->symbolOfOperation()));
        if( ! operatorsIsOk(textOfFunction,listOfSymbolsOfOperators,nameOfVariable))
            return false;
    }

    if( ! operandIsOk(textOfFunction,nameOfVariable))
        return false;

    //std::cout << textOfFunction. << std::endl;
    return true;
}

bool Function::operandIsOk(std::string textOfOperand, std::string nameOfVariable)
{
    textOfOperand = deleteAmountSpacers(textOfOperand);
    if(textOfOperand.find(' ')!=-1)
    //если внутри операнда найден пробел
        return false;

    /*возможные символы в операнде:'.',буквы,цифры*/
    if(textOfOperand[0]>='0' && textOfOperand[0]<='9')
    //операнд-константа
    {
        for(int i=0,numberOfPoints=0; i<textOfOperand.size(); ++i)
        {
            char c = textOfOperand[i];
            if(c=='.')
            {
                if(++numberOfPoints>1)
                    return false;
                else if(i==0 || i==textOfOperand.size()-1)
                //если точка на краях операнда
                    return false;
            }
            else if(!(c>='0'&& c<='9'))
            //проверка на символ не числа
                return false;
        }
    }
    else
    //операнд-идентификатор
    {
        if(textOfOperand.find('.')!=-1)
        //если внутри операнда-идентификатора найдена точка
            return false;
        if(!(textOfOperand==nameOfVariable || QString(textOfOperand.data()).contains(QRegExp("^tb([0-9]){1,}te$"))/*std::regex_match(textOfOperand,std::regex("^tb([0-9]){1,}te$"))*/))
                                                                                                       //изменить когда регексп стандарта будет имплементирован
            return false;
    }
    return true;
}

bool Function::operatorsIsOk(std::string &textOfFunction, std::list<std::string> operators, std::string nameOfVariable)
{
    for(int indexOfPowerOperator=0; (indexOfPowerOperator=lessedIndexOfFromList(textOfFunction,operators,indexOfPowerOperator))!=-1;)
    {
        if(indexOfPowerOperator==0 || indexOfPowerOperator==textOfFunction.size()-1)
        //оператор стоит в самом конце или начале,т.е. априори нет одного из операндов
            return false;

        int indexOfLeftOperandFromPower=indexOfPowerOperator-1;//индекс первого символа левого операнда
        for(; indexOfLeftOperandFromPower>0 ;indexOfLeftOperandFromPower--)
        //поиск начала левого операнда
        {
            char c = textOfFunction[indexOfLeftOperandFromPower];
            if(c=='-'||c=='+'||c=='*'||c=='/'||c=='%'||c=='^')
            {
                indexOfLeftOperandFromPower++;
                break;
            }
        }
        int indexOfRightOperandFromPower=indexOfPowerOperator+1; //индекс символа следующего за последним символом правого операнда
        for(; indexOfRightOperandFromPower<textOfFunction.size() ;indexOfRightOperandFromPower++)
        //поиск конца правого операнда
        {
            char c = textOfFunction[indexOfRightOperandFromPower];
            if(c=='-'||c=='+'||c=='*'||c=='/'||c=='%'||c=='^')
            {
                break;
            }
        }
        std::string leftOperand  = textOfFunction.substr(indexOfLeftOperandFromPower, indexOfPowerOperator-indexOfLeftOperandFromPower);
        std::string rightOperand = textOfFunction.substr(indexOfPowerOperator+1,indexOfRightOperandFromPower-(indexOfPowerOperator+1));
        if( ! operandIsOk(std::string(leftOperand.data()), nameOfVariable) || ! operandIsOk(std::string(rightOperand.data()),nameOfVariable))
            return false;

        //замена возведения в степень на временную переменную
        textOfFunction.erase(indexOfLeftOperandFromPower,indexOfRightOperandFromPower-indexOfLeftOperandFromPower);
        std::string nameOfTempVariable = "tb"+std::to_string(counterOfTempVariables++)+"te";
        textOfFunction.insert(indexOfLeftOperandFromPower,nameOfTempVariable.data());
        //так как поиск следующей степни будет происходить с индекса текущей степни, которая была удалена, меняем это индекс на следующий за именем временной переменной
        indexOfPowerOperator = indexOfLeftOperandFromPower + nameOfTempVariable.size();

    }
    return true;
}

std::string Function::deleteAmountSpacers(std::string s)
{
    int numberOfLeftSpacers=0;
    for(int i=0; i<s.size(); i++)
    {
        if(s[i]==' ')
            numberOfLeftSpacers++;
        else
            break;
    }
    s.erase(0,numberOfLeftSpacers);

    int numberOfRightSpacers=0;
    for(int i=s.size()-1; i>=0; i--)
    {
        if(s[i]==' ')
            numberOfRightSpacers++;
        else
            break;
    }
    s.erase(s.size()-numberOfRightSpacers,numberOfRightSpacers);
    return s;
}
std::string deleteWhiteSpaces(std::string s)
{
    for(std::string::iterator i=s.begin(); i!=s.end(); ++i)
    {
        if(*i==' ' || *i=='\t' || *i=='\n')
            s.erase(i);
    }
    return s;
}
IndexWithWhatsIndexed Function::lessedIndexOfFromList(std::string &text, std::list<std::string> stringsToSearch, int pos)
{
    int index=-1;
    int whatsIndexed=0;
    int numberOfiteration=-1;
    for(const std::string & stringToSearch : stringsToSearch)
    {
        ++numberOfiteration;
        int reachedIndex = text.find(stringToSearch,pos);
        if(index==-1 || (reachedIndex!= -1 && reachedIndex < index))
        {
            index = reachedIndex;
            whatsIndexed = numberOfiteration;
        }
    }
    return IndexWithWhatsIndexed(index,whatsIndexed);
}

bool Function::textOfFunctionIsOkWithNullingCounterOfTempVariables(std::string textOfFunction, std::string nameOfVariable)
{
    counterOfTempVariables = 0;
    return textOfFunctionIsOk(textOfFunction, nameOfVariable);
}

void Function::addBinaryOperation(BinaryOperation *operation, size_t priority)
{
    //создаем массив заданного приоритета, если его нет
     while(binaryOperations.size()/*количество приоритетов*/-1/*так как есть 0-ой приоритет*/<priority)
        binaryOperations.push_back(std::vector<BinaryOperation*>());

    binaryOperations[priority].push_back(operation);
}

void Function::addNestedFunction(const std::string &nameOfFunction, double (*pointerToFunction)(double))
{
    availableFunctions.push_back(NestedFunction(nameOfFunction,pointerToFunction));
}
