#include "../testHelper.h"

#include "../../lValueTreater.h"

#include <stdlib.h>

///////////////////////     test helpers    ///////////////////////



///////////////////////        tests       ////////////////////////

void test_TreatLValueAsAddress_AfterCall_TreatLValueAsItIs(void)
{
    treatLValueAsAddress();

    bool asAddress = isLValueAsAddress();
    CU_ASSERT_EQUAL(asAddress, true);
}

void test_TreatLValueAsContent_AfterCall_TreatLValueAsItIs(void)
{
    treatLValueAsContent();

    bool asContent = isLValueAsContent();
    CU_ASSERT_EQUAL(asContent, true);
}

void test_LValueTreater_ByDefault_OneResultIsOpposedToAnother(void)
{
    bool asAddress = isLValueAsAddress();
    bool asContent = isLValueAsContent();

    CU_ASSERT_EQUAL(asAddress, !asContent);
}

void test_NestedTreat_AddressFollowedByContent_AsContent()
{
    treatLValueAsAddress();
    treatLValueAsContent();

    CU_ASSERT_EQUAL(isLValueAsContent(), true);
}

void test_NestedTreat_ContentFollowedByAddress_AsAddress()
{
    treatLValueAsContent();
    treatLValueAsAddress();

    CU_ASSERT_EQUAL(isLValueAsAddress(), true);
}

void test_CancelTreatAsAddress_FormerContent_ReturnContent()
{
    treatLValueAsContent();
    treatLValueAsAddress();
    cancelTreatAsAddress();

    CU_ASSERT_EQUAL(isLValueAsContent(), true);
}

void test_CancelTreatAsContent_FormerAddress_ReturnAddress()
{
    treatLValueAsAddress();
    treatLValueAsContent();
    cancelTreatAsContent();

    CU_ASSERT_EQUAL(isLValueAsAddress(), true);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_TreatLValueAsAddress_AfterCall_TreatLValueAsItIs },
        { "", test_TreatLValueAsContent_AfterCall_TreatLValueAsItIs },
        { "", test_LValueTreater_ByDefault_OneResultIsOpposedToAnother },
        { "", test_NestedTreat_AddressFollowedByContent_AsContent },
        { "", test_NestedTreat_ContentFollowedByAddress_AsAddress },
        { "", test_CancelTreatAsAddress_FormerContent_ReturnContent },
        { "", test_CancelTreatAsContent_FormerAddress_ReturnAddress },
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}
