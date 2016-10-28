#include "../testHelper.h"

#include "../../mySymbol.h"
#include "../../makeMemory.h"
#include "../../myEnvironment.h"

#include <assert.h>
#include <stdlib.h>

void test_makeVarEntry_ByDefault_MakeOneValidVarEntry(void)
{
    void* fakeVarType = NULL;

    myVarAndFuncEntry entry = myEnvironment_makeVarEntry((Trans_myAccess)NULL,fakeVarType);

    CU_ASSERT(entry != NULL);
    CU_ASSERT(myEnvironment_isVarEntry(entry));
    CU_ASSERT_EQUAL(MyEnvironment_getVarType(entry), fakeVarType);
}

void test_makeFuncEntry_ByDefault_MakeOneValidFuncEntry(void)
{
    void* fakeFormalTypes = NULL;
    void* fakeReturnType = NULL;

    myVarAndFuncEntry entry = myEnvironment_makeFuncEntry(
        (Trans_myLevel)NULL, (myLabel)NULL,
        fakeFormalTypes, fakeReturnType);

    CU_ASSERT(entry != NULL);
    CU_ASSERT(myEnvironment_isFuncEntry(entry));
    CU_ASSERT_EQUAL(MyEnvironment_getFuncFormalTypes(entry), fakeFormalTypes);
    CU_ASSERT_EQUAL(MyEnvironment_getFuncReturnType(entry), fakeReturnType);
}

void test_addNewVarOrFunc_ByDefault_AddOne(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myType fakeType = NULL;
    mySymbol entryName = MySymbol_MakeSymbol("entryName");
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL, fakeType);

    MyEnvironment_addNewVarOrFunc(varAndFuncEnv, entryName, entry);

    myVarAndFuncEntry actualEntry = 
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, entryName); 
    CU_ASSERT_EQUAL(entry, actualEntry);
}

void test_addNewType_ByDefault_AddOne(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    myType expectedType = NULL;
    mySymbol typeName = MySymbol_MakeSymbol("typename");

    MyEnvironment_addNewType(typeEnv, typeName, expectedType);

    myType actualType = MyEnvironment_getTypeFromName(typeEnv, typeName); 
    CU_ASSERT_EQUAL(expectedType, actualType);
}

void test_MyEnvironmentsetTypeFromName_ValidSet_ReturnTrueAndTheOneSetted(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    myType formerType = NULL;
    mySymbol typeName = MySymbol_MakeSymbol("typename");
    MyEnvironment_addNewType(typeEnv, typeName, formerType);

    myType typeSetted = (myType)12;
    bool result =
        MyEnvironment_setTypeFromName(typeEnv, typeName, typeSetted);

    myType typeGot = MyEnvironment_getTypeFromName(typeEnv, typeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(typeGot, typeSetted);
}

void test_myEnvironmentBaseVarAndFunc_BeDefault_ContainPredefinedFuncs(void)
{
    char*   funcNames[] = {
        "print", "flush", "getchar", "ord", "chr",
        "size", "substring", "concat", "not", "exit"
    };

    myTable table = myEnvironment_BaseVarAndFunc();
    for (int i = 0; i < sizeof(funcNames) / sizeof(funcNames[0]); ++i)
    {
        void* result = MySymbol_Look(table, MySymbol_MakeSymbol(*funcNames));
        CU_ASSERT_NOT_EQUAL (result, NULL);
    }
}

void test_myEnvironmentBaseType_BeDefault_ContainsIntuitiveTypes(void)
{
    char* typeNames[] = { "int", "string" };

    myTable table = myEnvironment_BaseType();
    for (int i = 0; i < sizeof(typeNames) / sizeof(typeNames[0]); ++i)
    {
        void* result = MySymbol_Look(table, MySymbol_MakeSymbol(typeNames[i]));
        CU_ASSERT_NOT_EQUAL (result, NULL);
    }
}

int main (int argc, char* argv[])
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return -1;

    //	add tests
    CU_TestInfo tests[] = {
        { "", test_makeVarEntry_ByDefault_MakeOneValidVarEntry },
        { "", test_makeFuncEntry_ByDefault_MakeOneValidFuncEntry },
        { "", test_addNewVarOrFunc_ByDefault_AddOne },
        { "", test_addNewType_ByDefault_AddOne },
        { "", test_MyEnvironmentsetTypeFromName_ValidSet_ReturnTrueAndTheOneSetted },
        { "", test_myEnvironmentBaseVarAndFunc_BeDefault_ContainPredefinedFuncs },
        { "", test_myEnvironmentBaseType_BeDefault_ContainsIntuitiveTypes }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;
    
    if (!runTests(&suite))
        return -1;

    freeMemoryChain();
    return 0;
}