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

int getLocalCountFromFrame(myFrame frame)
{
    return Frame_getLocalCount(frame);
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

////////

void test_FramenewFrame_AllEscapes_FormalCountEquals(void)
{
    myBoolList flagsWithTwoTrue =
        Frame_makeBoolList(Frame_makeBoolList(NULL, true), true);
    int boolsCount = Frame_getBoolListCount(flagsWithTwoTrue);
    
    test_FramenewFrame_FormalCountEqualsFlags(flagsWithTwoTrue, boolsCount);
}

void test_FramenewFrame_NotAllEscapes_FormalCountEquals(void)
{
    myBoolList flagsWithTwoTrue =
        Frame_makeBoolList(Frame_makeBoolList(Frame_makeBoolList(NULL, false),
            false), true);
    int boolsCount = Frame_getBoolListCount(flagsWithTwoTrue);
    
    test_FramenewFrame_FormalCountEqualsFlags(flagsWithTwoTrue, boolsCount);
}

//  a parameterized test
void test_FramenewFrame_FormalCountEqualsFlags(myBoolList flags, int countExpected)
{
    myLabel fakeLabel = 0;

    myFrame frame = Frame_newFrame(fakeLabel, flags);

    int formalsCount = getFormalCountFromFrame(frame);
    CU_ASSERT_EQUAL(formalsCount, countExpected);
}

///////////////////////////////

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
    int localVarCountBefore = getLocalCountFromFrame(frame);

    Frame_allocateLocal(frame, flag);

    int localVarCountAfter = getLocalCountFromFrame(frame);
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

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_FramemakeBoolList_ByDefault_MakeWhatPassed },
        { "", test_FramemakeAccessList_ByDefault_MakeWhatPasses },

        { "", test_FramenewFrame_AllEscapes_FormalCountEquals },
        { "", test_FramenewFrame_NotAllEscapes_FormalCountEquals },

        { "", test_FrameallocateLocal_AllocateInFrame_LocalCountIncremented },
        { "", test_FrameallocateLocal_AllocateInReg_LocalCountNotChange },
        { "", test_FrameallocateLocal_AllocateInFrame_FormalCountNotChange },
        { "", test_FrameallocateLocal_AllocateInReg_FormalCountNotChange }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}