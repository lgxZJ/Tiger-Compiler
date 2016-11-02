#include "../testHelper.h"
#include "../testFixture.h"

#include "../../forChecker.h"

#include <stdlib.h>

///////////////////////     test helpers    ///////////////////////



///////////////////////        tests       ////////////////////////

void test_EnterForLoop_AfterCall_IsInsideLoop(void)
{
    enterForLoop(MySymbol_MakeSymbol("symbol"));

    bool result = isInsideForLoop();
    CU_ASSERT_EQUAL(result, true);

    //  clean up
    leaveForLoop();
}

////////////////////////////////////////////////////////

void test_LeaveForLoop_AfterCall_NotInsideLoop(void)
{
    enterForLoop(MySymbol_MakeSymbol("symbol"));
    leaveForLoop();

    bool result = isInsideForLoop();
    CU_ASSERT_EQUAL(result, false);
}

/////////////////////////////////////////////////////////

void test_IsLoopVarAssigned_AfterEnterLoopPassLoopVar_ReturnTrue(void)
{
    mySymbol loopVarName = MySymbol_MakeSymbol("symbol");
    enterForLoop(loopVarName);

    bool result = isLoopVarAssigned(loopVarName);
    CU_ASSERT_EQUAL(result, true);

    //  clean up
    leaveForLoop();
}

void test_IsLoopVarAssigned_AfterEnterLoopPassNotLoopVar_ReturnFalse(void)
{
    mySymbol loopVarName = MySymbol_MakeSymbol("symbol");
    enterForLoop(loopVarName);

    mySymbol anotherVarName = MySymbol_MakeSymbol("another");
    bool result = isLoopVarAssigned(anotherVarName);
    CU_ASSERT_EQUAL(result, false);

    //  clean up
    leaveForLoop();
}

void test_IsLoopVarAssigned_AfterLeaveLoopPassLoopVar_ReturnFalse(void)
{
    mySymbol loopVarName = MySymbol_MakeSymbol("symbol");
    enterForLoop(loopVarName);
    leaveForLoop();

    bool result = isLoopVarAssigned(loopVarName);
    CU_ASSERT_EQUAL(result, false);
}

void test_IsLoopVarAssigned_AfterLeaveLoopPassNotLoopVar_ReturnFalse(void)
{
    mySymbol loopVarName = MySymbol_MakeSymbol("symbol");
    enterForLoop(loopVarName);
    leaveForLoop();

    mySymbol anotherVarName = MySymbol_MakeSymbol("another");
    bool result = isLoopVarAssigned(anotherVarName);
    CU_ASSERT_EQUAL(result, false);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_EnterForLoop_AfterCall_IsInsideLoop },

        { "", test_LeaveForLoop_AfterCall_NotInsideLoop },

        { "", test_IsLoopVarAssigned_AfterEnterLoopPassLoopVar_ReturnTrue },
        { "", test_IsLoopVarAssigned_AfterEnterLoopPassNotLoopVar_ReturnFalse },
        { "", test_IsLoopVarAssigned_AfterLeaveLoopPassLoopVar_ReturnFalse },
        { "", test_IsLoopVarAssigned_AfterLeaveLoopPassNotLoopVar_ReturnFalse }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
