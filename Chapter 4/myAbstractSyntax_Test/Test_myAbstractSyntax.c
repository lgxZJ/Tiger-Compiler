#include "../myAbstractSyntax.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>

/*------------------------test functions---------------------------*/

//	NOTE:
//
//	Functions in this file are sets of structure makers,
//	they simply assign memory and assign value to its members
//	without check, so some functions are not tested.
//

void makeMyTyField_ShouldWorkRightBothOnValidAndInvalidParams(void)
{
    //	fixture setup
    mySymbol varName = MySymbol_MakeSymbol("valid");
    CU_ASSERT_NOT_EQUAL(varName, (mySymbol)NULL);

    mySymbol typeName = MySymbol_MakeSymbol("valid");
    CU_ASSERT_NOT_EQUAL(typeName, (mySymbol)NULL);

    //	system under control
    myTyField tyField_NullVarName = makeMyTyField((mySymbol)NULL, typeName);
    myTyField tyField_NullTypeName = makeMyTyField(varName, (mySymbol)NULL);
    myTyField tyField_BothNull = makeMyTyField((mySymbol)NULL, (mySymbol)NULL);

    myTyField tyField_Valid = makeMyTyField(varName, typeName);

    //	verify
    CU_ASSERT_EQUAL(tyField_NullVarName, (myTyField)NULL);
    CU_ASSERT_EQUAL(tyField_NullTypeName, (myTyField)NULL);
    CU_ASSERT_EQUAL(tyField_BothNull, (myTyField)NULL);

    CU_ASSERT_NOT_EQUAL(tyField_Valid, (myTyField)NULL);
}

void getLValueKind_ShoudReturnSimpleVarOnNullAux()
{
    //	fixture setup
    myLValueAux lValueAux = (myLValueAux)NULL;

    //	system under control
    enum myLValueKind kind = getLValueKind(lValueAux);

    //	verify
    CU_ASSERT_EQUAL(kind, SimpleVar);
}

void getLValueKind_ShouldReturnRecordFieldOnNullSymbolAux()
{
    //	fixture setup
    myExp fakeExp = (myExp)"fake";
    myLValueAux lValueAux = makeMyLValueAux((mySymbol)NULL, fakeExp, (myLValueAux)NULL);

    //	system under control
    enum myLValueKind actualKind = getLValueKind(lValueAux);

    //	verify
    CU_ASSERT_EQUAL(actualKind, ArraySubscript);
}

void getLValueKind_ShouldReturnArraySubScriptOnNullExpAux()
{
    //	fixture setup
    mySymbol symbol = MySymbol_MakeSymbol("symbol");
    myLValueAux lValueAux = makeMyLValueAux(symbol, (myExp)NULL, (myLValueAux)NULL);

    //	system under control
    enum myLValueKind actualKind = getLValueKind(lValueAux);

    //	verify
    CU_ASSERT_EQUAL(actualKind, RecordField);
}

/*-----------------------end of test functions---------------------*/

int main (int argc, char* argv[])
{

    if (CU_initialize_registry() != CUE_SUCCESS)
    {
	printf("attempt to registry, failed!\n");
	exit(-1);
    }

    CU_pSuite suite = CU_add_suite("test suite--myAbstractSyntax.c", NULL, NULL);
    if (suite == NULL)
    {
	printf("attempt to add suite, failed\n");
	exit(-1);
    }

    
    //	add tests (dummy test)
    CU_TestInfo tests[] = {
	{	"test func----makeMyTyField()", makeMyTyField_ShouldWorkRightBothOnValidAndInvalidParams	},
	{	"test func----getLValueKind()_1", getLValueKind_ShoudReturnSimpleVarOnNullAux	},
	{	"test func----getLValueKind()_2", getLValueKind_ShouldReturnRecordFieldOnNullSymbolAux	},
	{	"test func----getLValueKind()_3", getLValueKind_ShouldReturnArraySubScriptOnNullExpAux	}
    };
    
    for (unsigned i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i)
	if (CU_add_test(suite, tests[i].pName, tests[i].pTestFunc) == NULL)
	{
	    printf("attempts to add tests, failed!\n");
	    exit(-1);
	}


    if (CU_basic_run_suite(suite) == CUE_SUCCESS)
    {
	printf("attempt to run suite, failed!\n");
	exit(-1);
    }

    CU_cleanup_registry();
    freeMemoryChain();
    return 0;
}
