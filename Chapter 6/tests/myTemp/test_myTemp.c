#include "../testHelper.h"

#include "../../myTemp.h"

#include <stdlib.h>

///////////////////////     internal forwards   ///////////////////

int Temp_getTotalTempCount(void);
int Temp_getTotalLabelCount(void);

///////////////////////     test helpers    ///////////////////////



///////////////////////        tests       ////////////////////////

void test_TempNewTemp_ByDefault_ReturnNotNull(void)
{
    myTemp temp = Temp_newTemp();

    CU_ASSERT_NOT_EQUAL(temp, NULL);
}

void test_TempNewTemp_MadeOne_IncrementTotalTempNum(void)
{
    int totalNumBefore = Temp_getTotalTempCount();

    Temp_newTemp();

    int totalNumAfter = Temp_getTotalTempCount();
    CU_ASSERT_EQUAL(totalNumAfter - totalNumBefore, 1);
}

///////////////////////////

void test_TempNewLabel_ByDefault_ReturnNotNull(void)
{
    myLabel label = Temp_newLabel();

    CU_ASSERT_NOT_EQUAL(label, NULL);
}

void test_TempNewLabel_MadeOne_IncrementTotalLabelNum(void)
{
    int numBefore = Temp_getTotalLabelCount();

    Temp_newLabel();

    int numAfter = Temp_getTotalLabelCount();
    CU_ASSERT_EQUAL(numAfter - numBefore, 1);
}

void test_TempNewLabel_TwoLabels_NeverSame(void)
{
    myLabel labelOne = Temp_newLabel();
    myLabel labelTwo = Temp_newLabel();

    CU_ASSERT(!Temp_isLabelEqual(labelOne, labelTwo));
}

//////////////////////////////

void test_TempNewNamedLabel_ByDefault_ReturnNotNull(void)
{
    myLabel label = Temp_newNamedLabel("qw");

    CU_ASSERT_NOT_EQUAL(label, NULL);
}

void test_TempNewNamedLabel_SameName_ReturnSame(void)
{
    myLabel labelOne = Temp_newNamedLabel("same");
    myLabel labelTwo = Temp_newNamedLabel("same");

    CU_ASSERT(Temp_isLabelEqual(labelOne, labelTwo));
}

void test_TempNewNamedLabel_DifferentName_ReturnDifferent(void)
{
    myLabel labelOne = Temp_newNamedLabel("one");
    myLabel labelTwo = Temp_newNamedLabel("two");

    CU_ASSERT(!Temp_isLabelEqual(labelOne, labelTwo));
}

///////////////////////

void test_TempGetLabelString_SameLabel_ReturnSame(void)
{
    myLabel label = Temp_newLabel();

    myString firstGot = Temp_getLabelString(label);
    myString secondGot = Temp_getLabelString(label);

    CU_ASSERT_EQUAL(firstGot, secondGot);
}

void test_TempGetLabelString_DifferentLabel_ReturnDifferent(void)
{
    myLabel labelOne = Temp_newLabel();
    myLabel labelTwo = Temp_newLabel();

    myString firstGot = Temp_getLabelString(labelOne);
    myString secondGot = Temp_getLabelString(labelTwo);

    CU_ASSERT_NOT_EQUAL(firstGot, secondGot);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_TempNewTemp_ByDefault_ReturnNotNull },
        { "", test_TempNewTemp_MadeOne_IncrementTotalTempNum },

        { "", test_TempNewLabel_ByDefault_ReturnNotNull },
        { "", test_TempNewLabel_MadeOne_IncrementTotalLabelNum },
        { "", test_TempNewLabel_TwoLabels_NeverSame },

        { "", test_TempNewNamedLabel_ByDefault_ReturnNotNull },
        { "", test_TempNewNamedLabel_SameName_ReturnSame },
        { "", test_TempNewNamedLabel_DifferentName_ReturnDifferent },

        { "", test_TempGetLabelString_SameLabel_ReturnSame },
        { "", test_TempGetLabelString_DifferentLabel_ReturnDifferent }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}