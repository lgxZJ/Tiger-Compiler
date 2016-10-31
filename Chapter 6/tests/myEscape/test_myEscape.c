#include "../testHelper.h"
#include "../testFixture.h"

#include "../../myEscape.h"
#include "../../myParser.h"
#include "../../mySemantic.h"
#include "../../typeMaker.h"
#include "../../myEnvironment.h"
#include "../../abstractSyntaxMaker.h"

#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////
//////////////////////      Fake functions          /////////////////////

static myTable g_typeEnv = NULL;
static myTable g_varandFuncEnv = NULL;

myTable MySemantic_getTypeEnvironment(void)
{
    if (g_typeEnv == NULL)
        g_typeEnv = MySymbol_MakeNewTable();
    return g_typeEnv;
}

myTable MySemantic_getVarAndFuncEnvironment(void)
{
    if (g_varandFuncEnv == NULL)
        g_varandFuncEnv = MySymbol_MakeNewTable();
    return g_varandFuncEnv;
}

static void FakeSemantic_addNewType(mySymbol varName, myType varType)
{
    MyEnvironment_addNewType(
        MySemantic_getTypeEnvironment(),
        varName,
        varType);
}

static void FakeSemantic_addNewVarOrFunc(mySymbol varName, myType varType)
{
     MyEnvironment_addNewVarOrFunc(
        MySemantic_getVarAndFuncEnvironment(), 
        varName,
        myEnvironment_makeVarEntry((Trans_myAccess)NULL, varType));
}

static void resetTestEnv(void)
{
    g_typeEnv = g_varandFuncEnv = NULL;
    Escape_setEscapeEnvironment(MySymbol_MakeNewTable());
}

//////////////////////         test helpers             //////////////////////

static void recordCommon_(mySymbol varName, int depthDeclared)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordTypeName");
    FakeSemantic_addNewVarOrFunc(varName,
        makeAndAddOneType_NoFieldRecord(recordTypeName));
    Escape_findEscape_VarDec(depthDeclared, 
        makeMyVarDec_ShortForm(makeMyShortFormVar(
            varName, makeOneExp_Record(recordTypeName))));
}

static myLValueExp makeSimpleVarOfRecordType(mySymbol varName, int depthDeclared)
{
    recordCommon_(varName, depthDeclared);
    return makeMyLValue(makeOnePos(), varName, NULL);
}

static myLValueExp makeRecordField(mySymbol varName, int depthDeclared)
{
    recordCommon_(varName, depthDeclared);
    return makeMyLValue(makeOnePos(), varName, 
        makeMyLValueAux(makeSymbol_Int(), NULL, NULL));
}

/////////

static void ArrayCommom_(mySymbol varName, int depthDeclared)
{
    mySymbol arrayTypeName = MySymbol_MakeSymbol("recordTypeName");
    FakeSemantic_addNewVarOrFunc(varName,
        makeOneArray_StringArrayArray());
    Escape_findEscape_VarDec(depthDeclared, 
        makeMyVarDec_ShortForm(makeMyShortFormVar(
            varName,
            makeMyExp_ArrayCreation(makeOnePos(),
                makeMyArrayCreationExp(arrayTypeName,
                    makeOneExp_Integer(), makeOneExp_String())))));
}

static myLValueExp makeSimpleVarOfArrayType(mySymbol varName, int depthDeclared)
{
    ArrayCommom_(varName, depthDeclared);
    return makeMyLValue(makeOnePos(), varName, NULL);
}

static myLValueExp makeArraySubscript(mySymbol varName, int depthDeclared)
{
    ArrayCommom_(varName, depthDeclared);
    return makeMyLValue(makeOnePos(), varName,
        makeMyLValueAux(NULL, makeOneExp_Integer(), NULL));
}

//////////////

static myVarDec makeVarDecOfRecordType_Short(mySymbol varName)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeAndAddOneType_NoFieldRecord(recordTypeName);
    myVarDec varDec = makeMyVarDec_ShortForm(makeMyShortFormVar(
        varName, makeOneExp_Record(recordTypeName)));
    FakeSemantic_addNewVarOrFunc(varName, recordType);

    return varDec;
}

static myVarDec makeVarDecOfRecordType_Long(mySymbol varName)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeAndAddOneType_NoFieldRecord(recordTypeName);
    myVarDec varDec = makeMyVarDec_LongForm(makeMyLongFormVar(
        varName,  recordTypeName, makeOneExp_Record(recordTypeName)));
    FakeSemantic_addNewVarOrFunc(varName, recordType);

    return varDec;
}

////////////

static myFuncDec makeFuncDec_Procecdure(mySymbol varOne, mySymbol varTwo)
{
    return makeMyFuncDec_Procedure(makeMyProcedureDec(
        MySymbol_MakeSymbol("procedureOne"),
        makeMyTyFieldList(makeMyTyField(varOne, makeSymbol_Int()),
            makeMyTyFieldList(makeMyTyField(varTwo, makeSymbol_String()), NULL)),
        makeOneExp_NoValue()));
}

static myFuncDec makeFuncDec_Function(mySymbol varOne, mySymbol varTwo)
{
    return makeMyFuncDec_Function(makeMyFunctionDec(
        MySymbol_MakeSymbol("procedureOne"),
        makeMyTyFieldList(makeMyTyField(varOne, makeSymbol_Int()),
            makeMyTyFieldList(makeMyTyField(varTwo, makeSymbol_String()), NULL)),
        makeSymbol_Int(),
        makeOneExp_Integer()));
}

///////////////////////        tests       ////////////////////////

void test_EscapeGetterAndSetter_ByDefault_GetWhatSetted(void)
{
    resetTestEnv();
    myTable newEnv = MySymbol_MakeNewTable();

    Escape_setEscapeEnvironment(newEnv);
    myTable envGot = Escape_getEscapeEnvironment();

    CU_ASSERT_EQUAL(envGot, newEnv);
}

///////////////////////////////////////////////////////////////////

//  forwards
void test_LValueExpSimpleVar_UnderDepth_WhetherEscape(
    int depthDeclared, int depthUsed, bool ifEscape);

////////////

void test_EscapeFindEscapeLValueExp_SimpleVarNotNested_NotEscape(void)
{
    int depthDeclared = 0;
    int depthUsed = depthDeclared;
    test_LValueExpSimpleVar_UnderDepth_WhetherEscape(
        depthDeclared, depthUsed, false);
}

void test_EscapeFindEscapeLValueExp_SimpleVarNested_Escape(void)
{
    int depthDeclared = 0;
    int depthUsed = 1;
    test_LValueExpSimpleVar_UnderDepth_WhetherEscape(
        depthDeclared, depthUsed, true);
}

void test_EscapeFindEscapeLValueExp_SimpleVarRecordType_Escape(void)
{
    resetTestEnv();
    int depthDeclared = 0;
    mySymbol varName = MySymbol_MakeSymbol("varName");
    myLValueExp lValueExp = makeSimpleVarOfRecordType(varName, depthDeclared);

    int depthUsed = 1;
    Escape_findEscape_LValueExp(depthUsed, lValueExp);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, true);
}



void test_EscapeFindEscapeLValueExp_SimpleVarArraySubscript_Escape(void)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varName");
    int depthDeclared = 1;
    myLValueExp lValueExp = makeSimpleVarOfArrayType(varName, depthDeclared);

    int depthUsed = 1;
    Escape_findEscape_LValueExp(depthUsed, lValueExp);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, true);
}

//  a parameterized test
void test_LValueExpSimpleVar_UnderDepth_WhetherEscape(
    int depthDeclared, int depthUsed, bool ifEscape)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varName");
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int()); 
    Escape_findEscape_VarDec(depthDeclared, 
        makeMyVarDec_ShortForm(makeMyShortFormVar(
            varName, makeOneExp_Integer())));
    myLValueExp lValueExp = makeMyLValue(makeOnePos(), varName, NULL);


    Escape_findEscape_LValueExp(depthUsed, lValueExp);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, ifEscape);

    //  reset environment
    Escape_setEscapeEnvironment(MySymbol_MakeNewTable());
}

/////////////////////////

//  forwards
void test_LValueExpRecordField_UnderDepth_WhetherEscape(
    int depthDeclared, int depthUsed, bool ifEscape);

////////////

void test_EscapeFindEscapeRecordField_ByDefault_AlwaysEscape(void)
{
    int depthDeclared = 0, depthUsed = 0;
    test_LValueExpRecordField_UnderDepth_WhetherEscape(
        depthDeclared, depthUsed, true);
    
    depthUsed = depthDeclared + 1;
    test_LValueExpRecordField_UnderDepth_WhetherEscape(
        depthDeclared, depthUsed, true);
}

//  a parameterized test
void test_LValueExpRecordField_UnderDepth_WhetherEscape(
    int depthDeclared, int depthUsed, bool ifEscape)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varName");
    myLValueExp lValueExp = makeRecordField(varName, depthDeclared);

    Escape_findEscape_LValueExp(depthUsed, lValueExp);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, ifEscape);
}

/////////////////

//  forwards
void test_LValueExpArraySubscript_UnderDepth_WhetherEscape(
    int depthDeclared, int depthUsed, bool ifEscape);

////////

void test_EscapeFindEscapeArraySubscript_ByDefault_AlwaysEscape(void)
{
    int depthDeclared = 0, depthUsed = 0;
    test_LValueExpArraySubscript_UnderDepth_WhetherEscape(
        depthDeclared, depthUsed, true);
    
    depthUsed = depthDeclared + 1;
    test_LValueExpArraySubscript_UnderDepth_WhetherEscape(
        depthDeclared, depthUsed, true);
}

//  a parameterized test
void test_LValueExpArraySubscript_UnderDepth_WhetherEscape(
    int depthDeclared, int depthUsed, bool ifEscape)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varName");
    myLValueExp lValueExp = makeArraySubscript(varName, depthDeclared);

    Escape_findEscape_LValueExp(depthUsed, lValueExp);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, ifEscape);
}

///////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeVarDec_ShortFormVarInt_NotEscape(void)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeMyVarDec_ShortForm(makeMyShortFormVar(
        varName, makeOneExp_Integer()));
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int());

    Escape_findEscape_VarDec(0, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, false);
}

void test_EscapeFindEscapeVarDec_LongFormVarInt_NotEscape(void)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeMyVarDec_LongForm(makeMyLongFormVar(
        varName, makeSymbol_Int(), makeOneExp_Integer()));
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int());

    Escape_findEscape_VarDec(0, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, false);
}


void test_EscapeFindEscapeVarDec_ShortFormVarRecord_Escape(void)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeVarDecOfRecordType_Short(varName);

    Escape_findEscape_VarDec(1, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, true);
}

void test_EscapeFindEscapeVarDec_LongFormVarRecord_Escape(void)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeVarDecOfRecordType_Long(varName);

    Escape_findEscape_VarDec(1, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, true);
}

///////////////////////////////

void test_EscapeFindEscapeFuncDec_ProcedureDec_FormalsAreEscaped(void)
{
    resetTestEnv();
    mySymbol varOne = MySymbol_MakeSymbol("varOne");
    mySymbol varTwo = MySymbol_MakeSymbol("varTwo");
    myFuncDec proceDec = makeFuncDec_Procecdure(varOne, varTwo);

    Escape_findEscape_FuncDec(1, proceDec);

    bool resultOne = Escape_isVarEscaped(varOne);
    bool resultTwo = Escape_isVarEscaped(varTwo);
    CU_ASSERT_EQUAL(resultOne, true);
    CU_ASSERT_EQUAL(resultTwo, true);
}

void test_EscapeFindEscapeFuncDec_FunctionDec_FormalsAreEscaped(void)
{
    resetTestEnv();
    mySymbol varOne = MySymbol_MakeSymbol("varOne");
    mySymbol varTwo = MySymbol_MakeSymbol("varTwo");
    myFuncDec proceDec = makeFuncDec_Function(varOne, varTwo);

    Escape_findEscape_FuncDec(1, proceDec);

    bool resultOne = Escape_isVarEscaped(varOne);
    bool resultTwo = Escape_isVarEscaped(varTwo);
    CU_ASSERT_EQUAL(resultOne, true);
    CU_ASSERT_EQUAL(resultTwo, true);
}

///////////////////////         main        /////////////////////

int main()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return EXIT_FAILURE;

    CU_TestInfo tests[] = {
        { "", test_EscapeGetterAndSetter_ByDefault_GetWhatSetted },

        { "", test_EscapeFindEscapeLValueExp_SimpleVarNotNested_NotEscape },
        { "", test_EscapeFindEscapeLValueExp_SimpleVarNested_Escape },
        { "", test_EscapeFindEscapeLValueExp_SimpleVarRecordType_Escape },
        { "", test_EscapeFindEscapeLValueExp_SimpleVarArraySubscript_Escape },
        { "", test_EscapeFindEscapeRecordField_ByDefault_AlwaysEscape },
        { "", test_EscapeFindEscapeArraySubscript_ByDefault_AlwaysEscape },

        { "", test_EscapeFindEscapeVarDec_ShortFormVarInt_NotEscape },
        { "", test_EscapeFindEscapeVarDec_LongFormVarInt_NotEscape },
        { "", test_EscapeFindEscapeVarDec_ShortFormVarRecord_Escape },
        { "", test_EscapeFindEscapeVarDec_ShortFormVarRecord_Escape },

        { "", test_EscapeFindEscapeFuncDec_ProcedureDec_FormalsAreEscaped },
        { "", test_EscapeFindEscapeFuncDec_FunctionDec_FormalsAreEscaped }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}