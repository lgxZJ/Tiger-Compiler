#include "myError.h"
#include "makeMemory.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

///////////////////////////////////////////////////////////////////

//  these string must be matched with error codes.
static char* g_errorStrings[] = {
        "no error!",
        "subscript not int!",
        "record field not found!",
        "subscript error!",

        "declarations of Let illegal!",
        "body of Let illegal!",

        "variable of Simple-Var not declared!",
        "variable of Record-Field not declared!",
        "variable of Record-Field not record type!",
        "variable of Array-Subscript not declared!",
        "variable of Array-Subscript not array type!",
        "subscript of Array-Subscript not int!",

        "function of Function-Call not defined!",

        "type of Array-Creation not array!",
        "subscript of Array-Creation not type int!",
        "initial value of Array-Creation is not element type!",

        "type of Record-Creation not defined as record!",
        "type of Record-Creation not defined!",
        "names or types of fileds of Record-Creation not match!",

        "expression type of Arithmetic not int!",

        "expression of For not type int!",
        "expression of For not type no-return!",

        "condition type of If-Then-Else not int!",
        "clause of If-Then-Else not legal!",
        "clause types of If-Then-else not match!",

        "condition type of If-Then not int!",
        "clause type of If-Then not no-return!",

        "operand of Comparison not legal!",
        "operands type of Comparison not match!",

        "operand of Boolean-Operate not int!",

        "operand of Assignment not legal!",
        "operands type of Assignment not match!",

        "type of TypeDec-Named not defined!",
        "type of TypeDec-Record not defined!",
        "type of TypeDec-Array not defined!",

        "type of VarDec-Short can not be nil or no-return!",
        "type of VarDec-Long not defined!",
        "value expression of VarDec-Long illegal!",
        "operand types of VarDec-Long not match!",

        "parameter types of FuncDec-Procedure not defined!",
        "body of FuncDec-Procedure illgeal!",
        "body type of FuncDec-Procedure not no-return!",

        "parameter types of FuncDec-Function not defined!",
        "return type of FuncDec-Function not defined!",
        "body of FuncDec-Function illegal!",
        "return type and body type of FuncDec-Function not match!",

        "condition type of While not int!",
        "body type of While not no-return!",

        "expression type of Negative not int!",

        "parameter types of Function-Call not match!",

        "illegal recursive types!"
};


///////////////////////////////////////////////////////////////////

typedef struct{
    unsigned    errorCode;
    char*       msg;
} StackCell;

static StackCell*   g_errorStack = NULL;
static int          g_stackSize = 0;
static int          g_currentPos = -1;
static unsigned     g_stackIncrementor = 30;

//////////////////////////////////////////////////////////////////


void MyError_pushErrorCodeWithMsg(unsigned errorCode, char* msg)
{
    assert(errorCode >= 0); //  defiend error code

    //  no enough memory, re-allocate
    if (g_errorStack == NULL || g_currentPos == g_stackSize - 1)
    {
        StackCell* newStack = makeMemoryBlock(
            sizeof(StackCell) * (g_stackSize + g_stackIncrementor),
            MEMORY_TYPE_NONE);
        assert(newStack);

        memcpy(newStack, g_errorStack, sizeof(StackCell) * g_stackSize);
        g_errorStack = newStack;
        g_stackSize += g_stackIncrementor;
        //  memory is all made by makeMemoryBlock(), no need to free here.
    }

    ++g_currentPos;
    g_errorStack[g_currentPos].errorCode = errorCode;
    g_errorStack[g_currentPos].msg = msg;
}

void MyError_pushErrorCode(unsigned errorCode)
{ 
    MyError_pushErrorCodeWithMsg(errorCode, NULL);
}

////////////////////////////////////

unsigned MyError_getErrorCode(unsigned index)
{
    assert(index >=0 && index <= g_currentPos);

    return g_errorStack[index].errorCode;
}

char* MyError_getErrorMsg(unsigned index)
{
    assert(index >=0 && index <= g_currentPos);

    return g_errorStack[index].msg;
}

////////////////////////////////

unsigned MyError_getErrorCount(void)
{
    return g_currentPos + 1;
}


unsigned MyError_getErrorCapacity(void)
{
    return g_stackSize;
}

bool MyError_isErrorSet(void)
{
    return MyError_getErrorCount() != 0;
}


void MyError_clearErrors(void)
{
    g_currentPos = -1;
}


void MyError_printErrors()
{
    for (unsigned i = 0; i < MyError_getErrorCount(); ++i)
    {
        char* errorString = g_errorStrings[MyError_getErrorCode(i)];

        if (MyError_getErrorMsg(i) == NULL)
            fprintf(stderr, "%s\n", errorString);
        else
            fprintf(stderr, "%s : %s\n", errorString, MyError_getErrorMsg(i));
    }

    MyError_clearErrors();
}

