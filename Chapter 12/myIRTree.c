#include "myIRTree.h"
#include "makeMemory.h"
#include "myFrame.h"
#include "lValueTreater.h"

#include <assert.h>
#include <stdlib.h>

#define MAKE_ONE_STATEMENT()    IR_myStatement one =                                \
                                    makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);\
                                assert (one);      

#define MAKE_ONE_EXP()          IR_myExp one =                                      \
                                    makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);\
                                assert(one);


IR_myStatement IR_makeSeq(IR_myStatement left, IR_myStatement right)
{
    MAKE_ONE_STATEMENT();

    one->kind = IR_Seq;
    one->u.seq.left = left;
    one->u.seq.right = right;
    return one; 
}

IR_myStatement IR_makeLabel(myLabel label)
{
    MAKE_ONE_STATEMENT();

    one->kind = IR_Label;
    one->u.label = label;
    return one;
}

IR_myStatement IR_makeJump(IR_myExp exp, myLabelList labels)
{
    MAKE_ONE_STATEMENT();

    one->kind = IR_Jump;
    one->u.jump.exp = exp;
    one->u.jump.jumpLabels = labels;
    return one;
}

IR_myStatement IR_makeMove(IR_myExp dst, IR_myExp src)
{
    MAKE_ONE_STATEMENT();

    one->kind = IR_Move;
    one->u.move.dst = dst;
    one->u.move.src = src;
    return one;
}

IR_myStatement IR_makeExp(IR_myExp exp)
{
    MAKE_ONE_STATEMENT();

    one->kind = IR_Exp;
    one->u.exp = exp;
    return one;
}

IR_myStatement IR_makeCJump(
    IR_RelOperator op,
    IR_myExp leftExp, IR_myExp rightExp,
    myLabel trueLabel, myLabel falseLabel)
{
    MAKE_ONE_STATEMENT();

    one->kind = IR_CJump;
    one->u.cjump.op = op;
    one->u.cjump.left = leftExp;
    one->u.cjump.right = rightExp;
    one->u.cjump.trueLabel = trueLabel;
    one->u.cjump.falseLabel = falseLabel;
    return one;
}



IR_myExp IR_makeBinOperation(IR_BinOperator op, IR_myExp left, IR_myExp right)
{
    assert (left->kind == IR_Temp);

    MAKE_ONE_EXP();

    one->kind = IR_BinOperation;
    one->u.binOperation.op = op;
    one->u.binOperation.left = left;
    one->u.binOperation.right = right;
    return one;
}

IR_myExp IR_makeMem(IR_myExp mem)
{
    MAKE_ONE_EXP();

    one->kind = IR_Mem;
    one->u.mem = mem;
    return one;
}

IR_myExp IR_makeTemp(myTemp temp)
{
    MAKE_ONE_EXP();

    one->kind = IR_Temp;
    one->u.temp = temp;
    return one;
}

IR_myExp IR_makeESeq(IR_myStatement statement, IR_myExp exp)
{
    MAKE_ONE_EXP();

    one->kind = IR_ESeq;
    one->u.eseq.statement = statement;
    one->u.eseq.exp = exp;
    return one;
}

IR_myExp IR_makeName(myLabel label)
{
    MAKE_ONE_EXP();

    one->kind = IR_Name;
    one->u.name = label;
    return one;
}

IR_myExp IR_makeConst(int constValue)
{
    MAKE_ONE_EXP();

    one->kind = IR_Const;
    one->u.constValue = constValue;
    return one;
}

IR_myExp IR_makeCall(IR_myExp func, IR_myExpList args)
{
    assert (func->kind == IR_Name);

    MAKE_ONE_EXP();

    one->kind = IR_Call;
    one->u.call.func = func;
    one->u.call.args = args;
    return one;
}



IR_myStatementList IR_makeStatementList(
    IR_myStatement head, IR_myStatementList tails)
{
    IR_myStatementList one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
    assert (one);

    one->head = head;
    one->tails = tails;
    return one;
}

IR_myExpList IR_makeExpList(IR_myExp head, IR_myExpList tails)
{
    IR_myExpList one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
    assert (one);

    one->head = head;
    one->tails = tails;
    return one;
}


////////////////////////////////////////////////////////////////////

static void IR_divideExp_BinOperation(
    IR_myExp one, IR_myStatement* stateParts, IR_myExp* valueParts)
{
    assert (one->u.binOperation.left->kind == IR_Temp);

    IR_myStatement rightState;
    IR_myExp rightValue;
    IR_divideExp(one->u.binOperation.right, &rightState, &rightValue);

    IR_myStatement realBinOperation = IR_makeExp(IR_makeBinOperation(
        one->u.binOperation.op, one->u.binOperation.left, rightValue)); 
    *stateParts = IR_makeSeq(rightState, realBinOperation);

    if (one->u.binOperation.op == IR_Multiply ||
        one->u.binOperation.op == IR_Divide)
        //  the return value is in register eax, not the left operand
        *valueParts = IR_makeTemp(Frame_EAX());
    else
        *valueParts = one->u.binOperation.left;//  return value is always a temp
}

static void IR_divideExp_Call(
    IR_myExp one, IR_myStatement* stateParts, IR_myExp* valueParts)
{
    //  function args will be evaluated from right to left.
    IR_myExpList args = one->u.call.args; 
    IR_myStatement resultState = NULL;
    IR_myExpList resultValueList = NULL;

    //  todo: need test
    IR_myStatement* resultStateIter = &resultState;
    IR_myExpList* resultValueListIter = &resultValueList;
    while (args)
    {
        IR_myStatement argState;
        IR_myExp argValue;
        IR_divideExp(args->head, &argState, &argValue);

        //  first args, last execute
        *resultStateIter = IR_makeSeq(NULL, argState);
        resultStateIter = &((*resultStateIter)->u.seq.left);

        *resultValueListIter = IR_makeExpList(argValue, NULL);
        resultValueListIter = &((*resultValueListIter)->tails);

        args = args->tails;
    }

    //  move result to a new temp and recombine result
    IR_myExp newReg = IR_makeTemp(Temp_newTemp());
    *stateParts = IR_makeSeq(
        IR_makeSeq(
            resultState,
            IR_makeExp(IR_makeCall(one->u.call.func, resultValueList))),
        IR_makeMove(newReg, IR_makeTemp(Frame_RV())));
    *valueParts = newReg;
}

static void IR_divideExp_ESeq(
    IR_myExp one, IR_myStatement* stateParts, IR_myExp* valueParts)
{
    IR_myStatement firstState = one->u.eseq.statement;
    IR_myStatement secondState;
    IR_myExp secondValue;
    IR_divideExp(one->u.eseq.exp, &secondState, &secondValue);

    *stateParts = IR_makeSeq(firstState, secondState);
    *valueParts = secondValue;  //  ignore former values
}

static void IR_divideExp_Mem(
    IR_myExp one, IR_myStatement* stateParts, IR_myExp* valueParts)
{
    IR_myStatement state;
    IR_myExp value;   
    IR_divideExp(one->u.mem, &state, &value);

    //  only LValue generate Mem as a return value
    if (isLValueAsContent())
    {
        IR_myExp newReg = IR_makeTemp(Temp_newTemp());
        *stateParts = IR_makeSeq(state, IR_makeMove(newReg, IR_makeMem(value)));
        *valueParts = newReg;
    }
    else
    {
        *stateParts = state;
        *valueParts = IR_makeMem(value);
    }
}

static void IR_divideExp_Const(
    IR_myExp one, IR_myStatement* stateParts, IR_myExp* valueParts)
{
    IR_myExp newRegRep = IR_makeTemp(Temp_newTemp());

    *stateParts = IR_makeMove(newRegRep, IR_makeConst(one->u.constValue));
    *valueParts = newRegRep;
}

void IR_divideExp(IR_myExp one, IR_myStatement* stateParts, IR_myExp* valueParts)
{
    if (one == NULL)
    {
        *stateParts = NULL;
        *valueParts = NULL;
        return;
    }

    switch (one->kind)
    {
        case IR_BinOperation:
            IR_divideExp_BinOperation(one, stateParts, valueParts);
            break;
        case IR_Call:
            IR_divideExp_Call(one, stateParts, valueParts);
            break;
        case IR_ESeq:
            IR_divideExp_ESeq(one, stateParts, valueParts);
            break;
        case IR_Mem:
            IR_divideExp_Mem(one, stateParts, valueParts);
            break;
        case IR_Const:
            IR_divideExp_Const(one, stateParts, valueParts);
            break;
        case IR_Name:   //  fall through
        case IR_Temp:   //  fall through
            *stateParts = NULL, *valueParts = one;  break;
        default:       assert (false);
    }
}

//////////////////////////////////////////////////////////////////////////

IR_RelOperator revertRelOperator(IR_RelOperator op)
{
    switch (op)
    {
        case IR_Equal:          return IR_NotEqual;
        case IR_NotEqual:       return IR_Equal;
	
        case IR_LessThan:       return IR_GreaterEqual;
        case IR_LessEqual:      return IR_GreaterThan;
        case IR_GreaterThan:    return IR_LessEqual;
        case IR_GreaterEqual:   return IR_LessThan;
	
        case IR_ULessThan:      return IR_UGreaterEqual;
        case IR_ULessEqual:     return IR_UGreaterThan;
        case IR_UGreaterThan:   return IR_ULessEqual;
        case IR_UGreaterEqual:  return IR_ULessThan;
        default:        assert (false);
    }
}