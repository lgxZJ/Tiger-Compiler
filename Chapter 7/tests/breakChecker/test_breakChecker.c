#include "../testHelper.h"

#include "../../breakChecker.h"

#include <stdlib.h>

////////////////////////    test functions    //////////////////////////

void test_enterLoop_AfterCall_InsideLoop(void)
{
    resetChecker();
    enterLoop();

    bool result = isBreakInsideLoop();
    CU_ASSERT_EQUAL(result, true);
}

void test_leaveLoop_AfterCall_NotInsideLoop(void)
{
    resetChecker();
    enterLoop();
    leaveLoop();

    bool result = isBreakInsideLoop();
    CU_ASSERT_EQUAL(result, false);
}

void test_enterLoop_TwiceEnterOneLeave_InsideLoop(void)
{
    resetChecker();
    enterLoop();
    enterLoop();
    leaveLoop();

    bool result = isBreakInsideLoop();
    CU_ASSERT_EQUAL(result, true);
}

//////////////////////      test    runner     /////////////////////////
int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_enterLoop_AfterCall_InsideLoop },
        { "", test_leaveLoop_AfterCall_NotInsideLoop },
        { "", test_enterLoop_TwiceEnterOneLeave_InsideLoop },
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))      return EXIT_FAILURE;
    return 0;
}  