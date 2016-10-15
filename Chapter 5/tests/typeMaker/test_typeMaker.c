#include "../testHelper.h"

#include "../../makeMemory.h"
#include "../../typeMaker.h"

/**
 *  NOTE:   functions in 'myTypes.c' are simple make-functions,
 *  so, no much more tests are needed. 
 */

void test_makeTypeEmpty_ByDefault_MakeAValidEmptyType(void)
{
    myType result = makeType_Empty();

    CU_ASSERT_EQUAL(result->kind, TypeEmpty);
}

void test_makeTypeInt_ByDefault_MakeAValidIntType(void)
{
    myType result = makeType_Int();

    CU_ASSERT(result != NULL);
    CU_ASSERT(result->kind == TypeInt);
}

void test_makeTypeString_ByDefault_MakeAValidStringType(void)
{
    myType result = makeType_String();

    CU_ASSERT(result != NULL);
    CU_ASSERT(result->kind == TypeString);
}

void test_makeTypeNil_ByDefault_MakeAValidNilType(void)
{
    myType result = makeType_Nil();

    CU_ASSERT(result != NULL);
    CU_ASSERT(result->kind == TypeNil);
}

void test_makeTypeNoReturn_ByDefault_MakeAValidNoReturnType(void)
{
    myType result = makeType_NoReturn();

    CU_ASSERT(result != NULL);
    CU_ASSERT(result->kind == TypeNoReturn);
}

void test_makeTypeRecord_ByDefault_MakeAValidRecordType(void)
{
    myTypeFieldList fakeParam = NULL;

    myType result = makeType_Record(fakeParam);

    CU_ASSERT(result != NULL);
    CU_ASSERT(result->kind == TypeRecord);
    CU_ASSERT_EQUAL(result->u.typeRecord->fieldList, fakeParam);
}

void test_makeTypeArray_ByDefault_MakeAValidArrayType(void)
{
    myType fakeParam = NULL;

    myType result = makeType_Array(fakeParam);

    CU_ASSERT(result != NULL);
    CU_ASSERT_EQUAL(result->kind, TypeArray);
    CU_ASSERT_EQUAL(result->u.typeArray->type, fakeParam);
}


void test_makeTypeNamed_ByDefault_MakeAValidNamedType(void)
{
    mySymbol fakeTypeName = NULL;
    myType fakeNamedType = NULL;

    myType result = makeType_Named(fakeTypeName, fakeNamedType);

    CU_ASSERT(result != NULL);
    CU_ASSERT_EQUAL(result->kind, TypeNamed);
    CU_ASSERT_EQUAL(result->u.typeNamed->name, fakeTypeName);
    CU_ASSERT_EQUAL(result->u.typeNamed->type, fakeNamedType);
}

void test_makeTypeTypeField_ByDefault_MakeAValidTypeField(void)
{
    mySymbol fakeName = NULL;
    myType fakeType = NULL;

    myTypeField result = makeType_TypeField(fakeName, fakeType);

    CU_ASSERT(result != NULL);
    CU_ASSERT_EQUAL(result->name, fakeName);
    CU_ASSERT_EQUAL(result->type, fakeType);
}

void test_makeTypeTypeFieldList_ByDefault_MakeAValidTypeFieldList(void)
{
    myTypeField fakeHead = NULL;
    myTypeFieldList fakeTails = NULL;

    myTypeFieldList result = makeType_TypeFieldList(fakeHead, fakeTails);

    CU_ASSERT(result != NULL);
    CU_ASSERT_EQUAL(result->head, fakeHead);
    CU_ASSERT_EQUAL(result->tails, fakeTails);
}

void test_makeTypeTypeList_ByDefault_MakeAValidTypeList(void)
{
    myType fakeHead = NULL;
    myTypeList fakeTails = NULL;

    myTypeList result = makeType_TypeList(fakeHead, fakeTails);

    CU_ASSERT(result != NULL);
    CU_ASSERT_EQUAL(result->head, fakeHead);
    CU_ASSERT_EQUAL(result->tails, fakeTails);
}

///////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
    CU_pSuite suite;
    if (!initialTests(&suite))      return -1;

    //	add tests
    CU_TestInfo tests[] = {
        { "", test_makeTypeEmpty_ByDefault_MakeAValidEmptyType },
        { "", test_makeTypeInt_ByDefault_MakeAValidIntType },
        { "", test_makeTypeString_ByDefault_MakeAValidStringType },
        { "", test_makeTypeNil_ByDefault_MakeAValidNilType },
        { "", test_makeTypeNoReturn_ByDefault_MakeAValidNoReturnType },
        { "", test_makeTypeRecord_ByDefault_MakeAValidRecordType },
        { "", test_makeTypeArray_ByDefault_MakeAValidArrayType },
        { "", test_makeTypeNamed_ByDefault_MakeAValidNamedType },
        { "", test_makeTypeTypeField_ByDefault_MakeAValidTypeField },
        { "", test_makeTypeTypeFieldList_ByDefault_MakeAValidTypeFieldList },
        { "", test_makeTypeTypeList_ByDefault_MakeAValidTypeList }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;

    if (!runTests(&suite))          return -1;
    
    freeMemoryChain();
    return 0;
}