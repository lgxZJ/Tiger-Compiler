#include "../testHelper.h"

#include "../../breakGetter.h"

#include <stdlib.h>

///////////////////////        tests       ////////////////////////

void test_PushBreakTarget_ByDefault_GetWhatPushed(void)
{
    myLabel fakeLabel = (myLabel)12;
    pushBreakTarget(fakeLabel);

    myLabel labelGot = getBreakTarget();
    CU_ASSERT_EQUAL(labelGot, fakeLabel);

    fakeLabel = (myLabel)2323;
    pushBreakTarget(fakeLabel);

    labelGot = getBreakTarget();
    CU_ASSERT_EQUAL(labelGot, fakeLabel);
}

void test_PopBreakTarget_AfterCall_GetFormerOne(void)
{
    myLabel fakeLabelOne = (myLabel)12;
    pushBreakTarget(fakeLabelOne);
    myLabel fakeLabelTwo = (myLabel)23;
    pushBreakTarget(fakeLabelTwo);

    popBreakTarget();
    myLabel labelGot = getBreakTarget();

    CU_ASSERT_EQUAL(labelGot, fakeLabelOne);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_PushBreakTarget_ByDefault_GetWhatPushed },
        { "", test_PopBreakTarget_AfterCall_GetFormerOne },
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
