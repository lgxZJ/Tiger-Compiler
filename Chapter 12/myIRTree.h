#ifndef MY_IRTREE_H
#define MY_IRTREE_H

#include "myTemp.h"

/////////////////////////////////////////////////////////////////////////////
//                            type definitions

// 
//      Binary operators, which includes arithmetic operators and
//  bitwise operators.
//
typedef enum
{
    IR_Plus, IR_Minus, IR_Multiply, IR_Divide, IR_And,
    IR_Or, IR_LShift, IR_RShift, IR_ARShift, IR_Xor 
}       IR_BinOperator;


//
//      Relational operators, which include equality and nonequality
//  operators and inequality operators.
//
typedef enum
{
    IR_Equal, IR_NotEqual, IR_LessThan, IR_GreaterThan,
    IR_LessEqual, IR_GreaterEqual, IR_ULessThan, IR_ULessEqual,
    IR_UGreaterThan, IR_UGreaterEqual 
}       IR_RelOperator;


//  forwards
typedef struct IR_myExp_* IR_myExp;

//
//  ADT for basic statements
//
typedef struct IR_myStatement_
{
    enum 
    {
        IR_Seq, IR_Label, IR_Jump, IR_CJump, IR_Move, IR_Exp
    }       kind;

    union
    {
        struct { struct IR_myStatement_* left, *right; } seq;
        myLabel label;
        struct { IR_myExp exp; myLabelList jumpLabels; } jump;
        struct { IR_RelOperator op; IR_myExp left, right; 
                 myLabel trueLabel, falseLabel; } cjump;
        struct { IR_myExp dst, src; } move;
        IR_myExp exp;
    }       u;
}*  IR_myStatement;

// 
//  ADT for expressions
//
typedef struct IR_myExp_
{
    enum 
    { 
        IR_BinOperation, IR_Mem, IR_Temp, IR_ESeq,
        IR_Name, IR_Const, IR_Call
    } kind;

    union 
    {
        struct { IR_BinOperator op; struct IR_myExp_* left, *right; } binOperation;
        struct IR_myExp_* mem;
        myTemp temp;
        struct { IR_myStatement statement; struct IR_myExp_* exp; } eseq;
        myLabel name;
        int constValue;
        struct { myString funcName; struct IR_myExpList_* args; } call;
    } u;
}*  IR_myExp;


typedef struct IR_myStatementList_
{
    IR_myStatement              head;
    struct IR_myStatementList_* tails;
}*  IR_myStatementList;

typedef struct IR_myExpList_
{
    IR_myExp                head;
    struct IR_myExpList_*   tails;
}*  IR_myExpList;

/////////////////////////////////////////////////////////////////////////////////
//                              make functions

//
//  NOTE:   every IR_myExp must begin with a temporary or is a const.
//  `exp` field of ESeq can be ignored.

IR_myStatement IR_makeSeq(IR_myStatement left, IR_myStatement right);
IR_myStatement IR_makeLabel(myLabel label);
IR_myStatement IR_makeJump(IR_myExp exp, myLabelList labels);
IR_myStatement IR_makeMove(IR_myExp dst, IR_myExp src);
IR_myStatement IR_makeExp(IR_myExp exp);
IR_myStatement IR_makeCJump(
    IR_RelOperator op, IR_myExp leftExp, IR_myExp rightExp,
    myLabel trueLabel, myLabel falseLabel);

//  every binary operation must begin with a temp expression. And left register
//  is the result.
IR_myExp IR_makeBinOperation(IR_BinOperator op, IR_myExp left, IR_myExp right);
IR_myExp IR_makeMem(IR_myExp addr);
IR_myExp IR_makeTemp(myTemp temp);
IR_myExp IR_makeESeq(IR_myStatement statement, IR_myExp exp);
IR_myExp IR_makeName(myLabel label);
IR_myExp IR_makeConst(int constValue);
IR_myExp IR_makeCall(myString funcName, IR_myExpList args);

IR_myStatementList IR_makeStatementList(IR_myStatement head, IR_myStatementList tail);
IR_myExpList IR_makeExpList(IR_myExp head, IR_myExpList tails);

//  DO:
//          divide an IR_myExp into two parts:  'statement' and 'value'.
//      'statement' contains all statements needed to finish the expression,
//      'value' contains the value representation which is simply expressed.
//  PARAMS:
//      stateParts  a IR_myStatement pointer point to a variable which will hold
//                  the 'statement'.
//      valueParts  a IR_myExp pointer point to a variable which will hold the 'value'.
//  RETURN:
//      This function has no return value.
// 
void IR_divideExp(IR_myExp one, IR_myStatement* stateParts, IR_myExp* valueParts);

IR_RelOperator revertRelOperator(IR_RelOperator op);

#endif