#include "../testHelper.h"

#include "../../myTranslate.h"

#include <stdlib.h>

///////////////////////     test helpers    ///////////////////////

int getFormalCountFromLevel(Trans_myLevel level)
{
    CU_ASSERT((bool)level);
    return Frame_getAccessListCount(Frame_getFormals(level));
}

///////////////////////        tests       ////////////////////////

void test_TransMakeAccessList_ByDefault_MakeWhatPassed(void)
{
    Trans_myAccess headPassed = (Trans_myAccess)12;
    Trans_myAccessList tailPassed = (Trans_myAccessList)34;

    Trans_myAccessList listGot = Trans_makeAccessList(headPassed, tailPassed);

    CU_ASSERT_EQUAL(headPassed, listGot->head);
    CU_ASSERT_EQUAL(tailPassed, listGot->tail); 
}

///////////////////////

void test_TransIsLevelEquals_ByDefault_WorkRight(void)
{
    //  todo:
}

///////////////////////

void test_TransOutermostLevel_ByDefault_ContainsNoFormals(void)
{
    Trans_myLevel outermostLevel = Trans_outermostLevel();

    int formalCount = getFormalCountFromLevel(outermostLevel);
    CU_ASSERT_EQUAL(formalCount, 0);
}

void test_TransOutermostLevel_EveryCall_ReturnSame(void)
{
    Trans_myLevel firstLevel = Trans_outermostLevel();
    Trans_myLevel secondLevel = Trans_outermostLevel();

    CU_ASSERT(Trans_isLevelEqual(firstLevel, secondLevel));
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_TransMakeAccessList_ByDefault_MakeWhatPassed },

        { "", test_TransOutermostLevel_ByDefault_ContainsNoFormals },
        { "", test_TransOutermostLevel_EveryCall_ReturnSame }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
