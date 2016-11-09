#include "myIRTree.h"
#include "makeMemory.h"

#include <assert.h>

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