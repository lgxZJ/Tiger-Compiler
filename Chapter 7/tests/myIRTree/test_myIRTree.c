#include "../testHelper.h"

#include "../../myIRTree.h"
#include "../../lValueTreater.h"
#include "../../makeMemory.h"

#include <stdlib.h>

///////////////////////     test helpers    ///////////////////////



///////////////////////        tests       ////////////////////////

void test_IRMakeSeq_ByDefault_GetSeqWithMemberOfPassed(void)
{
    IR_myStatement left = (IR_myStatement)12;
    IR_myStatement right = (IR_myStatement)45;

    IR_myStatement oneMade = IR_makeSeq(left, right);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Seq);
    CU_ASSERT_EQUAL(oneMade->u.seq.left, left);
    CU_ASSERT_EQUAL(oneMade->u.seq.right, right);
}

//////////////////////

void test_IRMakeLabel_ByDefault_GetLabelWithMemberOfPassed(void)
{
    myLabel label = (myLabel)23;
    
    IR_myStatement oneMade = IR_makeLabel(label);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Label);
    CU_ASSERT_EQUAL(oneMade->u.label, label);
}

///////////////////////

void test_IRMakeJump_ByDefault_GetJumpWithMemberOfPassed(void)
{
    IR_myExp exp = (IR_myExp)334;
    myLabelList labels = Temp_makeLabelList((myLabel)23, NULL);

    IR_myStatement oneMade = IR_makeJump(exp, labels);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Jump);
    CU_ASSERT_EQUAL(oneMade->u.jump.exp, exp);
    CU_ASSERT_EQUAL(oneMade->u.jump.jumpLabels, labels); 
}

///////////////////////

void test_IRMakeCJump_ByDefault_GetCJumpWithMemberOfPassed(void)
{
    IR_myExp leftExp = (IR_myExp)12;
    IR_myExp rightExp = (IR_myExp)23;
    myLabel trueLabel = (myLabel)45;
    myLabel falseLabel = (myLabel)78;

    IR_myStatement oneMade =
        IR_makeCJump(1, leftExp, rightExp, trueLabel, falseLabel);

    CU_ASSERT_EQUAL(oneMade->kind, IR_CJump);
    CU_ASSERT_EQUAL(oneMade->u.cjump.left, leftExp);
    CU_ASSERT_EQUAL(oneMade->u.cjump.right, rightExp);
    CU_ASSERT_EQUAL(oneMade->u.cjump.trueLabel, trueLabel);
    CU_ASSERT_EQUAL(oneMade->u.cjump.falseLabel, falseLabel);
}

///////////////////////

void test_IRMakeMove_ByDefault_GetMoveWithMemberOfPassed(void)
{
    IR_myExp dst = (IR_myExp)12;
    IR_myExp src = (IR_myExp)34;

    IR_myStatement oneMade = IR_makeMove(dst, src);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Move);
    CU_ASSERT_EQUAL(oneMade->u.move.dst, dst);
    CU_ASSERT_EQUAL(oneMade->u.move.src, src);
}

///////////////////////

void test_IRMakeExp_ByDefault_GetExpWithMemberOfPassed(void)
{
    IR_myExp exp = (IR_myExp)34;

    IR_myStatement oneMade = IR_makeExp(exp);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Exp);
    CU_ASSERT_EQUAL(oneMade->u.exp, exp);
}

///////////////////////

void test_IRMakeBinOperation_ByDefault_GetBinOperationWithMemberOfPassed(void)
{
    IR_myExp left = IR_makeTemp((myTemp)12);
    IR_myExp right = (IR_myExp)34;

    IR_myExp oneMade = IR_makeBinOperation(1, left, right);

    CU_ASSERT_EQUAL(oneMade->kind, IR_BinOperation);
    CU_ASSERT_EQUAL(oneMade->u.binOperation.left, left);
    CU_ASSERT_EQUAL(oneMade->u.binOperation.right, right);
}

//////////////////////

void test_IRMakeMem_ByDefault_GetMemWithMemberOfPassed(void)
{
    IR_myExp fakeMem = (IR_myExp)23;

    IR_myExp oneMade = IR_makeMem(fakeMem);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Mem);
    CU_ASSERT_EQUAL(oneMade->u.mem, fakeMem);
}

///////////////////////

void test_IRMakeTemp_ByDefault_GetTempWithMemberOfPassed(void)
{
    myTemp fakeTemp = (myTemp)12;

    IR_myExp oneMade = IR_makeTemp(fakeTemp);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Temp);
    CU_ASSERT_EQUAL(oneMade->u.temp, fakeTemp);
}

//////////////////////

void test_IRMakeESeq_ByDefault_GetESeqWithMemberOfPassed(void)
{
    IR_myStatement fakeStatement = (IR_myStatement)12;
    IR_myExp fakeExp = (IR_myExp)34;

    IR_myExp oneMade = IR_makeESeq(fakeStatement, fakeExp);

    CU_ASSERT_EQUAL(oneMade->kind, IR_ESeq);
    CU_ASSERT_EQUAL(oneMade->u.eseq.exp, fakeExp);
    CU_ASSERT_EQUAL(oneMade->u.eseq.statement, fakeStatement); 
}

///////////////////////

void test_IRMakeName_ByDefault_GetNameWithMemberOfPassed(void)
{
    myLabel fakeLabel = (myLabel)12;

    IR_myExp oneMade = IR_makeName(fakeLabel);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Name);
    CU_ASSERT_EQUAL(oneMade->u.name, fakeLabel);
}

///////////////////////

void test_IRMakeConst_ByDefault_GetConstWithMemberOfPassed(void)
{
    int constValue = 12;

    IR_myExp oneMade = IR_makeConst(constValue);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Const);
    CU_ASSERT_EQUAL(oneMade->u.constValue, constValue);
}

///////////////////////

void test_IRMakeCall_ByDefault_GetCallWithMemberOfPassed(void)
{
    IR_myExp fakeFunc = IR_makeName((myLabel)12);
    IR_myExpList fakeArgs = NULL;

    IR_myExp oneMade = IR_makeCall(fakeFunc, fakeArgs);

    CU_ASSERT_EQUAL(oneMade->kind, IR_Call);
    CU_ASSERT_EQUAL(oneMade->u.call.func, fakeFunc);
    CU_ASSERT_EQUAL(oneMade->u.call.args, fakeArgs); 
}

///////////////////////

void test_IRMakeStatementList_ByDefault_GetListWithMemberOfPassed(void)
{
    IR_myStatement fakeHead = (IR_myStatement)12;
    IR_myStatementList fakeTails = (IR_myStatementList)34;

    IR_myStatementList oneMade = IR_makeStatementList(fakeHead, fakeTails);

    CU_ASSERT_EQUAL(oneMade->head, fakeHead);
    CU_ASSERT_EQUAL(oneMade->tails, fakeTails);
}

///////////////////////

void test_IRMakeExpList_ByDefault_GetListWithMemberOfPassed(void)
{
    IR_myExp fakeHead = (IR_myExp)12;
    IR_myExpList fakeTails = (IR_myExpList)45;

    IR_myExpList oneMade = IR_makeExpList(fakeHead, fakeTails);

    CU_ASSERT_EQUAL(oneMade->head, fakeHead);
    CU_ASSERT_EQUAL(oneMade->tails, fakeTails);
}

////////////////////////

//  a parameterized test
void testOneIRExp_ReturnNullStateSameExp(IR_myExp exp)
{
    IR_myStatement state;
    IR_myExp value;
    IR_divideExp(exp, &state, &value);

    CU_ASSERT_EQUAL(state, NULL);
    CU_ASSERT_EQUAL(value, exp);
}

////////

void test_IRDivideExp_TempExp_ReturnNullStateAndSameExp(void)
{
    IR_myExp exp = IR_makeTemp(Temp_newTemp());

    testOneIRExp_ReturnNullStateSameExp(exp);
}

void test_IRDivideExp_NameExp_ReturnNullStateAndSameExp(void)
{
    IR_myExp exp = IR_makeName((myLabel)12);

    testOneIRExp_ReturnNullStateSameExp(exp);
}

void test_IRDivideExp_ConstExp_ReturnNullStateAndSameExp(void)
{
    IR_myExp exp = IR_makeConst(1);

    testOneIRExp_ReturnNullStateSameExp(exp);
}

void test_IRDivideExp_TreatLValueAsAddress_DoNotConvertToRegister(void)
{
    treatLValueAsAddress();
    myTemp addrTemp = Temp_newTemp();
    IR_myExp exp = IR_makeMem(IR_makeTemp(addrTemp));

    IR_myStatement state;
    IR_myExp value;
    IR_divideExp(exp, &state, &value);

    CU_ASSERT_EQUAL(value->kind, IR_Mem);
    CU_ASSERT_EQUAL(value->u.mem->kind, IR_Temp)
    CU_ASSERT_EQUAL(value->u.mem->u.temp, addrTemp);
}

void test_IRDivideExp_TreatLValueAsContent_DoConvertToRegister(void)
{
    treatLValueAsContent();
    myTemp addrTemp = Temp_newTemp();
    IR_myExp exp = IR_makeMem(IR_makeTemp(addrTemp));

    IR_myStatement state;
    IR_myExp value;
    IR_divideExp(exp, &state, &value);

    CU_ASSERT_EQUAL(state->kind, IR_Seq);
    CU_ASSERT_EQUAL(state->u.seq.right->kind, IR_Move);

    CU_ASSERT_EQUAL(value->kind, IR_Temp);
}

void test_IRDivideExp_ESeqExp_ReturnInnerExpAsItsExp(void)
{
    treatLValueAsAddress();
    IR_myExp exp = IR_makeESeq(
        IR_makeLabel((myLabel)12), IR_makeMem(IR_makeConst(0)));

    IR_myStatement state;
    IR_myExp value;
    IR_divideExp(exp, &state, &value);

    CU_ASSERT_EQUAL(value->kind, IR_Mem);
    CU_ASSERT_EQUAL(value->u.mem->kind, IR_Const)
    CU_ASSERT_EQUAL(value->u.mem->u.constValue, 0);
}

void test_IRDivideExp_BinOperationExp_DivideRightAndRecombineWithLeft(void)
{
    IR_myExp exp = IR_makeBinOperation(
        IR_Plus,
        IR_makeTemp((myTemp)12),
        IR_makeBinOperation(IR_Plus, IR_makeTemp((myTemp)12), IR_makeConst(0)));

    IR_myStatement state;
    IR_myExp value;
    IR_divideExp(exp, &state, &value);

    CU_ASSERT_EQUAL(state->kind, IR_Seq);
    CU_ASSERT_EQUAL(state->u.seq.left->kind, IR_BinOperation);
    CU_ASSERT_EQUAL(state->u.seq.right->kind, IR_Exp);
    CU_ASSERT_EQUAL(state->u.seq.right->u.exp->kind, IR_BinOperation);
    CU_ASSERT_EQUAL(value->kind, IR_Temp);
}

void test_IRDivideExp_CallExp_ExtractArgs(void)
{
    IR_myExp exp = IR_makeCall(
        IR_makeName((myLabel)12),
        IR_makeExpList(IR_makeESeq(IR_makeMove(NULL, NULL), IR_makeConst(1)),
            IR_makeExpList(IR_makeESeq(IR_makeLabel((myLabel)NULL), IR_makeConst(2)), NULL)));
    
    IR_myStatement state;
    IR_myExp value;
    IR_divideExp(exp, &state, &value);

    CU_ASSERT_EQUAL(state->kind, IR_Seq);
    CU_ASSERT_EQUAL(state->u.seq.right->kind, IR_Move);
    CU_ASSERT_EQUAL(state->u.seq.right->u.move.src->kind, IR_Temp);
    CU_ASSERT_EQUAL(value->kind, IR_Temp);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_IRMakeSeq_ByDefault_GetSeqWithMemberOfPassed },
        { "", test_IRMakeLabel_ByDefault_GetLabelWithMemberOfPassed },
        { "", test_IRMakeJump_ByDefault_GetJumpWithMemberOfPassed },
        { "", test_IRMakeCJump_ByDefault_GetCJumpWithMemberOfPassed },
        { "", test_IRMakeMove_ByDefault_GetMoveWithMemberOfPassed },
        { "", test_IRMakeExp_ByDefault_GetExpWithMemberOfPassed },

        { "", test_IRMakeBinOperation_ByDefault_GetBinOperationWithMemberOfPassed },
        { "", test_IRMakeMem_ByDefault_GetMemWithMemberOfPassed },
        { "", test_IRMakeTemp_ByDefault_GetTempWithMemberOfPassed },
        { "", test_IRMakeESeq_ByDefault_GetESeqWithMemberOfPassed },
        { "", test_IRMakeName_ByDefault_GetNameWithMemberOfPassed },
        { "", test_IRMakeConst_ByDefault_GetConstWithMemberOfPassed },
        { "", test_IRMakeCall_ByDefault_GetCallWithMemberOfPassed },

        { "", test_IRMakeStatementList_ByDefault_GetListWithMemberOfPassed },
        { "", test_IRMakeExpList_ByDefault_GetListWithMemberOfPassed },

        { "", test_IRDivideExp_TempExp_ReturnNullStateAndSameExp },
        { "", test_IRDivideExp_NameExp_ReturnNullStateAndSameExp },
        { "", test_IRDivideExp_ConstExp_ReturnNullStateAndSameExp },
        { "", test_IRDivideExp_TreatLValueAsAddress_DoNotConvertToRegister },
        { "", test_IRDivideExp_TreatLValueAsContent_DoConvertToRegister },
        { "", test_IRDivideExp_ESeqExp_ReturnInnerExpAsItsExp },
        { "", test_IRDivideExp_BinOperationExp_DivideRightAndRecombineWithLeft },
        { "", test_IRDivideExp_CallExp_ExtractArgs }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}