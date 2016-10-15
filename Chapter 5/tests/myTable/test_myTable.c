#include "../testHelper.h"

#include "../../myTable.h"

///////////////////////////////////////////////////////////////////////

void test_MyTableMakeEmptyTable_ByDefault_ContainsNothing(void)
{
	myTable table = MyTable_MakeEmptyTable_();

	void* popResult = MyTable_Pop_(table);

    CU_ASSERT_EQUAL(popResult, NULL);
}

////////////////////////////////////////////////////

void test_MyTableEnter_NullTableOrKey_ReturnFalse(void)
{
	myTable	table = MyTable_MakeEmptyTable_();
    myTable nullTable = NULL;
    void* keyOrValue = (void*)0xffffff;
    void* nullKey = NULL;

    bool nullTableResult = MyTable_Enter_(nullTable, keyOrValue, keyOrValue);
    bool nullKeyResult = MyTable_Enter_(table, nullKey, keyOrValue);

    CU_ASSERT_EQUAL(nullTableResult, false);
    CU_ASSERT_EQUAL(nullKeyResult, false);
}

void test_MyTableEnter_ValidParams_ReturnTrueAndOneAdded(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* key = (void*)12;
    void* value = (void*)43;

    bool result = MyTable_Enter_(table, key, value);

    void* valueGot = MyTable_Look_(table, key);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(valueGot, value);
}

void test_MyTableEnter_OneAdded_CouldFindDifferentFormers(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* key_one = (void*)12;
    void* key_two = (void*)13;
    void* value_one = key_one;
    void* value_two = key_two;
    MyTable_Enter_(table, key_one, value_one);
    MyTable_Enter_(table, key_two, value_two);

    void* another_key = (void*)1414;
    void* another_vajlue = another_key;
    MyTable_Enter_(table, another_key, another_vajlue);

    void* value_one_got = MyTable_Look_(table, key_one);
    void* value_two_got = MyTable_Look_(table, key_two);
    CU_ASSERT_EQUAL(value_one_got, value_one);
    CU_ASSERT_EQUAL(value_two_got, value_two);
}

void test_MyTableEnter_AddTwoSameKey_FindLaterOne(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* key = (void*)12;
    void* value_first = (void*)23;
    void* value_second = (void*)34;
    MyTable_Enter_(table, key, value_first);
    MyTable_Enter_(table, key, value_second);

    void* findResult = MyTable_Look_(table, key);
    CU_ASSERT_EQUAL(findResult, value_second);
}

void test_MyTableEnter_PopOneAfterTwoSameKeyAdded_FindFormerOne(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* key = (void*)12;
    void* value_first = (void*)23;
    void* value_second = (void*)34;
    MyTable_Enter_(table, key, value_first);
    MyTable_Enter_(table, key, value_second);

    MyTable_Pop_(table);

    void* findResult = MyTable_Look_(table, key);
    CU_ASSERT_EQUAL(findResult, value_first);
}

///////////////////////////////////////////////////////

void test_MyTableLook_InvalidParams_ReturnNull(void)
{
  myTable table = MyTable_MakeEmptyTable_();
  void* nullTable = NULL;
  void* nullKey = NULL;
  void* validKeyOrValue = (void*)12;

  void* nullTableResult = MyTable_Look_(nullTable, validKeyOrValue);
  CU_ASSERT_EQUAL(nullTable, NULL);

  void* nullKeyResult = MyTable_Look_(table, nullKey);
  CU_ASSERT_EQUAL(nullKeyResult, NULL);
}

void test_MyTableLook_PassEnteredKeys_FindOne(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* key_one = (void*)12;
    void* key_two = (void*)23;
    void* key_three = (void*)67;  
    void* value_one = (void*)41;
    void* value_two = (void*)56;
    void* value_three = (void*)89;
    MyTable_Enter_(table, key_one, value_one);
    MyTable_Enter_(table, key_two, value_two);
    MyTable_Enter_(table, key_three, value_three);

    void* findOneResult = MyTable_Look_(table, key_one);
    void* findTwoResult = MyTable_Look_(table, key_two);
    void* findThreeResult = MyTable_Look_(table, key_three);

    CU_ASSERT_EQUAL(findOneResult, value_one);
    CU_ASSERT_EQUAL(findTwoResult, value_two);
    CU_ASSERT_EQUAL(findThreeResult, value_three);
}

void test_MyTableLook_PassKeyNotEntered_ReturnNull(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* notEnteredKey = (void*)1212;

    void* result = MyTable_Look_(table, notEnteredKey);

    CU_ASSERT_EQUAL(result, NULL);
}

///////////////////////////////////////////////////////

void test_MyTableSet_PassNullParams_ReturnFalse(void)
{
    myTable nullTable = NULL;
    myTable validTable = MyTable_MakeEmptyTable_();
    void* nullKey = NULL;
    void* validKeyOrValue = (void*)12;

    bool resultNullTable =
        MyTable_Set_(nullKey, validKeyOrValue, validKeyOrValue);
    bool resultNullKey =
        MyTable_Set_(validTable, nullKey, validKeyOrValue);

    CU_ASSERT_EQUAL(resultNullKey, false);
    CU_ASSERT_EQUAL(resultNullTable, false);
}

void test_MyTableSet_KeyNotEntered_ReturnFalse(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* notEnteredKey = (void*)12;

    bool result = MyTable_Set_(table, notEnteredKey, (void*)12);

    CU_ASSERT_EQUAL(result, false); 
}

void test_MyTableSet_ValidSet_Setted(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* key = (void*)12;
    void* valueBefore = (void*)1;
    MyTable_Enter_(table, key, valueBefore);

    void* valueSetted = (void*)22;
    MyTable_Set_(table, key, valueSetted);

    void* valueGot = MyTable_Look_(table, key);
    CU_ASSERT_EQUAL(valueGot, valueSetted);
}

///////////////////////////////////////////////////////

void test_MyTablePop_NullTableOrKey_ReturnNull(void)
{
    void* nullTable = NULL;

    void* nullTableResult = MyTable_Pop_(nullTable);

    CU_ASSERT_EQUAL(nullTableResult, NULL);
}

void test_MyTablePop_EmptyTable_ReturnNull(void)
{
    myTable table = MyTable_MakeEmptyTable_();

    void* result = MyTable_Pop_(table);

    CU_ASSERT_EQUAL(result, NULL);
}

void test_MyTablePop_TableContainsDifferentPairs_PopsPairsReverse(void)
{
    myTable table = MyTable_MakeEmptyTable_();
    void* key_one = (void*)12;
    void* key_two = (void*)23;
    void* value_one = (void*)41;
    void* value_two = (void*)56;
    MyTable_Enter_(table, key_one, value_one);
    MyTable_Enter_(table, key_two, value_two);

    void* value_two_got = MyTable_Pop_(table);
    void* value_one_got = MyTable_Pop_(table);

    CU_ASSERT_EQUAL(value_two_got, value_two);
    CU_ASSERT_EQUAL(value_one_got, value_one);
}

///////////////////////////////////////////////////////////////////////

int main ()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return -1;

    CU_TestInfo tests[] = {
        { "", test_MyTableMakeEmptyTable_ByDefault_ContainsNothing },

        { "", test_MyTableEnter_NullTableOrKey_ReturnFalse },
        { "", test_MyTableEnter_ValidParams_ReturnTrueAndOneAdded },
        { "", test_MyTableEnter_OneAdded_CouldFindDifferentFormers },
        { "", test_MyTableEnter_AddTwoSameKey_FindLaterOne },
        { "", test_MyTableEnter_PopOneAfterTwoSameKeyAdded_FindFormerOne },

        { "", test_MyTableLook_InvalidParams_ReturnNull },
        { "", test_MyTableLook_PassEnteredKeys_FindOne },
        { "", test_MyTableLook_PassKeyNotEntered_ReturnNull },

        { "", test_MyTableSet_PassNullParams_ReturnFalse },
        { "", test_MyTableSet_KeyNotEntered_ReturnFalse },
        { "", test_MyTableSet_ValidSet_Setted },

        { "", test_MyTablePop_NullTableOrKey_ReturnNull },
        { "", test_MyTablePop_EmptyTable_ReturnNull },
        { "", test_MyTablePop_TableContainsDifferentPairs_PopsPairsReverse }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;

    if (!runTests(&suite))  return -1;
    freeMemoryChain();
    return 0;
}