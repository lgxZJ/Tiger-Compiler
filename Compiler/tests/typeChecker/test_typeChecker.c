#include "../testHelper.h"

#include "../../typeMaker.h"
#include "../../typeChecker.h"


//////////////////////////////////////////////////////////////////////////////////////

void test_checkTypeRecordContainsFields_PassNullField_ReturnNoReturnType(void)
{
    mySymbol symbol = MySymbol_MakeSymbol("iddd");
    myType type = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol, makeType_Int()),
        NULL));

    mySymbol nullField = NULL;
    myType resultType = checkType_RecordContainsFields(
        type->u.typeRecord, nullField);

    CU_ASSERT_EQUAL(resultType->kind, TypeNoReturn);
}

void test_checkTypeRecordContainsFields_BeDefault_ReturnFalseOnNullTypeRecord(void)
{
    myTypeRecord recordType = NULL;

    bool result = checkType_RecordContainsFields(recordType, NULL);

    CU_ASSERT_EQUAL(result, false);
}

void test_checkTypeRecordContainsFields_BeDefault_ReturnFalseOnNullTypeRecordFeilds(void)
{
    myType type = makeType_Record(NULL);

    bool result = checkType_RecordContainsFields(type->u.typeRecord, NULL);

    CU_ASSERT_EQUAL(result, false);
}

void test_checkTypeRecordContainsFields_InvalidFieldName_ReturnNULL(void)
{
    mySymbol symbol = MySymbol_MakeSymbol("iddd");
    myType type = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol, makeType_Int()),
        NULL));

    mySymbol invalidFieldSymbol = MySymbol_MakeSymbol("notfield");
    myType resultType = checkType_RecordContainsFields(
        type->u.typeRecord, invalidFieldSymbol);

    CU_ASSERT_EQUAL(resultType, NULL);
}

void test_checkTypeRecordContainsFields_BeDefault_ReturnTrueOnFields(void)
{
    mySymbol symbol = MySymbol_MakeSymbol("iddd");
    myType type = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol, makeType_Int()),
        NULL));

    myType resultType = checkType_RecordContainsFields(type->u.typeRecord, symbol);

    CU_ASSERT_EQUAL(resultType->kind, TypeInt);
}

/////////////////////////////////////////////////////////////////////////////////////

void test_isTypeEqual_CheckRecord_WorkRight(void)
{
    mySymbol symbol1 = MySymbol_MakeSymbol("symbol1");
    mySymbol symbol2 = MySymbol_MakeSymbol("symbol2");
    myType firstType = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol1, makeType_Int()),
        makeType_TypeFieldList(
            makeType_TypeField(symbol2, makeType_String()),
            NULL)));
    myType secondType = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol2, makeType_String()),
        makeType_TypeFieldList(
            makeType_TypeField(symbol1, makeType_Nil()),
            NULL)));

    bool firstCompare = isTypeEqual(firstType, firstType);
    bool secondCompare = isTypeEqual(firstType, secondType);

    CU_ASSERT_EQUAL(firstCompare, true);
    CU_ASSERT_EQUAL(secondCompare, false);
}


void test_isTypeEqual_CheckPrimitiveTypes_WorksRightOnEquality(void)
{
    myType typeInt = makeType_Int();
    myType typeString = makeType_String();
    myType typeNil = makeType_Nil();
    myType typeNoReturn = makeType_NoReturn();

    bool resultInt = isTypeEqual(typeInt, typeInt);
    bool resultString = isTypeEqual(typeString, typeString);
    bool resultNil = isTypeEqual(typeNil, typeNil);
    bool resultNoReturn = isTypeEqual(typeNoReturn, typeNoReturn);

    CU_ASSERT_EQUAL(resultInt, true);
    CU_ASSERT_EQUAL(resultString, true);
    CU_ASSERT_EQUAL(resultNil, true);
    CU_ASSERT_EQUAL(resultNoReturn, true);
}

void test_isTypeEqual_CheckPrimitiveTypes_WorksRightOnNonEquality(void)
{
    myType typeInt = makeType_Int();
    myType typeString = makeType_String();
    myType typeNil = makeType_Nil();
    myType typeNoReturn = makeType_NoReturn();

    bool resultFirst = isTypeEqual(typeInt, typeString);
    bool resultSecond = isTypeEqual(typeString, typeNil);
    bool resultThrid = isTypeEqual(typeNil, typeNoReturn);
    bool resultFouth = isTypeEqual(typeNoReturn, typeInt);

    CU_ASSERT_EQUAL(resultFirst, false);
    CU_ASSERT_EQUAL(resultSecond, false);
    CU_ASSERT_EQUAL(resultThrid, false);
    CU_ASSERT_EQUAL(resultFirst, false);
}

void test_isTypeEqual_CheckArray_WorksRight(void)
{
    myType typeArrayOne = makeType_Array(makeType_Array(makeType_Int()));
    myType typeArraySimilar = makeType_Array(makeType_Array(makeType_Int()));
    myType typeArrayDifferent = makeType_Array(makeType_Int());

    bool resultSameType = isTypeEqual(typeArrayOne,typeArrayOne);
    bool resultSimilarType = isTypeEqual(typeArrayOne, typeArraySimilar);
    bool resultDifferentType = isTypeEqual(typeArrayOne, typeArrayDifferent);

    CU_ASSERT_EQUAL(resultSameType, true);
    CU_ASSERT_EQUAL(resultSimilarType, false);  
    CU_ASSERT_EQUAL(resultDifferentType, false);
}

void test_isTypeEqual_CheckNamed_WorksRight(void)
{
    mySymbol symbol = MySymbol_MakeSymbol("symbol");
    myType typeNamedOne = makeType_Named(symbol, makeType_Array(makeType_Int()));
    myType typeNamedDiffreent = makeType_Named(symbol, makeType_Int());

    bool resultCheckSame = isTypeEqual(typeNamedOne, typeNamedOne);
    bool resultCheckDifferent = isTypeEqual(typeNamedOne, typeNamedDiffreent);

    CU_ASSERT_EQUAL(resultCheckSame, true);
    CU_ASSERT_EQUAL(resultCheckDifferent, false);
}


//////////////////////////////////////////////////////////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))
        return -1;

    CU_TestInfo tests[] = {
        { "", test_checkTypeRecordContainsFields_PassNullField_ReturnNoReturnType },
        { "", test_checkTypeRecordContainsFields_InvalidFieldName_ReturnNULL },
        { "test checkType_RecordContainsField",
          test_checkTypeRecordContainsFields_BeDefault_ReturnFalseOnNullTypeRecord },
        { "", test_checkTypeRecordContainsFields_BeDefault_ReturnFalseOnNullTypeRecordFeilds },
        { "", test_checkTypeRecordContainsFields_BeDefault_ReturnTrueOnFields },

        { "", test_isTypeEqual_CheckRecord_WorkRight },
        { "", test_isTypeEqual_CheckPrimitiveTypes_WorksRightOnEquality },
        { "", test_isTypeEqual_CheckPrimitiveTypes_WorksRightOnNonEquality },
        { "", test_isTypeEqual_CheckArray_WorksRight },
        { "", test_isTypeEqual_CheckNamed_WorksRight }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;

    if (!runTests(&suite))
        return -1;

    freeMemoryChain();
    return 0;
}