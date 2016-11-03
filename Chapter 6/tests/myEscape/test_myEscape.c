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

static bool g_fakeEscapeOne = false;
static bool g_fakeEscapeTwo = false;
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
    g_fakeEscapeOne = false;
    g_fakeEscapeTwo = false;
    g_typeEnv = g_varandFuncEnv = NULL;
    Escape_setEscapeEnvironment(MySymbol_MakeNewTable());
}


///////////////////////        private forwards            ////////////////////////

typedef struct EscapeEntry_* EscapeEntry;

void        Escape_addVarEntry      (mySymbol varSymbol, EscapeEntry entry);
EscapeEntry makeEscapeEntry  (int depth, bool* escapePtr);

void Escape_findEscape_LValueExp(
    int depth, myLValueExp lValueExp);
void Escape_findEscape_FunctionCallExp(
    int depth, myFunctionCallExp functionCallExp);
void Escape_findEscape_NilExp(
    int depth, myNilExp nilExp);
void Escape_findEscape_IntegerLiteralExp(
    int depth, myIntegerLiteralExp integerLiteralExp);
void Escape_findEscape_StringLiteralExp(
    int depth, myStringLiteralExp stringLiteralExp);
void Escape_findEscape_ParenthesesExp(
    int depth, myParenthesesExp parenthesesExp);
void Escape_findEscape_NoValueExp(
    int depth, myNoValueExp noValueExp);
void Escape_findEscape_VarDec(
    int depth, myVarDec varDec);
void Escape_findEscape_FuncDec(
    int depth, myFuncDec funcDec);
void Escape_findEscape_ArrayCreationExp(
    int depth, myArrayCreationExp arrayCreationExp);
void Escape_findEscape_RecordCreationExp(
    int depth, myRecordCreationExp recordCreationExp);
void Escape_findEscape_ArithmeticExp(
    int depth, myArithmeticExp arithmeticExp);
void Escape_findEscape_SequencingExp(
    int depth, mySequencingExp sequencingExp);
void Escape_findEscape_ForExp(
    int depth, myForExp forExp);
void Escape_findEscape_IfThenElseExp(
    int depth, myIfThenElseExp ifThenElseExp);
void Escape_findEscape_IfThenExp(
    int depth, myIfThenExp ifThenExp);
void Escape_findEscape_ComparisonExp(
    int depth, myComparisonExp comparisonExp);
void Escape_findEscape_BooleanOperateExp(
    int depth, myBooleanOperateExp booleanOperateExp);
void Escape_findEscape_AssignmentExp(
    int depth, myAssignmentExp assignmentExp);
void Escape_findEscape_LetExp(
    int depth, myLetExp letExp);
void Escape_findEscape_WhileExp(
    int depth, myWhileExp whileExp);
void Escape_findEscape_NegationExp(
    int depth, myNegationExp negationExp);


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

static myExp makeExpBodyWithVar(mySymbol varName, int funcDepth)
{
    static bool isFirst = false;
    isFirst = !isFirst;

    Escape_addVarEntry(varName, makeEscapeEntry(funcDepth,
        isFirst ? &g_fakeEscapeOne : &g_fakeEscapeTwo));
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int());
    return makeMyExp_LValue(makeOnePos(),
        makeMyLValue(makeOnePos(), varName, NULL));
}

static myFuncDec makeFuncDecWithIntBody(mySymbol varOne, mySymbol varTwo, myExp body)
{
    return makeMyFuncDec_Function(makeMyFunctionDec(
        MySymbol_MakeSymbol("procedureOne"),
        makeMyTyFieldList(makeMyTyField(varOne, makeSymbol_Int()),
            makeMyTyFieldList(makeMyTyField(varTwo, makeSymbol_String()), NULL)),
        makeSymbol_Int(),
        body));
}

////////////

static myFunctionCallExp makeFuncCallExp(mySymbol varName, int depthDeclared)
{
    Escape_addVarEntry(varName, makeEscapeEntry(depthDeclared, &g_fakeEscapeOne));
    return makeMyFunctionCallExp_Param(
        makeMyParamFunctionCallExp(
            MySymbol_MakeSymbol("func"),
            makeMyExpList(
                makeMyExp_LValue(
                    makeOnePos(), makeMyLValue(makeOnePos(), varName, NULL)),
            NULL)));
}

////////////

static myRecordCreationExp makeRecordCreationWithOneField(
    mySymbol fieldName, int depth)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordTypeName");
    mySymbol fieldNameInRecord = MySymbol_MakeSymbol("fieldName");
    myType recordType = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(fieldNameInRecord, makeType_Int()), NULL));
    return makeMyRecordCreationExp_Field(
        makeMyFieldRecordCreationExp(recordTypeName,
            makeMyRecordFieldList(makeMyRecordField(
                fieldNameInRecord, makeExpBodyWithVar(fieldName, depth)), NULL)));    
}

///////////

static myIfThenElseExp makeIfthenElseExpWithOneVar(mySymbol varName, int depth)
{
    return makeMyIfThenElseExp(
        makeExpBodyWithVar(varName, depth),
        makeExpBodyWithVar(varName, depth),
        makeExpBodyWithVar(varName, depth));
}

////////////

static myIfThenExp makeIfthenExpWithOneVar(mySymbol varName, int varDepth)
{
    return makeMyIfThenExp(
        makeExpBodyWithVar(varName, varDepth),
        makeOneExp_Integer());
}

////////////

static myComparisonExp makeComparisonExpWithOneVar(mySymbol varName, int varDepth)
{
    return makeMyComparisonExp_Equal(
        makeExpBodyWithVar(varName, varDepth),
        makeOneExp_Integer());
}

////////////

static myBooleanOperateExp makeBooleanOperateWithOneVar(mySymbol varName, int varDepth)
{
    return makeMyBooleanOperateExp_And(
        makeExpBodyWithVar(varName, varDepth),
        makeOneExp_Integer());
}

/////////////

static myAssignmentExp makeAssignmentWithOneVar(mySymbol varName, int varDepth)
{
    return makeMyAssignmentExp(
        makeMyLValue(makeOnePos(),varName, NULL),
        makeExpBodyWithVar(varName, varDepth));
}

///////////

static myLValueExp makeLValueWithOneVar(mySymbol varName, int varDepth)
{
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int()); 
    Escape_findEscape_VarDec(varDepth, 
        makeMyVarDec_ShortForm(makeMyShortFormVar(
            varName, makeOneExp_Integer())));
    return makeMyLValue(makeOnePos(), varName, NULL);

}

///////////

static myVarDec makeVarDecOfIntType_Short(mySymbol varName)
{
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int());
    return makeMyVarDec_ShortForm(makeMyShortFormVar(
        varName, makeOneExp_Integer()));
}

static myVarDec makeVarDecOfIntType_Long(mySymbol varName)
{
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int());
    return makeMyVarDec_LongForm(makeMyLongFormVar(
        varName, makeSymbol_Int(), makeOneExp_Integer()));
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
    myLValueExp lValueExp = makeLValueWithOneVar(varName, depthDeclared);


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

////////////////////////////////////////////////////////////////////

//  forwards
void test_FunctionCallExp_UnderDepth_WhetherRealParamEscape(
    int depthDeclared, int depthUsed, bool ifEscape);

//////////

void test_EscapeFindEscapeFunctionCallExp_ByDefault_TreatPartsAsSingleExps(void)
{
    int depthDeclared = 1;
    int depthUsed = depthDeclared;
    test_FunctionCallExp_UnderDepth_WhetherRealParamEscape(
        depthDeclared, depthUsed, false);
    depthUsed = depthDeclared + 1;
    test_FunctionCallExp_UnderDepth_WhetherRealParamEscape(
        depthDeclared, depthUsed, true);
}

//  a parameterized test
void test_FunctionCallExp_UnderDepth_WhetherRealParamEscape(
    int depthDeclared, int depthUsed, bool ifEscape)
{
    resetTestEnv();
    
    mySymbol varName = MySymbol_MakeSymbol("varName");
    FakeSemantic_addNewVarOrFunc(varName, makeType_Int());
    myFunctionCallExp functionCall =
        makeFuncCallExp(varName, depthDeclared);
    
    Escape_findEscape_FunctionCallExp(depthUsed, functionCall);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, ifEscape);
}

///////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeVarDec_ShortFormVarInt_NotEscape(void)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeVarDecOfIntType_Short(varName);

    Escape_findEscape_VarDec(0, varDec);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, false);
}

void test_EscapeFindEscapeVarDec_LongFormVarInt_NotEscape(void)
{
    resetTestEnv();
    mySymbol varName = MySymbol_MakeSymbol("varname");
    myVarDec varDec = makeVarDecOfIntType_Long(varName);

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
    /*resetTestEnv();
    mySymbol varOne = MySymbol_MakeSymbol("varOne");
    mySymbol varTwo = MySymbol_MakeSymbol("varTwo");
    myFuncDec proceDec = makeFuncDec_Procecdure(varOne, varTwo);

    Escape_findEscape_FuncDec(1, proceDec);

    bool resultOne = Escape_isVarEscaped(varOne);
    bool resultTwo = Escape_isVarEscaped(varTwo);
    CU_ASSERT_EQUAL(resultOne, true);
    CU_ASSERT_EQUAL(resultTwo, true);*/
}

void test_EscapeFindEscapeFuncDec_FunctionDec_FormalsAreEscaped(void)
{
    /*resetTestEnv();
    mySymbol varOne = MySymbol_MakeSymbol("varOne");
    mySymbol varTwo = MySymbol_MakeSymbol("varTwo");
    myFuncDec proceDec = makeFuncDec_Function(varOne, varTwo);

    Escape_findEscape_FuncDec(1, proceDec);

    bool resultOne = Escape_isVarEscaped(varOne);
    bool resultTwo = Escape_isVarEscaped(varTwo);
    CU_ASSERT_EQUAL(resultOne, true);
    CU_ASSERT_EQUAL(resultTwo, true);*/
}

void test_EscapeFindEscapeFuncDec_FuncDec_TreatBodyAsSingleExp(void)
{
    resetTestEnv();
    int functionDepth = 2;
    mySymbol varNameInFunc = MySymbol_MakeSymbol("varName");
    myFuncDec funcDec = makeFuncDecWithIntBody(
        MySymbol_MakeSymbol("varOne"),
        MySymbol_MakeSymbol("varTwo"),
        makeExpBodyWithVar(varNameInFunc, functionDepth));

    int outerDepth = 1;
    Escape_findEscape_FuncDec(functionDepth, funcDec);

    bool result = Escape_isVarEscaped(varNameInFunc);
    CU_ASSERT_EQUAL(result, true);
}

/////////////////////////////////////////////////////////////////

static myLetExp makeLetWithOneVar(mySymbol varName, int varDepth)
{
    return makeMyLetExp(NULL, makeMyExpList(
        makeExpBodyWithVar(varName, varDepth),
        NULL));
}

void test_EscapeFindEscapeLetExp_ByDefault_TreatExpsAsSingleExps(void)
{
    resetTestEnv();
    int varDepth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    mySymbol varNameTwo = MySymbol_MakeSymbol("var2");
    myLetExp expNotNested = makeLetWithOneVar(varNameOne, varDepth);
    myLetExp expNested = makeLetWithOneVar(varNameTwo, varDepth);
    
    Escape_findEscape_LetExp(varDepth, expNotNested);
    Escape_findEscape_LetExp(varDepth + 1, expNested);

    bool resultNotNested = Escape_isVarEscaped(varNameOne);
    bool resultNested = Escape_isVarEscaped(varNameTwo);
    CU_ASSERT_EQUAL(resultNotNested, false);
    CU_ASSERT_EQUAL(resultNested, true);
}

/////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeArrayCreation_ByDefault_TreatExpsAsSingleExp(void)
{
    resetTestEnv();
    int depth = 1;
    mySymbol arrayTypeName = MySymbol_MakeSymbol("arrayTypeName");
    mySymbol varNameInLengthExp = MySymbol_MakeSymbol("varNameLength");
    mySymbol varNameInInitialExp = MySymbol_MakeSymbol("varNameInitial");
    myArrayCreationExp exp = makeMyArrayCreationExp(
        arrayTypeName,
        makeExpBodyWithVar(varNameInLengthExp, depth),
        makeExpBodyWithVar(varNameInInitialExp, depth - 1));

    Escape_findEscape_ArrayCreationExp(depth, exp);

    bool resultInitial = Escape_isVarEscaped(varNameInInitialExp);
    bool resultLength = Escape_isVarEscaped(varNameInLengthExp);
    CU_ASSERT_EQUAL(resultInitial, true);
    CU_ASSERT_EQUAL(resultLength, false);
}

/////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeRecordCreation_ByDefault_TreatExpsAsSingleExp(void)
{
    resetTestEnv();
    int depth = 1;
    mySymbol fieldNameOne = MySymbol_MakeSymbol("varNameOne");
    mySymbol fieldNameTwo = MySymbol_MakeSymbol("varNameTwo");
    myRecordCreationExp fieldVarNotNestedExp = makeRecordCreationWithOneField(
        fieldNameOne, depth);
    myRecordCreationExp fieldVarNestedExp = makeRecordCreationWithOneField(
        fieldNameTwo, depth);
    
    Escape_findEscape_RecordCreationExp(depth, fieldVarNotNestedExp);
    Escape_findEscape_RecordCreationExp(depth + 1, fieldVarNestedExp);

    //bool resultNotNested = Escape_isVarEscaped(fieldNameOne);
    bool resultNested = Escape_isVarEscaped(fieldNameTwo);
    //CU_ASSERT_EQUAL(resultNotNested, false);
    CU_ASSERT_EQUAL(resultNested, true);
}

/////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeArithmeticExp_ByDefault_TreatPartsAsSingleExps(void)
{
    resetTestEnv();
    int depth = 0;
    mySymbol varName = MySymbol_MakeSymbol("varName");
    Escape_addVarEntry(varName, makeEscapeEntry(depth, &g_fakeEscapeOne));
    myArithmeticExp exp = makeMyArithmeticExp_Divide(
        makeExpBodyWithVar(varName, depth), makeExpBodyWithVar(varName, depth));

     Escape_findEscape_ArithmeticExp(depth, exp);

     bool result = Escape_isVarEscaped(varName);
     CU_ASSERT_EQUAL(result, false);
}

///////////////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeSequencingExp_ByDefault_TreatPartsAsSingleExps(void)
{
    resetTestEnv();
    int depth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("varNameOne");
    mySymbol varNameTwo = MySymbol_MakeSymbol("varNameTwo");
    Escape_addVarEntry(varNameOne, makeEscapeEntry(depth, &g_fakeEscapeOne));
    Escape_addVarEntry(varNameTwo, makeEscapeEntry(depth, &g_fakeEscapeOne));
    mySequencingExp expOne = makeMySequencingExp(
        makeExpBodyWithVar(varNameOne, depth),
        makeExpBodyWithVar(varNameOne, depth),
        makeMyExpList(makeExpBodyWithVar(varNameOne, depth), NULL));
    mySequencingExp expTwo = makeMySequencingExp(
        makeExpBodyWithVar(varNameTwo, depth),
        makeExpBodyWithVar(varNameTwo, depth),
        makeMyExpList(makeExpBodyWithVar(varNameTwo,depth), NULL));

     Escape_findEscape_SequencingExp(depth, expOne);
     Escape_findEscape_SequencingExp(depth + 1, expTwo);

     bool resultOne = Escape_isVarEscaped(varNameOne);
     bool resultTwo = Escape_isVarEscaped(varNameTwo);
     CU_ASSERT_EQUAL(resultOne, false);
     CU_ASSERT_EQUAL(resultTwo, true);
}

/////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeForExp_ByDefault_TreatExpsAsSingleExps(void)
{
    resetTestEnv();
    int depth = 0;
    mySymbol varName = MySymbol_MakeSymbol("varName");
    mySymbol loopVarName = MySymbol_MakeSymbol("loop-var");
    myForExp forExp = makeMyForExp(
        loopVarName, makeOneExp_Integer(), makeOneExp_Integer(),
        makeExpBodyWithVar(varName, depth));

    Escape_findEscape_ForExp(depth, forExp);

    bool result = Escape_isVarEscaped(varName);
    CU_ASSERT_EQUAL(result, false);
}

////////////////////////////////////////////////////////////////////



void test_EscapeFindEscapeIfThenElseExp_ByDefault_TreatClausesAsSingleExps(void)
{
    resetTestEnv();
    int depth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    mySymbol varNameTwo = MySymbol_MakeSymbol("var2");
    myIfThenElseExp expVarNotNested = 
        makeIfthenElseExpWithOneVar(varNameOne, depth);
    myIfThenElseExp expVarNested =
        makeIfthenElseExpWithOneVar(varNameTwo, depth);

    Escape_findEscape_IfThenElseExp(depth, expVarNotNested);
    Escape_findEscape_IfThenElseExp(depth + 1, expVarNested);

    bool resultVarNotNested = Escape_isVarEscaped(varNameOne);
    bool resultVarNested = Escape_isVarEscaped(varNameTwo);
    CU_ASSERT_EQUAL(resultVarNotNested, false);
    CU_ASSERT_EQUAL(resultVarNested, true);
}

///////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeIfThenExp_ByDefault_TreatClausesAsSingleExps(void)
{
    resetTestEnv();
    int varDepth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    mySymbol varNameTwo = MySymbol_MakeSymbol("var2");
    myIfThenExp expVarNotNested = 
        makeIfthenExpWithOneVar(varNameOne, varDepth);
    myIfThenExp expVarNested =
        makeIfthenExpWithOneVar(varNameTwo, varDepth);

    Escape_findEscape_IfThenExp(varDepth, expVarNotNested);
    Escape_findEscape_IfThenExp(varDepth + 1, expVarNested);

    bool resultVarNotNested = Escape_isVarEscaped(varNameOne);
    bool resultVarNested = Escape_isVarEscaped(varNameTwo);
    CU_ASSERT_EQUAL(resultVarNotNested, false);
    CU_ASSERT_EQUAL(resultVarNested, true);
}

////////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeComparisonExp_ByDefault_TreatOperandsAsSingleExps(void)
{
    resetTestEnv();
    int varDepth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    mySymbol varNameTwo = MySymbol_MakeSymbol("var2");
    myComparisonExp expNotNested = 
        makeComparisonExpWithOneVar(varNameOne, varDepth);
    myComparisonExp expNested =
        makeComparisonExpWithOneVar(varNameTwo, varDepth);

    Escape_findEscape_ComparisonExp(varDepth, expNotNested);
    Escape_findEscape_ComparisonExp(varDepth + 1, expNested);

    bool resultNotNested = Escape_isVarEscaped(varNameOne);
    bool resultNested = Escape_isVarEscaped(varNameTwo);
    CU_ASSERT_EQUAL(resultNotNested, false);
    CU_ASSERT_EQUAL(resultNested, true);
}

///////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeBooleanOperateExp_ByDefault_TreatOperandsAsSingleExps(void)
{
    resetTestEnv();
    int varDepth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    mySymbol varNameTwo = MySymbol_MakeSymbol("var2");
    myBooleanOperateExp expNotNested =
        makeBooleanOperateWithOneVar(varNameOne, varDepth);
    myBooleanOperateExp expNested =
        makeBooleanOperateWithOneVar(varNameTwo, varDepth);

    Escape_findEscape_BooleanOperateExp(varDepth, expNotNested);
    Escape_findEscape_BooleanOperateExp(varDepth + 1, expNested);

    bool resultNotNested = Escape_isVarEscaped(varNameOne);
    bool resultNested = Escape_isVarEscaped(varNameTwo);
    CU_ASSERT_EQUAL(resultNotNested, false);
    CU_ASSERT_EQUAL(resultNested, true);
}

//////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeAssignmentExp_ByDefault_TreatExpsAsSingleExps(void)
{
    resetTestEnv();
    int varDepth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    mySymbol varNameTwo = MySymbol_MakeSymbol("var2");
    myAssignmentExp expNotNested =
        makeAssignmentWithOneVar(varNameOne, varDepth);
    myAssignmentExp expNested =
        makeAssignmentWithOneVar(varNameTwo, varDepth);
    
    Escape_findEscape_AssignmentExp(varDepth, expNotNested);
    Escape_findEscape_AssignmentExp(varDepth + 1, expNested);

    bool resultNotNested = Escape_isVarEscaped(varNameOne);
    bool resultNested = Escape_isVarEscaped(varNameTwo);
    CU_ASSERT_EQUAL(resultNotNested, false);
    CU_ASSERT_EQUAL(resultNested, true);
}

///////////////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeWhileExp_ByDefault_TreatExpsAsSingleExps(void)
{
    resetTestEnv();
    int varDepth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    myWhileExp exp = makeMyWhileExp(
        makeExpBodyWithVar(varNameOne, varDepth),
        makeExpBodyWithVar(varNameOne, varDepth));

    Escape_findEscape_WhileExp(varDepth, exp);

    bool result = Escape_isVarEscaped(varNameOne);
    CU_ASSERT_EQUAL(result, false);
}

/////////////////////////////////////////////////////////////////////

void test_EscapeFindEscapeNegationExp_ByDefault_TreatExpsAsSingleExps(void)
{
    resetTestEnv();
    int varDepth = 0;
    mySymbol varNameOne = MySymbol_MakeSymbol("var1");
    myNegationExp exp =
        makeMyNegationExp(makeExpBodyWithVar(varNameOne, varDepth));

    Escape_findEscape_NegationExp(varDepth, exp);

    bool result = Escape_isVarEscaped(varNameOne);
    CU_ASSERT_EQUAL(result, false);
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

        { "", test_EscapeFindEscapeFunctionCallExp_ByDefault_TreatPartsAsSingleExps },        
        { "", test_EscapeFindEscapeArrayCreation_ByDefault_TreatExpsAsSingleExp },
        { "", test_EscapeFindEscapeRecordCreation_ByDefault_TreatExpsAsSingleExp },
        { "", test_EscapeFindEscapeArithmeticExp_ByDefault_TreatPartsAsSingleExps },
        { "", test_EscapeFindEscapeSequencingExp_ByDefault_TreatPartsAsSingleExps },
        { "", test_EscapeFindEscapeComparisonExp_ByDefault_TreatOperandsAsSingleExps },
        { "", test_EscapeFindEscapeBooleanOperateExp_ByDefault_TreatOperandsAsSingleExps },
        { "", test_EscapeFindEscapeAssignmentExp_ByDefault_TreatExpsAsSingleExps },
        { "", test_EscapeFindEscapeForExp_ByDefault_TreatExpsAsSingleExps },
        { "", test_EscapeFindEscapeLetExp_ByDefault_TreatExpsAsSingleExps },
        { "", test_EscapeFindEscapeWhileExp_ByDefault_TreatExpsAsSingleExps },
        { "", test_EscapeFindEscapeIfThenElseExp_ByDefault_TreatClausesAsSingleExps },
        { "", test_EscapeFindEscapeIfThenExp_ByDefault_TreatClausesAsSingleExps },
        { "", test_EscapeFindEscapeNegationExp_ByDefault_TreatExpsAsSingleExps },

        { "", test_EscapeFindEscapeVarDec_ShortFormVarInt_NotEscape },
        { "", test_EscapeFindEscapeVarDec_LongFormVarInt_NotEscape },
        { "", test_EscapeFindEscapeVarDec_ShortFormVarRecord_Escape },
        { "", test_EscapeFindEscapeVarDec_ShortFormVarRecord_Escape },
        { "", test_EscapeFindEscapeFuncDec_ProcedureDec_FormalsAreEscaped },
        { "", test_EscapeFindEscapeFuncDec_FunctionDec_FormalsAreEscaped },
        { "", test_EscapeFindEscapeFuncDec_FuncDec_TreatBodyAsSingleExp }

    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return EXIT_FAILURE;

    if (!runTests(&suite))  return EXIT_FAILURE;
    freeMemoryChain();
    return EXIT_SUCCESS;
}