#include "../testHelper.h"

#include "../../breakChecker.h"

#include <stdlib.h>

////////////////////////    test functions    //////////////////////////

void test_enterLoop_AfterCall_InsideLoop(void)
{
    enterLoop();

    bool result = isBreakInsideLoop();
    CU_ASSERT_EQUAL(result, true);
}

void test_leaveLoop_AfterCall_NotInsideLoop(void)
{
    enterLoop();
    leaveLoop();

    bool result = isBreakInsideLoop();
    CU_ASSERT_EQUAL(result, false);
}

//////////////////////      test    runner     /////////////////////////
int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_enterLoop_AfterCall_InsideLoop },
        { "", test_leaveLoop_AfterCall_NotInsideLoop }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))      return EXIT_FAILURE;
    return 0;
}  