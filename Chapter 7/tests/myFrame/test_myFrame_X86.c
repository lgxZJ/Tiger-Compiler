#include "../testHelper.h"

#include "../../myFrame.h"
#include "../../makeMemory.h"

#include <stdlib.h>

///////////////////////     test helpers    ///////////////////////

int getFormalCountFromFrame(myFrame frame)
{
    CU_ASSERT((bool)frame);
    return Frame_getAccessListCount(Frame_getFormals(frame));
}

///////////////////////        tests       ////////////////////////

void test_FramemakeBoolList_ByDefault_MakeWhatPassed(void)
{
    myBoolList fakeFormmers = NULL;
    bool newOne = false;

    myBoolList formalsMade = Frame_makeBoolList(fakeFormmers, newOne);

    CU_ASSERT_EQUAL(formalsMade->head, newOne);
    CU_ASSERT_EQUAL(formalsMade->tail, fakeFormmers);
}

///////////////////////////

void test_FramemakeAccessList_ByDefault_MakeWhatPasses(void)
{
    myAccessList fakeFormers = NULL;
    myAccess fakeOne = (myAccess)NULL;

    myAccessList accessesMade = Frame_makeAccessList(fakeOne, fakeFormers);

    CU_ASSERT_EQUAL(accessesMade->head, fakeOne);
    CU_ASSERT_EQUAL(accessesMade->tail, fakeFormers);
}

/////////////////////////////

//  forwards
void test_FramenewFrame_FormalCountEqualsFlags(
    myBoolList flags, int countExpected);
void test_FramenewFrame_FormalLocalCountEqualsFlags(
    myBoolList flags, int countExpected);

////////

void test_FranenewFrame_FlagEscape_AccessInFrame(void)
{
    myBoolList oneTrueFlag = Frame_makeBoolList(NULL, true);

    myFrame frame = Frame_newFrame((myLabel)12, oneTrueFlag);

    myAccessList accesses = Frame_getFormals(frame);
    CU_ASSERT(Frame_isAccessInFrame(accesses->head));
}

void test_FranenewFrame_FlagNotEscape_AccessInReg(void)
{
    myBoolList oneFalseFlag = Frame_makeBoolList(NULL, false);

    myFrame frame = Frame_newFrame((myLabel)12, oneFalseFlag);

    myAccessList accesses = Frame_getFormals(frame);
    CU_ASSERT(Frame_isAccessInReg(accesses->head));
}

void test_FramenewFrame_AllEscapes_FormalAndLocalCountEquals(void)
{
    myBoolList flagsWithTwoTrue =
        Frame_makeBoolList(Frame_makeBoolList(NULL, true), true);
    int boolsCount = Frame_getBoolListCount(flagsWithTwoTrue);
    
    test_FramenewFrame_FormalCountEqualsFlags(flagsWithTwoTrue, boolsCount);
    test_FramenewFrame_FormalLocalCountEqualsFlags(flagsWithTwoTrue, boolsCount);
}

void test_FramenewFrame_NotAllEscapes_FormalAndLocalCountEquals(void)
{
    myBoolList flagsWithTwoTrue =
        Frame_makeBoolList(Frame_makeBoolList(Frame_makeBoolList(NULL, false),
            false), true);
    int boolsCount = Frame_getBoolListCount(flagsWithTwoTrue);
    
    test_FramenewFrame_FormalCountEqualsFlags(flagsWithTwoTrue, boolsCount);
    test_FramenewFrame_FormalLocalCountEqualsFlags(flagsWithTwoTrue, boolsCount - 2);
}

void test_FramenewFrame_AllEscapes_NegativeOffsetWithFourInterval(void)
{
    myBoolList formalFlags = Frame_makeBoolList(Frame_makeBoolList(NULL, true), true);

    myFrame frame = Frame_newFrame((myLabel)12, formalFlags);

    myAccessList formals = Frame_getFormals(frame);
    int firstArgOffset = Frame_getAccessOffset(formals->head);
    int secondArgOffset = Frame_getAccessOffset(formals->tail->head);
    CU_ASSERT_EQUAL(firstArgOffset, 0);
    CU_ASSERT_EQUAL(secondArgOffset, -4);
}

void test_FramenewFrame_AllEscapes_IncrementOnlyLocal(void)
{
    myBoolList formalFlags = Frame_makeBoolList(Frame_makeBoolList(NULL, true), true);

    myFrame frame = Frame_newFrame((myLabel)12, formalFlags);

    int localCount = Frame_getLocalCount(frame);
    int formalCount = Frame_getformalLocalCount(frame);
    CU_ASSERT_EQUAL(localCount, 0);
    CU_ASSERT_EQUAL(formalCount, 2);
}

//  a parameterized test
void test_FramenewFrame_FormalCountEqualsFlags(myBoolList flags, int countExpected)
{
    myLabel fakeLabel = 0;

    myFrame frame = Frame_newFrame(fakeLabel, flags);

    int formalsCount = getFormalCountFromFrame(frame);
    CU_ASSERT_EQUAL(formalsCount, countExpected);
}

void test_FramenewFrame_FormalLocalCountEqualsFlags(myBoolList flags, int countExpected)
{
    myLabel fakeLabel = 0;

    myFrame frame = Frame_newFrame(fakeLabel, flags);

    int formalsCount = Frame_getformalLocalCount(frame);
    CU_ASSERT_EQUAL(formalsCount, countExpected);
}

///////////////////////////////

void test_FrameIsFrameEqual_ByDefault_WorkRight(void)
{
    myFrame frameOne = Frame_newFrame((myLabel)NULL, (myBoolList)NULL);
    myFrame frameTwo = Frame_newFrame((myLabel)NULL, (myBoolList)NULL);

    CU_ASSERT(Frame_isFrameEqual(frameOne, frameOne));
    CU_ASSERT(!Frame_isFrameEqual(frameOne, frameTwo));
}

void test_FrameIsFrameSame_DifferentFrame_ReturnFalse(void)
{
    myFrame frameOne = Frame_newFrame((myLabel)NULL, (myBoolList)NULL);
    myFrame frameTwo = Frame_newFrame((myLabel)NULL, (myBoolList)NULL);
    myFrame frameThree = Frame_newFrame((myLabel)NULL, (myBoolList)NULL);

    CU_ASSERT_FALSE(Frame_isFrameSame(frameOne, frameTwo));

    Frame_allocateLocal(frameTwo, true);
    Frame_allocateLocal(frameThree, false);

    CU_ASSERT_FALSE(Frame_isFrameSame(frameOne, frameTwo));
    CU_ASSERT_FALSE(Frame_isFrameSame(frameOne, frameThree));
}

void test_FrameIsFrameSame_SameFrame_ReturnTrue(void)
{
    myFrame frameOne = Frame_newFrame((myLabel)NULL, (myBoolList)NULL);

    CU_ASSERT_TRUE(Frame_isFrameSame(frameOne, frameOne));
}

///////////////////////////////

void test_FrameallocateLocal_OneEscapeFlag_ReturnInFrame(void)
{
    myFrame frame = Frame_newFrame((myLabel)NULL, NULL);

    myAccess access = Frame_allocateLocal(frame, true);

    CU_ASSERT(Frame_isAccessInFrame(access));
}

void test_FrameallocateLocal_OneNotEscapeFlag_ReturnInReg(void)
{
    myFrame frame = Frame_newFrame((myLabel)NULL, NULL);

    myAccess access = Frame_allocateLocal(frame, false);

    CU_ASSERT(Frame_isAccessInReg(access));
}

//  forwards
void test_FrameallocateLocal_LocalCountIncremented_EqualsTo(
    bool flag, int numExpected);
void test_FrameallocateLocal_FormalCountIncremented_EqualsTo(
    bool flag, int numExpected);

///////////

void test_FrameallocateLocal_AllocateInFrame_LocalCountIncremented(void)
{
    test_FrameallocateLocal_LocalCountIncremented_EqualsTo(
        true, 1);
}

void test_FrameallocateLocal_AllocateInReg_LocalCountNotChange(void)
{
    test_FrameallocateLocal_LocalCountIncremented_EqualsTo(
        false, 0);
}

void test_FrameallocateLocal_AllocateInFrame_FormalCountNotChange(void)
{
    test_FrameallocateLocal_FormalCountIncremented_EqualsTo(
        true, 0);
}

void test_FrameallocateLocal_AllocateInReg_FormalCountNotChange(void)
{
    test_FrameallocateLocal_FormalCountIncremented_EqualsTo(
        false, 0);
}

//  two parameterized tests
void test_FrameallocateLocal_LocalCountIncremented_EqualsTo(
    bool flag, int numExpected)
{
    myFrame frame = Frame_newFrame((myLabel)NULL, NULL);
    int localVarCountBefore = Frame_getLocalCount(frame);

    Frame_allocateLocal(frame, flag);

    int localVarCountAfter = Frame_getLocalCount(frame);
    CU_ASSERT_EQUAL(localVarCountAfter - localVarCountBefore, numExpected);
}

void test_FrameallocateLocal_FormalCountIncremented_EqualsTo(
    bool flag, int numExpected)
{
    myFrame frame = Frame_newFrame((myLabel)NULL, NULL);
    int formalCountBefore = getFormalCountFromFrame(frame);

    Frame_allocateLocal(frame, flag);

    int formalCountAfter = getFormalCountFromFrame(frame);
    CU_ASSERT_EQUAL(formalCountAfter - formalCountBefore, numExpected);
}

//////////////////////////////

extern int Frame_getAccessOffset(myAccess access);

void test_FrameallocateLocal_TwoConsecutiveCallsInSameFrame_OffsetIncrementFour(void)
{
    myFrame frame = Frame_newFrame((myLabel)NULL, NULL);

    int accessOneOffset = Frame_getAccessOffset(Frame_allocateLocal(frame, true));
    int accessTwoOffset = Frame_getAccessOffset(Frame_allocateLocal(frame, true));

    CU_ASSERT_EQUAL(accessTwoOffset - accessOneOffset, -4);
}

void test_FrameallocateLocal_TwoCallsInDifferentNewFrame_OffsetSame(void)
{
    myFrame frameOne = Frame_newFrame((myLabel)NULL, NULL);
    myFrame frameTwo = Frame_newFrame((myLabel)NULL, NULL);

    int accessOneOffset = Frame_getAccessOffset(Frame_allocateLocal(frameOne, true));
    int accessTwoOffset = Frame_getAccessOffset(Frame_allocateLocal(frameTwo, true));

    CU_ASSERT_EQUAL(accessTwoOffset, accessOneOffset);
}

void test_FrameallocateLocal_ByDefault_IncrementLocal(void)
{
    myFrame frameOne = Frame_newFrame((myLabel)NULL, NULL);
    int localCountBefore = Frame_getLocalCount(frameOne);

    Frame_allocateLocal(frameOne, true);
    int localCountAfter = Frame_getLocalCount(frameOne);

    CU_ASSERT_EQUAL(localCountAfter - localCountBefore, 1);
}

///////////////////////

void test_FrameMakeStringFrag_ByDefault_MakeStringFragWithWhatPassed(void)
{
    myLabel fakeLabel = (myLabel)12;
    myString fakeString = (myString)"12";

    Frame_myFrag fragment = Frame_makeStringFrag(fakeLabel, fakeString);

    CU_ASSERT_EQUAL(fragment->kind, Frame_StringFrag);
    CU_ASSERT_EQUAL(fragment->u.strFrag.label, fakeLabel);
    CU_ASSERT_EQUAL(fragment->u.strFrag.str, fakeString);
}

void test_FrameMakeProcFrag_ByDefault_MakeProcFragWithWhatPassed(void)
{
    IR_myStatement fakeBodyState = (IR_myStatement)12;
    myFrame fakeFrame = (myFrame)3;

    Frame_myFrag fragement = Frame_makeProcFrag(
        fakeBodyState, fakeFrame, MySymbol_MakeSymbol("funcName"));

    CU_ASSERT_EQUAL(fragement->kind, Frame_ProcFrag);
    CU_ASSERT_EQUAL(fragement->u.procFrag.body, fakeBodyState);
    CU_ASSERT_EQUAL(fragement->u.procFrag.frame, fakeFrame);
}

void test_FrameMakeFragList_ByDefault_MakeFragListWithWhatPassed(void)
{
    Frame_myFrag fakeHead = (Frame_myFrag)12;
    Frame_myFragList fakeTail = (Frame_myFragList)23;

    Frame_myFragList list = Frame_makeFragList(fakeHead, fakeTail);

    CU_ASSERT_EQUAL(list->head, fakeHead);
    CU_ASSERT_EQUAL(list->tail, fakeTail);
}

/////////////

void test_FrameFP_ByDefault_AlwaysReturnTheSameOne(void)
{
    myTemp pfFirst = Frame_FP();
    myTemp pfSecond = Frame_FP();

    CU_ASSERT_EQUAL(pfFirst, pfSecond);
}

void test_FrameRV_ByDefault_AlwaysReturnTheSameOne(void)
{
    myTemp rvFirst = Frame_RV();
    myTemp rvSecond = Frame_RV();

    CU_ASSERT_EQUAL(rvFirst, rvSecond);
}


///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_FramemakeBoolList_ByDefault_MakeWhatPassed },
        { "", test_FramemakeAccessList_ByDefault_MakeWhatPasses },

        { "", test_FranenewFrame_FlagEscape_AccessInFrame },
        { "", test_FranenewFrame_FlagNotEscape_AccessInReg },
        { "", test_FramenewFrame_AllEscapes_FormalAndLocalCountEquals },
        { "", test_FramenewFrame_NotAllEscapes_FormalAndLocalCountEquals },
        { "", test_FramenewFrame_AllEscapes_NegativeOffsetWithFourInterval },
        { "", test_FramenewFrame_AllEscapes_IncrementOnlyLocal },

        { "", test_FrameIsFrameEqual_ByDefault_WorkRight },
        { "", test_FrameIsFrameSame_DifferentFrame_ReturnFalse },
        { "", test_FrameIsFrameSame_SameFrame_ReturnTrue },

        { "", test_FrameallocateLocal_OneEscapeFlag_ReturnInFrame },
        { "", test_FrameallocateLocal_OneNotEscapeFlag_ReturnInReg },
        { "", test_FrameallocateLocal_AllocateInFrame_LocalCountIncremented },
        { "", test_FrameallocateLocal_AllocateInReg_LocalCountNotChange },
        { "", test_FrameallocateLocal_AllocateInFrame_FormalCountNotChange },
        { "", test_FrameallocateLocal_AllocateInReg_FormalCountNotChange },
        { "", test_FrameallocateLocal_TwoConsecutiveCallsInSameFrame_OffsetIncrementFour },
        { "", test_FrameallocateLocal_TwoCallsInDifferentNewFrame_OffsetSame },
        { "", test_FrameallocateLocal_ByDefault_IncrementLocal },

        { "", test_FrameMakeStringFrag_ByDefault_MakeStringFragWithWhatPassed },
        { "", test_FrameMakeProcFrag_ByDefault_MakeProcFragWithWhatPassed },
        { "", test_FrameMakeFragList_ByDefault_MakeFragListWithWhatPassed },

        { "", test_FrameFP_ByDefault_AlwaysReturnTheSameOne },
        { "", test_FrameRV_ByDefault_AlwaysReturnTheSameOne },
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}