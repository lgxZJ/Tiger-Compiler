#include "../testHelper.h"

#include "../../myTranslate.h"

#include <stdlib.h>

//////////////////////     private forwards  //////////////////////
myFrame Trans_getFrame(Trans_myLevel level);

///////////////////////     test helpers    ///////////////////////

int getFormalCountFromLevel(Trans_myLevel level)
{
    CU_ASSERT((bool)level);
    return Trans_getAccessListCount(Trans_getFormals(level));
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

void test_TransGetAccessListCount_ByDefault_WorkRight(void)
{
    Trans_myAccessList twoElementList = Trans_makeAccessList((Trans_myAccess)NULL,
        Trans_makeAccessList((Trans_myAccess)NULL, NULL));

    int countGot = Trans_getAccessListCount(twoElementList);

    CU_ASSERT_EQUAL(2, countGot);
}

///////////////////////

void test_TransIsLevelEquals_ByDefault_WorkRight(void)
{
    Trans_myLevel levelOne = Trans_newLevel(Trans_outermostLevel(),
        (myLabel)34, (myBoolList)NULL);
    Trans_myLevel levelTwo = Trans_newLevel(Trans_outermostLevel(),
        (myLabel)45, (myBoolList)NULL);

    CU_ASSERT(Trans_isLevelEqual(levelOne, levelOne));
    CU_ASSERT(!Trans_isLevelEqual(levelOne, levelTwo));
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

///////////////////////

void test_TransNewLevel_EveryCall_ReturnDifferent(void)
{
    Trans_myLevel fakeLevel = Trans_outermostLevel();
    myLabel fakeLabel = (myLabel)34;
    myBoolList fakeFormals = (myBoolList)NULL;
    Trans_myLevel levelOne = Trans_newLevel(fakeLevel, fakeLabel, fakeFormals);
    Trans_myLevel levelTwo = Trans_newLevel(fakeLevel, fakeLabel, fakeFormals);

    CU_ASSERT(!Trans_isLevelEqual(levelOne, levelTwo));
}

void test_TransNewLevel_AfterNewLevel_ParentLevelHoldActualFormals(void)
{
    Trans_myLevel fakeLevel = Trans_outermostLevel();
    myLabel fakeLabel = (myLabel)34;
    myBoolList fakeFormals = Frame_makeBoolList(NULL, true);

    Trans_myLevel levelOne = Trans_newLevel(fakeLevel, fakeLabel, fakeFormals);

    int expectedAccessCount = Frame_getBoolListCount(fakeFormals) + 2;
    CU_ASSERT_EQUAL(
        Frame_getAccessListCount(Frame_getFormals(Trans_getFrame(fakeLevel))),
        expectedAccessCount);
}

///////////////////////

void test_TransAllocateLocal_ByDefault_AccessWithinGivenLevel(void)
{
    Trans_myLevel level = Trans_outermostLevel();

    Trans_myAccess accessOne = Trans_allocateLocal(level, true);
    Trans_myAccess accessTwo = Trans_allocateLocal(level, false);

    CU_ASSERT(Trans_isLevelEqual(level, Trans_getAccessLevel(accessOne)));
    CU_ASSERT(Trans_isLevelEqual(level, Trans_getAccessLevel(accessTwo)));
}

///////////////////////

void test_TransGetFormals_PassOutermostLevel_FormalCountIsZero(void)
{
    Trans_myLevel outermostLevel = Trans_outermostLevel();

    Trans_myAccessList formals = Trans_getFormals(outermostLevel);

    int formalCount = Trans_getAccessListCount(formals);
    CU_ASSERT_EQUAL(formalCount, 0);
}

void test_TransGetFormals_PassOtherLevels_FormalCountEqualsToBoolFlags(void)
{
    myBoolList oneFlags = Frame_makeBoolList(NULL, true);
    myBoolList twoFlags = Frame_makeBoolList(Frame_makeBoolList(NULL, true), true);
    Trans_myLevel levelOne = Trans_newLevel(Trans_outermostLevel(),
        (myLabel)NULL, oneFlags);
    Trans_myLevel levelTwo = Trans_newLevel(Trans_outermostLevel(),
        (myLabel)NULL, twoFlags);

    int formalCountOne = getFormalCountFromLevel(levelOne);
    int formalCountTwo = getFormalCountFromLevel(levelTwo);

    CU_ASSERT_EQUAL(formalCountOne, 1);
    CU_ASSERT_EQUAL(formalCountTwo, 2);
}


///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_TransMakeAccessList_ByDefault_MakeWhatPassed },

        { "", test_TransGetAccessListCount_ByDefault_WorkRight },

        { "", test_TransIsLevelEquals_ByDefault_WorkRight },

        { "", test_TransOutermostLevel_ByDefault_ContainsNoFormals },
        { "", test_TransOutermostLevel_EveryCall_ReturnSame },

        { "", test_TransNewLevel_EveryCall_ReturnDifferent },
        { "", test_TransNewLevel_AfterNewLevel_ParentLevelHoldActualFormals },

        { "", test_TransAllocateLocal_ByDefault_AccessWithinGivenLevel },

        { "", test_TransGetFormals_PassOtherLevels_FormalCountEqualsToBoolFlags },
        { "", test_TransGetFormals_PassOutermostLevel_FormalCountIsZero }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
