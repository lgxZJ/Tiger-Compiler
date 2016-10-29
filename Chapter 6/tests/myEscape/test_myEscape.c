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
//////////////////////      mock functions          /////////////////////

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

static void MockSemantic_addNewType(mySymbol varName, myType varType)
{
    MyEnvironment_addNewType(
        MySemantic_getTypeEnvironment(),
        varName,
        varType);
}

static void MockSemantic_addNewVarOrFunc(mySymbol varName, myType varType)
{
     MyEnvironment_addNewVarOrFunc(
        MySemantic_getVarAndFuncEnvironment(), 
        varName,
        myEnvironment_makeVarEntry((Trans_myAccess)NULL, varType));
}

//////////////////////         forwrads             //////////////////////

void Escape_findEscape_VarDec(int depth, myVarDec varDec);
void Escape_findEscape_FuncDec(int depth, myFuncDec funcDec);



///////////////////////        tests       ////////////////////////

void test_EscapeGetterAndSetter_ByDefault_GetWhatSetted(void)
{
    myTable newEnv = MySymbol_MakeNewTable();

    Escape_setEscapeEnvironment(newEnv);
    myTable envGot = Escape_getEscapeEnvironment();

    CU_ASSERT_EQUAL(envGot, newEnv);
}

///////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeLValueExp_NotNested_NotEscape(void)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeVarDec_ShortFormVarInt_NotEscape(void)
{
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeMyVarDec_ShortForm(makeMyShortFormVar(
        varName, makeOneExp_Integer()));
    MockSemantic_addNewVarOrFunc(varName, makeType_Int());

    Escape_findEscape_VarDec(0, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, false);
}

void test_EscapeFindEscapeVarDec_LongFormVarInt_NotEscape(void)
{
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeMyVarDec_LongForm(makeMyLongFormVar(
        varName, makeSymbol_Int(), makeOneExp_Integer()));
    MockSemantic_addNewVarOrFunc(varName, makeType_Int());

    Escape_findEscape_VarDec(0, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, false);
}

void test_EscapeFindEscapeVarDec_ShortFormVarRecord_Escape(void)
{
    mySymbol varName = MySymbol_MakeSymbol("varname");
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeAndAddOneType_NoFieldRecord(recordTypeName);
    myVarDec varDec = makeMyVarDec_ShortForm(makeMyShortFormVar(
        varName, makeOneExp_Record(recordTypeName)));
    MockSemantic_addNewVarOrFunc(varName, recordType);

    Escape_findEscape_VarDec(1, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, true);
}

void test_EscapeFindEscapeVarDec_LongFormVarRecord_Escape(void)
{
    mySymbol varName = MySymbol_MakeSymbol("varname");
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeAndAddOneType_NoFieldRecord(recordTypeName);
    myVarDec varDec = makeMyVarDec_LongForm(makeMyLongFormVar(
        varName,  recordTypeName, makeOneExp_Record(recordTypeName)));
    MockSemantic_addNewVarOrFunc(varName, recordType);

    Escape_findEscape_VarDec(1, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, true);
}

///////////////////////////////

void test_EscapeFindEscapeFuncDec_ProcedureDec_FormalsAreEscaped(void)
{
    mySymbol varOne = MySymbol_MakeSymbol("varOne");
    mySymbol varTwo = MySymbol_MakeSymbol("varTwo");
    myFuncDec proceDec = makeMyFuncDec_Procedure(makeMyProcedureDec(
        MySymbol_MakeSymbol("procedureOne"),
        makeMyTyFieldList(makeMyTyField(varOne, makeSymbol_Int()),
            makeMyTyFieldList(makeMyTyField(varTwo, makeSymbol_String()), NULL)),
        makeOneExp_NoValue()));

    Escape_findEscape_FuncDec(1, proceDec);

    bool resultOne = Escape_isVarEscaped(varOne);
    bool resultTwo = Escape_isVarEscaped(varTwo);
    CU_ASSERT_EQUAL(resultOne, true);
    CU_ASSERT_EQUAL(resultTwo, true);
}

void test_EscapeFindEscapeFuncDec_FunctionDec_FormalsAreEscaped(void)
{
    mySymbol varOne = MySymbol_MakeSymbol("varOne");
    mySymbol varTwo = MySymbol_MakeSymbol("varTwo");
    myFuncDec proceDec = makeMyFuncDec_Function(makeMyFunctionDec(
        MySymbol_MakeSymbol("procedureOne"),
        makeMyTyFieldList(makeMyTyField(varOne, makeSymbol_Int()),
            makeMyTyFieldList(makeMyTyField(varTwo, makeSymbol_String()), NULL)),
        makeSymbol_Int(),
        makeOneExp_Integer()));

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