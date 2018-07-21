#include "../testHelper.h"
#include "../testFixture.h"

#include "internalForwards.h"

#include "../../myParser.h"
#include "../../mySemantic.h"
#include "../../myEnvironment.h"
#include "../../typeMaker.h"
#include "../../typeChecker.h"
#include "../../abstractSyntaxMaker.h"
#include "../../breakChecker.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//  todo: change name *NULL to *SemanticError
///////////////////////////////////////////////////////////////////////

//  NOTE: after recursive version finished, these functions actually
//  not exists, but i don't want to lose these test efforts, so i create
//  some dalegates to called the recursive versions.

bool MySemantic_Dec_Type_Named(
    mySymbol newTypeName, mySymbol existedTypeName)
{
    myTypeDec dec =
        makeMyTyDec(newTypeName, makeMyTy_Named(makeOnePos(), existedTypeName));
    return MySemantic_Dec_Type_OnePass(dec) &&
            MySemantic_Dec_Type_Named_TwoPass(newTypeName, existedTypeName);
}

bool MySemantic_Dec_Type_Record(
    mySymbol newTypeName, myTyFieldList fields)
{
    myTypeDec dec =
        makeMyTyDec(newTypeName, makeMyTy_Record(makeOnePos(), fields));
    return MySemantic_Dec_Type_OnePass(dec) &&
            MySemantic_Dec_Type_Record_TwoPass(newTypeName, fields);
}

bool MySemantic_Dec_Type_Array(
    mySymbol newTypeName, mySymbol elementTypeName)
{
    myTypeDec dec =
        makeMyTyDec(newTypeName, makeMyTy_Array(makeOnePos(), elementTypeName));
    return MySemantic_Dec_Type_OnePass(dec) &&
            MySemantic_Dec_Type_Array_TwoPass(newTypeName, elementTypeName);
}

bool MySemantic_Dec_Func_Procedure(
    myProcedureDec procedureDec)
{
    return MySemantic_Dec_Func_Procedure_OnePass(procedureDec) &&
        MySemantic_Dec_Func_Procedure_TwoPass(procedureDec);
}

bool MySemantic_Dec_Func_Function(
    myFunctionDec functionDec)
{
    return MySemantic_Dec_Func_Function_OnePass(functionDec) &&
        MySemantic_Dec_Func_Function_TwoPass(functionDec);
}

////////////////////////////////////////////////////////////////////////
//                  forwards

void MySemantic_enterNewLevel(Trans_myLevel newLevel);
void MySemantic_leaveNewLevel(void);

///////////////////////////////////////////////////////////////////////

void test_GetterAndSetter_GetWhatSetted(void)
{
    myTable typeTableSetted = (myTable)12;
    myTable varAndFuncTableSetted = (myTable)34;

    MySemantic_setTypeEnvironment(typeTableSetted);
    MySemantic_setVarAndFuncEnvironment(varAndFuncTableSetted);

    myTable typeTableGot = MySemantic_getTypeEnvironment();
    myTable varAndFuncTableGot = MySemantic_getVarAndFuncEnvironment();
    CU_ASSERT_EQUAL(typeTableSetted, typeTableGot);
    CU_ASSERT_EQUAL(varAndFuncTableSetted, varAndFuncTableGot);
}

//////////////////////////////////////////////////////////////////////

void test_typeContainsLValueAux_CheckRecord_ReturnTypeOfOneField(void)
{
  /* myTranslate.c 
   *
    mySymbol symbol1 = MySymbol_MakeSymbol("field1");
    mySymbol symbol2 = MySymbol_MakeSymbol("field2");
    mySymbol symbolRecord = MySymbol_MakeSymbol("recordd");

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(symbol1, makeType_Int()),
            makeType_TypeFieldList(
                makeType_TypeField(symbol2, makeType_String()),
            NULL))));
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolRecord, entry);

    myLValueAux aux1 = makeMyLValueAux(symbol1, NULL, NULL);
    IR_myExp addressResult;
    myType typeReturn1 = typeContainsLValueAux(
        MyEnvironment_getVarType(entry), aux1, &addressResult);
    myLValueAux aux2 = makeMyLValueAux(symbol2, NULL, NULL);

    myType typeReturn2 = typeContainsLValueAux(
        MyEnvironment_getVarType(entry), aux2, &addressResult);
    
    CU_ASSERT(typeReturn1->kind == TypeInt);
    CU_ASSERT(typeReturn2->kind == TypeString);
  */
}

void test_typeContainsLValueAux_CheckRecord_ReturnTypeOfNestedField(void)
{
  /* myTranslate.c
   *
    mySymbol symbol1 = MySymbol_MakeSymbol("field1");
    mySymbol symbol2 = MySymbol_MakeSymbol("field2");
    mySymbol symbolRecord = MySymbol_MakeSymbol("recordd");

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    myType nestedRecordType = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol2, makeType_String()), NULL));
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol1, nestedRecordType), NULL)));
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolRecord, entry);

    myLValueAux aux = makeMyLValueAux(symbol1, NULL,
        makeMyLValueAux(symbol2, NULL, NULL));

    IR_myExp addressResult;
    myType typeReturn = typeContainsLValueAux(
        MyEnvironment_getVarType(entry), aux, &addressResult);
    
    CU_ASSERT(typeReturn->kind == TypeString);
  */
}

void test_typeContainsLValueAux_CheckArray_ReturnTypeOfOneField(void)
{
  /* myTranslate.c
   *
    mySymbol symbol = MySymbol_MakeSymbol("field1");
    mySymbol symbolArray = MySymbol_MakeSymbol("recordd");

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Array(makeType_Int()));
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolArray, entry);

    myLValueAux aux = makeMyLValueAux(NULL, 
        makeOneExp_Integer(), NULL);

    IR_myExp addressResult;
    myType typeReturn = typeContainsLValueAux(
        MyEnvironment_getVarType(entry), aux, &addressResult);
    
    CU_ASSERT(typeReturn->kind == TypeInt);
  */
}

void test_typeContainsLValueAux_CheckArray_ReturnTypeOfNestedField(void)
{
  /* myTranslate.c
   *
    mySymbol symbolArray = MySymbol_MakeSymbol("recordd");

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myType arrayType = makeType_Array(makeType_String());
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Array(arrayType));
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolArray, entry);

    myExp exp = makeOneExp_Integer();
    myLValueAux aux = makeMyLValueAux(NULL, 
        exp, makeMyLValueAux(NULL, exp, NULL));

    IR_myExp addressResult;
    myType typeReturn = typeContainsLValueAux(
        MyEnvironment_getVarType(entry), aux, &addressResult);
    
    CU_ASSERT(typeReturn->kind == TypeString);
  */
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticLValueExpSimpleVar_VarNotDeclared_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    myLValueExp simpleVarExp = makeMyLValue(
        makeOnePos(), makeOneSymbol(), NULL);

    myTranslationAndType result = MySemantic_LValueExp_SimpleVar(
        simpleVarExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpSimpleVar_LegalExp_ReturnTypeOfSimpleVar(void)
{
    /*
     * 	myTranslate.c related, currently not testable, line 428
     *
    mySymbol symbol = MySymbol_MakeSymbol("bolOne");
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Record(NULL));
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbol, entry);

    myLValueExp exp = makeMyLValue(makeOnePos(), symbol, NULL);


    myTranslationAndType result =
        MySemantic_LValueExp_SimpleVar(exp);

    CU_ASSERT(isTypeRecord(result->type));
    */
}

//////////////////////////////////////////////////////////////////////////////

void test_MySemanticLValueExpRecordField_VariableNotDeclared_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myLValueExp recordExp = makeMyLValue(makeOnePos(), makeOneSymbol(), 
        makeMyLValueAux(makeOneSymbol(), NULL, NULL));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(
        recordExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpRecordField_VariableNotRecord_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol symbolRecord = MySymbol_MakeSymbol("record");
    mySymbol symbolField = MySymbol_MakeSymbol("field");

    myVarAndFuncEntry intEntry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Int());
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolRecord, intEntry);

    myLValueExp recordExp = makeMyLValue(makeOnePos(), symbolRecord, 
        makeMyLValueAux(symbolField, NULL, NULL));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(
        recordExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpRecordField_FieldNamesNotMatch_ReturnSemanticError(void)
{
    /*  myTranslate.c
     *
    mySymbol symbolRecord = MySymbol_MakeSymbol("record");
    mySymbol symbolField = MySymbol_MakeSymbol("field");

    myTable variableSymbolTable = myEnvironment_BaseVarAndFunc();
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(symbolField, makeType_Int()),
            NULL)));
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolRecord, entry);

    mySymbol anotherField = MySymbol_MakeSymbol("anotherField");
    myLValueExp exp = makeMyLValue(makeOnePos(), symbolRecord, 
        makeMyLValueAux(anotherField, NULL, NULL));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
    */
}

void test_MySemanticLValueExpRecordField_LegalExp_ReturnTypeOfRecordField(void)
{
    /* myTranslate.c
     *
    mySymbol symbolRecord = MySymbol_MakeSymbol("record");
    mySymbol symbolField = MySymbol_MakeSymbol("field");

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myType innerRecordType = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbolField, makeType_Int()),
        NULL));
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL,
        makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(symbolField, innerRecordType),
            NULL)));
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolRecord, entry);

    myLValueExp exp = makeMyLValue(makeOnePos(), symbolRecord, 
        makeMyLValueAux(symbolField, NULL, 
            makeMyLValueAux(symbolField, NULL, NULL)));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(exp);

    CU_ASSERT(isTypeInt(result->type));
    */
}

///////////////////////////////////////////////////////////////////////////////

 void test_MySemanticLValueExpArraySubscript_ArrayVarNotDeclared_ReturnSemanticError(void)
 {
     MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
     MySemantic_setTypeEnvironment(myEnvironment_BaseType());
     myLValueExp lValueExp = makeOneLValueExp_TwoIntArraySubscript(
         makeOneSymbol(), makeOneExp_Integer(), makeOneExp_Integer());

    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(
        lValueExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
 }

void test_MySemanticLValueExpArraySubscript_VarNotArrayType_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol arrayName = makeOneSymbol();
    myLValueExp lValueExp = makeOneLValueExp_TwoIntArraySubscript(
         arrayName, makeOneExp_Integer(), makeOneExp_Integer());

    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL, makeType_Int()); 
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), arrayName, entry);


    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(
        lValueExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpArraySubscript_SubscriptNotInt_ReturnSemanticError(void)
{
    /* myTranslate.c 
     *
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol arrayName = makeOneSymbol();
    myLValueExp lValueExp = makeOneLValueExp_TwoIntArraySubscript(
         arrayName, makeOneExp_Integer(), makeOneExp_String());

    myType arrayType = makeOneArray_StringArrayArray();
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL, arrayType);
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), arrayName, entry);
    MySymbol_Enter(MySemantic_getTypeEnvironment(), arrayName, arrayType);


    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(
        lValueExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
    */
}

void test_MySemanticLValueExpArraySubscript_LegalExp_ReturnTypeOfArray(void)
{
    /* myTranslate.c
     *
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(
        (Trans_myAccess)NULL, makeOneArray_StringArrayArray());

    mySymbol symbolArray = MySymbol_MakeSymbol("array");
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), symbolArray, entry);
    MySymbol_Enter(MySemantic_getTypeEnvironment(), symbolArray, MyEnvironment_getVarType(entry));

    myLValueExp exp = makeOneLValueExp_TwoIntArraySubscript(symbolArray,
        makeOneExp_Integer(), makeOneExp_Integer());


    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(exp);

    CU_ASSERT(isTypeString(result->type));
    */
}

///////////////////////////////////////////////////////////////////////////////

void test_MySemanticNilExp_ByDefault_ReturnNilType(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;

    myPos pos;  pos.column = pos.line = 0;
    myNilExp exp = makeMyNilExp();

    myTranslationAndType result = MySemantic_NilExp(exp);

    CU_ASSERT(isTypeNil(result->type));
}

///////////////////////////////////////////////////////////////////////////////

void test_MySemanticIntegerLiteralExp_ByDefault_ReturnIntType(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;

    myPos pos;  pos.column = pos.line = 0;
    myIntegerLiteralExp exp = makeMyIntegerLiteralExp(1);

    myTranslationAndType result = 
        MySemantic_IntegerLiteralExp(exp);

    CU_ASSERT(isTypeInt(result->type));
}

///////////////////////////////////////////////////////////////////////////////

void test_MySemanticStringLiteralExp_ByDefault_ReturnStringType(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;

    myPos pos;  pos.column = pos.line = 0;
    myStringLiteralExp exp = makeMyStringLiteralExp("1");

    myTranslationAndType result = 
        MySemantic_StringLiteralExp(exp);

    CU_ASSERT(isTypeString(result->type));
}

///////////////////////////////////////////////////////////////////////////////

void test_MySemanticNoValueExp_ByDefault_ReturnNoReturnType(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;

    myPos pos;  pos.column = pos.line = 0;
    myNoValueExp exp = makeMyNoValueExp();

    myTranslationAndType result = 
        MySemantic_NoValueExp(exp);

    CU_ASSERT(isTypeNoReturn(result->type));
}

///////////////////////////////////////////////////////////////////////////////

void test_MySemanticBreakExp_InsideLoop_ReturnNoReturnType(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;
    myBreakExp exp = makeMyBreakExp();

    enterLoop();
    pushBreakTarget(NULL);
    myTranslationAndType result = 
        MySemantic_BreakExp(exp);

    CU_ASSERT(result != SEMANTIC_ERROR && isTypeNoReturn(result->type));
}

void test_MySemanticBreakExp_NotInsideLoop_ReturnSemanticError(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;
    myBreakExp exp = makeMyBreakExp();

    leaveLoop();
    myTranslationAndType result = 
        MySemantic_BreakExp(exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

//////////////////////////////////////////////////////////////////////////////////

void test_MySemanticFunctionCallExpParam_FunctionNotDefined_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myParamFunctionCallExp exp = makeMyParamFunctionCallExp(
        makeOneSymbol(),
        makeMyExpList(
            makeOneExp_Integer(),
            makeMyExpList(makeOneExp_String(),NULL)));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        exp);

    
    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticFunctionCallExpParam_ParamsTypeNotMatch_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionSymbol = MySymbol_MakeSymbol("function");
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        (Trans_myLevel)NULL, (myLabel)NULL,
        makeType_TypeList(
            makeType_String(),
            makeType_TypeList(makeType_String(), NULL)),
        makeType_Int());
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), functionSymbol, functionEntry);

    myParamFunctionCallExp functionExp = makeMyParamFunctionCallExp(
        functionSymbol,
        makeMyExpList(
            makeOneExp_Integer(),
            makeMyExpList(makeOneExp_String(),NULL)));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        functionExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticFunctionCallExpParam_LegalExp_ReturnTypeOfFunctionResult(void)
{ 
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionSymbol = MySymbol_MakeSymbol("function");
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        (Trans_myLevel)NULL, (myLabel)NULL,
        makeType_TypeList(
            makeType_Int(),
            makeType_TypeList(makeType_String(), NULL)),
        makeType_Int());
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), functionSymbol, functionEntry);

    myParamFunctionCallExp exp = makeMyParamFunctionCallExp(
        functionSymbol,
        makeMyExpList(
            makeOneExp_Integer(),
            makeMyExpList(makeOneExp_String(),NULL)));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        exp);

    
    CU_ASSERT(result != SEMANTIC_ERROR && isTypeInt(result->type));
}

void test_MySemanticFunctionCallExpParam_NilValueOfRecordParamType_Succeed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol recordTypeName = MySymbol_MakeSymbol("record type");
    myType recordType = makeAndAddOneType_NoFieldRecord(
        recordTypeName);

    mySymbol functionSymbol = MySymbol_MakeSymbol("function");
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        (Trans_myLevel)NULL, (myLabel)NULL,
        makeType_TypeList(
            makeType_Record(makeType_TypeFieldList(
                makeType_TypeField(makeOneSymbol(), recordType),
                NULL)),
            NULL),
        makeType_Int());
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), functionSymbol, functionEntry);

    myParamFunctionCallExp exp = makeMyParamFunctionCallExp(
        functionSymbol,
        makeMyExpList(
            makeOneExp_Nil(), NULL));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        exp);

    
    CU_ASSERT(result != SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////////////

void test_MySemanticFunctionCallExpNoParam_FunctionNotDefined_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myNoParamFunctionCallExp exp = makeMyNoParamFunctionCallExp(
        makeOneSymbol());

    myTranslationAndType result = MySemantic_FunctionCallExp_NoParam(
        exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticFunctionCallExpNoParam_LegalExp_ReturnTypeOfFunctionResult(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionSymbol = MySymbol_MakeSymbol("function"); 
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        (Trans_myLevel)NULL, (myLabel)NULL,
        NULL, makeType_Int());
    MySymbol_Enter(MySemantic_getVarAndFuncEnvironment(), functionSymbol, functionEntry);

    myNoParamFunctionCallExp exp = makeMyNoParamFunctionCallExp(
        functionSymbol);


    myTranslationAndType result = MySemantic_FunctionCallExp_NoParam(
        exp);

    CU_ASSERT(result != SEMANTIC_ERROR && isTypeInt(result->type));
}

/////////////////////////////////////////////////////////////////////////////

//  test template for illegal `ArrayCreationExp`
void test_IllegalIntArrayCreation_ReturnSemanticError(myExp length, myExp initial);

/////////////////////////////

void test_MySemanticArrayCreationExp_ArrayTypeNotDefined_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myArrayCreationExp arrayCreationExp = makeMyArrayCreationExp(
        makeOneSymbol(), makeOneExp_Integer(), makeOneExp_Integer());


    myTranslationAndType result = MySemantic_ArrayCreationExp(
        arrayCreationExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticArrayCreationExp_OnlyLengthPartIllegal_ReturnSemanticError(void)
{
    myExp lengthExpNotInt = makeOneExp_String();
    myExp legalInitialExp = makeOneExp_Integer();

    test_IllegalIntArrayCreation_ReturnSemanticError(lengthExpNotInt, legalInitialExp);
}

void test_MySemanticArrayCreationExp_OnlyInitialPartIllegal_ReturnSemanticError(void)
{
    myExp legalLength = makeOneExp_Integer();
    myExp initialExpNotArrayElementType = makeOneExp_NoValue();

    test_IllegalIntArrayCreation_ReturnSemanticError(legalLength, initialExpNotArrayElementType);
}

void test_MySemanticArrayCreationExp_LegalExp_ReturnArrayType(void)
{
    mySymbol arrayName = MySymbol_MakeSymbol("array");

    myExp length = makeOneExp_Integer();
    myExp initial = makeOneExp_Integer();
    myArrayCreationExp exp = makeMyArrayCreationExp(arrayName, length, initial);

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myType arrayType = makeType_Array(makeType_Int());
    MySymbol_Enter(MySemantic_getTypeEnvironment(), arrayName, arrayType);

    myTranslationAndType result = 
        MySemantic_ArrayCreationExp(exp);

    CU_ASSERT(result != SEMANTIC_ERROR && isTypeEqual(result->type, arrayType));
}

void test_MySemanticArrayCreationExp_NilInitialValueOfRecordElement_Succeed(void)
{
    mySymbol arrayName = MySymbol_MakeSymbol("array");
    myArrayCreationExp nilValueExp = makeMyArrayCreationExp(
        arrayName, makeOneExp_Integer(), makeOneExp_Nil());

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myType arrayType = makeType_Array(
        makeAndAddOneType_NoFieldRecord(
            makeOneSymbol()));
    MySymbol_Enter(MySemantic_getTypeEnvironment(), arrayName, arrayType);


    myTranslationAndType result = 
        MySemantic_ArrayCreationExp(nilValueExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalIntArrayCreation_ReturnSemanticError(myExp length, myExp initial)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol arrayTypeName = MySymbol_MakeSymbol("array");
    myArrayCreationExp exp = makeMyArrayCreationExp(arrayTypeName, length, initial);

    myType arrayType = makeType_Array(makeType_Int());
    MySymbol_Enter(MySemantic_getTypeEnvironment(), arrayTypeName, arrayType);


    myTranslationAndType result = MySemantic_ArrayCreationExp(
        exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticRecordCreationExpNoField_RecordTypeNotDefined_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myNoFieldRecordCreationExp noFieldRecordCreationExp = 
        makeOneNoFieldCreationExp();

    myTranslationAndType result =
        MySemantic_RecordCreationExp_NoField(noFieldRecordCreationExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticRecordCreationExpNoField_TypeNotRecord_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    MySymbol_Enter(MySemantic_getTypeEnvironment(), recordTypeName, makeType_Int());

    myNoFieldRecordCreationExp noFieldRecordCreationExp =
        makeMyNoFieldRecordCreationExp(recordTypeName);

    
    myTranslationAndType result = MySemantic_RecordCreationExp_NoField(
        noFieldRecordCreationExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticRecordCreationExpNoField_LegalExp_ReturnRecordType(void)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("noFieldRecord");
    myNoFieldRecordCreationExp exp = 
        makeMyNoFieldRecordCreationExp(recordTypeName);

    mySymbol fieldName = MySymbol_MakeSymbol("field");

    myType recordType = makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(fieldName, makeType_Int()),
            NULL));
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySymbol_Enter(MySemantic_getTypeEnvironment(), recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_NoField(exp);

    CU_ASSERT(isTypeEqual(result->type, recordType));    
}

/////////////////////////////////////////////////////////////////////////////
//todo: change mysymbol_enter to ..
void test_MySemanticRecordCreationExpField_TypeNotRecord_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol fieldName = MySymbol_MakeSymbol("field");
    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(fieldName, makeOneExp_Integer()),
                NULL));

    MySymbol_Enter(MySemantic_getTypeEnvironment(), recordTypeName, makeType_Int());

    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticRecordCreationExpField_RecordTypeNotDefined_ReturnSemanticError(void)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol fieldName = MySymbol_MakeSymbol("field");

    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(fieldName, makeOneExp_Integer()),
                NULL));

    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);   
}

void test_MySemanticRecordCreationExpField_RecordFieldsNotMatch_ReturnSemanticError(void)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol recordFieldName = MySymbol_MakeSymbol("field");
    mySymbol anotherFieldName = MySymbol_MakeSymbol("another");

    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(anotherFieldName, makeOneExp_Integer()),
                NULL));

    myType recordType = makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(recordFieldName, makeType_Int()),
            NULL));
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySymbol_Enter(MySemantic_getTypeEnvironment(), recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);   
}

void test_MySemanticRecordCreationExpField_LegalExp_ReturnRecordType(void)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol fieldName = MySymbol_MakeSymbol("field");

    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(fieldName, makeOneExp_Integer()),
                NULL));

    myType recordType = makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(fieldName, makeType_Int()),
            NULL));
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySymbol_Enter(MySemantic_getTypeEnvironment(), recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(exp);

    CU_ASSERT(isTypeEqual(result->type, recordType));   
}

void test_MySemanticRecordCreationExpField_NilValueOfRecordField_Succeed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol fieldName = MySymbol_MakeSymbol("field");
    mySymbol anotherRecordTypeName = MySymbol_MakeSymbol("fieldfield");
    myType anotherRecordType = makeAndAddOneType_NoFieldRecord(
        anotherRecordTypeName);

    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(fieldName, makeOneExp_Nil()),
                NULL));
    myType recordType = makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(fieldName, anotherRecordType),
            NULL));
    MySymbol_Enter(MySemantic_getTypeEnvironment(), recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(exp);

    CU_ASSERT(result != SEMANTIC_ERROR);   
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticArithmeticExp_IllegalExp_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myArithmeticExp incorrectLeftOperand = makeMyArithmeticExp_Divide(
        makeOneExp_Integer(), makeOneExp_String());
    myArithmeticExp incorrectRightOperand = makeMyArithmeticExp_Divide(
        makeOneExp_String(), makeOneExp_Integer());
    myArithmeticExp incorrectBothOperands = makeMyArithmeticExp_Divide(
        makeOneExp_NoValue(), makeOneExp_String());

    myTranslationAndType resultLeftOperand = 
        MySemantic_ArithmeticExp(incorrectLeftOperand);
    myTranslationAndType resultRightOperand = 
        MySemantic_ArithmeticExp(incorrectRightOperand);
    myTranslationAndType resultBothOperands = 
        MySemantic_ArithmeticExp(incorrectBothOperands);

    CU_ASSERT_EQUAL(resultLeftOperand, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultRightOperand, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultBothOperands, SEMANTIC_ERROR);
}

void test_MySemanticArithmeticExp_LegalExp_ReturnIntType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myArithmeticExp correctExp = makeMyArithmeticExp_Divide(
        makeOneExp_Integer(), makeOneExp_Integer());
    
    myTranslationAndType correctResult = 
        MySemantic_ArithmeticExp(correctExp);

    CU_ASSERT(correctResult != SEMANTIC_ERROR && isTypeInt(correctResult->type));
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticParenthesesExp_BeDefault_ReturnInnerResult(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myIntegerLiteralExp innerExp = makeMyIntegerLiteralExp(1);
    myParenthesesExp exp = makeMyParenthesesExp(
        makeMyExp_IntegerLiteral(makeOnePos(), innerExp));

    myTranslationAndType result =
        MySemantic_ParenthesesExp(exp);
    myTranslationAndType resultInner =
        MySemantic_IntegerLiteralExp(innerExp);

    CU_ASSERT(isTypeEqual(resultInner->type, result->type)); 
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticSequencingExp_AnyOneIllegal_ReturnSemanticError(void)
{
    /* myTranslate.c, line 358
     *
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myExp illegalForExp = makeMyExp_For(makeOnePos(),
        makeMyForExp(MySymbol_MakeSymbol("var"),
            makeOneExp_NoValue(), makeOneExp_NoValue(), makeOneExp_NoValue()));
    
    mySequencingExp illegalOne = makeMySequencingExp(
        makeOneExp_Integer(), illegalForExp, NULL);
    mySequencingExp illegalTwo = makeMySequencingExp(
        illegalForExp, makeOneExp_Integer(), NULL);
    mySequencingExp illegalThree = makeMySequencingExp(
        makeOneExp_Integer(), makeOneExp_String(),
        makeMyExpList(
            makeOneExp_String(),
            makeMyExpList(illegalForExp, NULL)));

    myTranslationAndType resultOne =
        MySemantic_SequencingExp(illegalOne);
    myTranslationAndType resultTwo =
        MySemantic_SequencingExp(illegalTwo);
    myTranslationAndType resultThree =
        MySemantic_SequencingExp(illegalThree);

    CU_ASSERT_EQUAL(resultOne, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultTwo, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultThree, SEMANTIC_ERROR);
    */
}

void test_MySemanticSequencingExp_LegalExp_ReturnLastExpressionResult(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySequencingExp expOne = makeMySequencingExp(
        makeOneExp_Integer(), makeOneExp_Integer(), NULL);
    mySequencingExp expTwo = makeMySequencingExp(
        makeOneExp_Integer(), makeOneExp_String(),
        makeMyExpList(
            makeOneExp_String(),
            makeMyExpList(makeOneExp_String(), NULL)));
    
    myTranslationAndType resultOne = 
        MySemantic_SequencingExp(expOne);
    myTranslationAndType resultTwo = 
        MySemantic_SequencingExp(expTwo);
    
    CU_ASSERT(isTypeInt(resultOne->type));
    CU_ASSERT(isTypeString(resultTwo->type));
}

//////////////////////////////////////////////////////////////////////////////

//  forward declarations
void test_IllegalForExp_ReturnSemanticError(myExp low, myExp high, myExp body);

////////////////////

void test_MySemanticForExp_OnlyLowRangeNotInt_ReturnSemanticError(void)
{
    /* myTranslate.c
     *
    myExp illegalLow = makeOneExp_NoValue();
    myExp legalHigh = makeOneExp_Integer();
    myExp legalBody = makeOneExp_NoValue();

    test_IllegalForExp_ReturnSemanticError(illegalLow, legalHigh, legalBody);
    */
}

void test_MySemanticForExp_OnlyHighRangeNotInt_ReturnSemanticError(void)
{
    /* myTranslate.c
     *
    myExp legalLow = makeOneExp_Integer();
    myExp illegalHigh = makeOneExp_NoValue();
    myExp legalBody = makeOneExp_NoValue();

    test_IllegalForExp_ReturnSemanticError(legalLow, illegalHigh, legalBody);
    */
}

void test_MySemanticForExp_OnlybodyNotTypeNoReturn_ReturnSemanticErrort(void)
{
    myExp legalLow = makeOneExp_Integer();
    myExp legalHigh = makeOneExp_Integer();
    myExp illegalBody = makeOneExp_Integer();

    test_IllegalForExp_ReturnSemanticError(legalLow, legalHigh, illegalBody);
}

void test_MySemanticForExp_AssignValueToLoopVarInBody_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol forVarName = MySymbol_MakeSymbol("var");
    myExp assignmentBody = makeMyExp_Assignment(makeOnePos(),
        makeMyAssignmentExp(
            makeMyLValue(makeOnePos(), forVarName, NULL),
            makeOneExp_Integer())); 
    myForExp forExp = makeMyForExp(
        forVarName,
        makeOneExp_Integer(),
        makeOneExp_Integer(),
        assignmentBody);

    myTranslationAndType result = MySemantic_ForExp(forExp);
    
    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticForExp_LegalForExp_ReturnTypeNoReturn(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol forVarName = MySymbol_MakeSymbol("var"); 
    myForExp forExp = makeMyForExp(
        forVarName,
        makeOneExp_Integer(),
        makeOneExp_Integer(),
        makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_ForExp(forExp);
    
    CU_ASSERT(isTypeNoReturn(result->type));

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticForExp_LegalForExp_ConditionVarCanBeUsedInBody(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol forVarName = MySymbol_MakeSymbol("var"); 

    myForExp forExp = makeMyForExp(
        forVarName,
        makeOneExp_Integer(),
        makeOneExp_Integer(),
        makeMyExp_Sequencing(makeOnePos(),
            makeMySequencingExp(
                makeMyExp_LValue(makeOnePos(), 
                        makeMyLValue(makeOnePos(), forVarName, NULL)),
                makeOneExp_NoValue(),
                NULL)));

    myTranslationAndType result =
        MySemantic_ForExp(forExp);
    
    CU_ASSERT_NOT_EQUAL(result, SEMANTIC_ERROR);

    //  clean up
    MySemantic_leaveNewLevel();
}

//  a parameterized test
void test_IllegalForExp_ReturnSemanticError(myExp low, myExp high, myExp body)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol forVarName = MySymbol_MakeSymbol("var"); 
    myForExp forExp = makeMyForExp(forVarName, low, high, body);

    myTranslationAndType result =
        MySemantic_ForExp(forExp);
    
    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);

    //  clean up
    MySemantic_leaveNewLevel();
}

//////////////////////////////////////////////////////////////////////////////

//  a parameterized tester for illegal IfThenElseExp
void test_IllegalIfThenElseExp_ReturnSemanticError(myExp ifExp, myExp thenExp, myExp elseExp);

//////////////////

void test_MySemanticIfThenElseExp_OnlyIfClauseNotInt_ReturnSemanticError(void)
{
    myExp illegalIf = makeOneExp_NoValue();
    myExp legalThen = makeOneExp_Integer();
    myExp legalElse = makeOneExp_Integer();

    test_IllegalIfThenElseExp_ReturnSemanticError(illegalIf, legalThen, legalElse);
}

void test_MySemanticIfThenElseExp_OnlyThenClauseNotInt_ReturnSemanticError(void)
{
    myExp legalIf = makeOneExp_Integer();
    myExp illegalThen = makeMyExp_IfThenElse(makeOnePos(),
        makeMyIfThenElseExp(
            makeOneExp_NoValue(), makeOneExp_String(), makeOneExp_String()));
    myExp legalElse = makeOneExp_Integer();

    test_IllegalIfThenElseExp_ReturnSemanticError(legalIf, illegalThen, legalElse);
}

void test_MySemanticIfThenElseExp_OnlyElseClauseNotInt_ReturnSemanticError(void)
{
    myExp legalIf = makeOneExp_Integer();
    myExp legalThen = makeOneExp_Integer();
    myExp illegalElse= makeMyExp_IfThenElse(makeOnePos(),
        makeMyIfThenElseExp(
            makeOneExp_NoValue(), makeOneExp_String(), makeOneExp_String()));

    test_IllegalIfThenElseExp_ReturnSemanticError(legalIf, legalThen, illegalElse);
}

void test_MySemanticIfThenElseExp_OnlyThenElseTypeMismatch_ReturnSemanticError(void)
{
    myExp legalIf = makeOneExp_Integer();
    myExp intThen = makeOneExp_Integer();
    myExp stringElse = makeOneExp_String();

    test_IllegalIfThenElseExp_ReturnSemanticError(legalIf, intThen, stringElse);
}

void test_MySemanticIfThenElseExp_LegalExpression_ReturnClauseType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myIfThenElseExp exp = makeMyIfThenElseExp(
        makeOneExp_Integer(), makeOneExp_String(), makeOneExp_String());

    myTranslationAndType result = 
        MySemantic_IfThenElseExp(exp);

    CU_ASSERT(isTypeString(result->type));
}

void test_MySemanticIfThenElseExp_OneNilAnotherRecord_Succeed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol recordTypeName = MySymbol_MakeSymbol("record type");
    myType recordType = makeAndAddOneType_NoFieldRecord(
        recordTypeName);

    myIfThenElseExp expOne = makeMyIfThenElseExp(
        makeOneExp_Integer(), makeOneExp_Record(recordTypeName), makeOneExp_Nil());
    myIfThenElseExp expTwo = makeMyIfThenElseExp(
        makeOneExp_Integer(), makeOneExp_Nil(), makeOneExp_Record(recordTypeName));

    myTranslationAndType resultOne = 
        MySemantic_IfThenElseExp(expOne);
    myTranslationAndType resultTwo = 
        MySemantic_IfThenElseExp(expTwo);

    CU_ASSERT(resultOne != SEMANTIC_ERROR);
    CU_ASSERT(resultTwo != SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalIfThenElseExp_ReturnSemanticError(myExp ifExp, myExp thenExp, myExp elseExp)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myIfThenElseExp exp = makeMyIfThenElseExp(ifExp, thenExp, elseExp);

    myTranslationAndType result = 
        MySemantic_IfThenElseExp(exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

///////////////////////////////////////////////////////////////////////////////

//  a template test
void test_IllegalIfThenExp_ReturnSemanticError(myExp ifExp, myExp thenExp);

//////////////////////////////

void test_MySemanticIfThenExp_OnlyIfconditionNotInt_ReturnSemanticError(void)
{
    myExp noReturnIfCondition= makeOneExp_NoValue();
    myExp legalThen = makeOneExp_NoValue();

    test_IllegalIfThenExp_ReturnSemanticError(noReturnIfCondition, legalThen);
}

void test_MySemanticIfThenExp_OnlyThenClauseNotNoReturn_ReturnSemanticError(void)
{
    myExp legalIfCondition = makeOneExp_Integer();
    myExp intTypeThenClause = makeOneExp_Integer();

    test_IllegalIfThenExp_ReturnSemanticError(legalIfCondition, intTypeThenClause);
}

void test_MySemanticIfThenExp_LegalExp_ReturnNoReturnType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myIfThenExp ifThenExp = makeMyIfThenExp(
        makeOneExp_Integer(), makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_IfThenExp(ifThenExp);

    CU_ASSERT(isTypeNoReturn(result->type));
}

//  a parameterized test
void test_IllegalIfThenExp_ReturnSemanticError(myExp ifExp, myExp thenExp)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myIfThenExp ifThenExp = makeMyIfThenExp(ifExp, thenExp);

    myTranslationAndType result =
        MySemantic_IfThenExp(ifThenExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

//  a parameterized test
void test_IllegalComparisonExp_ReturnSemanticError(myExp lhs, myExp rhs);

///////////////////

void test_MySemanticComparisonExp_IllegalLeftOperand_ReturnSemanticError(void)
{
    myExp leftIllegalOperand = makeOneIllegalExp_Integer();
    myExp rightLegalOperand = makeOneExp_Integer();

    test_IllegalComparisonExp_ReturnSemanticError(leftIllegalOperand, rightLegalOperand);
}

void test_MySemanticComparisonExp_IllegalRightOperand_ReturnSemanticError(void)
{
    myExp leftLegalOperand = makeOneExp_Integer();
    myExp rightIllegalOperand = makeOneIllegalExp_Integer();

    test_IllegalComparisonExp_ReturnSemanticError(leftLegalOperand, rightIllegalOperand);
}

void test_MySemanticComparisonExp_OperandsTypeNotEqual_ReturnSemanticError(void)
{
    myExp intOperand = makeOneExp_Integer();
    myExp stringOperand = makeOneExp_String();

    test_IllegalComparisonExp_ReturnSemanticError(intOperand, stringOperand);
    test_IllegalComparisonExp_ReturnSemanticError(stringOperand, intOperand);
}

void test_MySemanticComparisonExp_OperandsTypesAllNoReturn_ReturnSemanticError(void)
{
   myExp breakOperand = makeOneExp_NoValue();

    test_IllegalComparisonExp_ReturnSemanticError(breakOperand, breakOperand);
}

void test_MySemanticComparisonExp_LegalExp_ReturnIntType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myComparisonExp intComparison = makeMyComparisonExp_Equal(
        makeOneExp_Integer(), makeOneExp_Integer());
    
    myTranslationAndType result = MySemantic_ComparisonExp(
        intComparison);

    CU_ASSERT(isTypeInt(result->type));
}

void test_MySemanticComparisonExp_OneRecordAnotherNil_Succeed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol recordTypeName = makeOneSymbol();
    makeAndAddOneType_NoFieldRecord(
        recordTypeName);

    myComparisonExp intComparisonOne = makeMyComparisonExp_Equal(
       makeOneExp_Record(recordTypeName) , makeOneExp_Nil());
    myComparisonExp intComparisonTwo = makeMyComparisonExp_Equal(
       makeOneExp_Nil(), makeOneExp_Record(recordTypeName));
    
    myTranslationAndType resultOne = MySemantic_ComparisonExp(
        intComparisonOne);
    myTranslationAndType resultTwo = MySemantic_ComparisonExp(
        intComparisonTwo);

    CU_ASSERT(resultOne != SEMANTIC_ERROR);
    CU_ASSERT(resultTwo != SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalComparisonExp_ReturnSemanticError(myExp lhs, myExp rhs)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myComparisonExp comparisonExp = makeMyComparisonExp_Equal(lhs, rhs);

    myTranslationAndType result = MySemantic_ComparisonExp(
        comparisonExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

//  a template test
void test_IllegalBooleanOperateExp_ReturnSemanticError(
    myExp leftOperand, myExp rightOperand);

///////////////

void test_MySemanticBooleanOperateExp_OnlyLeftOperandNotInt_ReturnSemanticError(void)
{
    myExp stringLeftOperand = makeOneExp_String();
    myExp intRightOperand =  makeOneExp_Integer();

    test_IllegalBooleanOperateExp_ReturnSemanticError(stringLeftOperand, intRightOperand);
}

void test_MySemanticBooleanOperateExp_OnlyRightOperandNotInt_ReturnSemanticError(void)
{
    myExp intLeftOperand = makeOneExp_Integer();
    myExp stringRightOperand =  makeOneExp_String();

    test_IllegalBooleanOperateExp_ReturnSemanticError(intLeftOperand, stringRightOperand);
}

void test_MySemanticBooleanOperateExp_LegalExp_ReturnIntType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myBooleanOperateExp exp = makeMyBooleanOperateExp_Or(
        makeOneExp_Integer(), makeOneExp_Integer());

    myTranslationAndType result = MySemantic_BooleanOperateExp(
        exp);

    CU_ASSERT(isTypeInt(result->type));
}

//  a parameterized test
void test_IllegalBooleanOperateExp_ReturnSemanticError(
    myExp leftOperand, myExp rightOperand)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myBooleanOperateExp exp = makeMyBooleanOperateExp_Or(
        leftOperand, rightOperand);

    myTranslationAndType result = MySemantic_BooleanOperateExp(
        exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

//  forwards
void test_IllegalAssignmentExp_ReturnSemanticError(
    myLValueExp leftOperand, myExp rightOperand);

////////////////////

void test_MySemanticAssignmentExp_OnlyLeftOperandIllegal_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myLValueExp illegalLeftOperand =
        makeMyLValue(makeOnePos(), makeOneSymbol(), NULL);
    myExp legalRightOperand = makeOneExp_Integer();

    test_IllegalAssignmentExp_ReturnSemanticError(
        illegalLeftOperand, legalRightOperand);
}



void test_MySemanticAssignmentExp_OnlyRightOperandIllegal_ReturnSemanticError(void)
{
  /* myTranslate.c 492
   *
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myLValueExp legalLeftOperand =
        makeOneLegalLValueExp_SimpleVar_Int();
    myExp illegalRightOperand = makeOneIllegalExp_Integer();

    test_IllegalAssignmentExp_ReturnSemanticError(
        legalLeftOperand, illegalRightOperand);    
  */
}

void test_MySemanticAssignmentExp_OperandsTypeNotMatch_ReturnSemanticError(void)
{
  /* myTranslate.c
   *
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myLValueExp intLeftOperand =
        makeOneLegalLValueExp_SimpleVar_Int(); 
    myExp stringRightOperand = makeOneExp_String();

    test_IllegalAssignmentExp_ReturnSemanticError(
        intLeftOperand, stringRightOperand);
  */
}

void test_MySemanticAssignmentExp_RightOperandTypeNoReturn_ReturnSemanticError(void)
{
  /* myTranslate.c
   *
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myLValueExp intLeftOperand =
        makeOneLegalLValueExp_SimpleVar_Int(); 
    myExp breakExp = makeOneExp_NoValue();

    test_IllegalAssignmentExp_ReturnSemanticError(
        intLeftOperand, breakExp);
  */
}

void test_MySemanticAssignmentExp_LegalExp_ReturnNoReturnType(void)
{
  /* myTranslate.c
   *
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myAssignmentExp assignmentExp =
        makeMyAssignmentExp(
            makeOneLegalLValueExp_SimpleVar_Int(),
            makeOneExp_Integer());

    myTranslationAndType result = MySemantic_AssignmentExp(
        assignmentExp);

    CU_ASSERT(isTypeNoReturn(result->type));
  */
}

void test_MySemanticAssignmentExp_LeftRecordRightNil_Succeed(void)
{
  /* myTranslate.c
   *
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myAssignmentExp assignmentExp =
        makeMyAssignmentExp(
            makeOneLegalLValueExp_Record(
                makeAndAddOneType_NoFieldRecord(
                    makeOneSymbol())),    
            makeOneExp_Nil());


    myTranslationAndType result = MySemantic_AssignmentExp(
        assignmentExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
  */
}

//  a parameterized test
void test_IllegalAssignmentExp_ReturnSemanticError(
    myLValueExp leftOperand, myExp rightOperand)
{
    myAssignmentExp assignmentExp = makeMyAssignmentExp(
        leftOperand, rightOperand);

    myTranslationAndType result = MySemantic_AssignmentExp(
        assignmentExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

////////////////////////////////////////////////////////////////////////////

//  a parameterized test
void test_IllegalWhileExp_ReturnSemanticError(myExp conditionExp, myExp bodyExp);

//////////////////////

void test_MySemanticWhileExp_OnlyConditionNotInt_ReturnSemanticError(void)
{
    myExp noReturnCondition = makeOneExp_NoValue();
    myExp noReturnBody = makeOneExp_NoValue();

    test_IllegalWhileExp_ReturnSemanticError(noReturnCondition, noReturnBody);
}

void test_MySemanticWhileExp_OnlyBodyNotNoReturn_ReturnSemanticError(void)
{
    myExp intCondition = makeOneExp_Integer();
    myExp notNoReturnBody = makeOneExp_Integer();

    test_IllegalWhileExp_ReturnSemanticError(intCondition, notNoReturnBody);
}

void test_MySemanticWhileExp_LegalExp_ReturnNoReturnType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myWhileExp whileExp = makeMyWhileExp(
        makeOneExp_Integer(), makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_WhileExp(whileExp);

    CU_ASSERT(isTypeNoReturn(result->type));
}

//  a parameterized test
void test_IllegalWhileExp_ReturnSemanticError(myExp conditionExp, myExp bodyExp)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myWhileExp whileExp = makeMyWhileExp(conditionExp, bodyExp);

    myTranslationAndType result =
        MySemantic_WhileExp(whileExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticNegationExp_ExpNotInt_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myNegationExp negationExp = makeMyNegationExp(makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_NegationExp(negationExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticNegationExp_LegalExp_ReturnIntType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myNegationExp negationExp = makeMyNegationExp(makeOneExp_Integer());

    myTranslationAndType result =
        MySemantic_NegationExp(negationExp);

    CU_ASSERT(isTypeInt(result->type));
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeNamed_NamedTypeNotDefined_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol undefinedTypeName = MySymbol_MakeSymbol("undefined");
    mySymbol typeName = makeOneSymbol(); 

    bool result = MySemantic_Dec_Type_Named(
        typeName , undefinedTypeName);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeNamed_NamedTypeOfBuildInt_AddedActualTypeIsInt(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol newTypeName = MySymbol_MakeSymbol("newType");
    mySymbol buildIntTypeName = MySymbol_MakeSymbol("int");

    bool result =MySemantic_Dec_Type_Named(newTypeName, buildIntTypeName);

    myType expectedType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        buildIntTypeName);
    myType actualType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        newTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(expectedType, actualType->u.typeNamed->type);
}

void test_MySemanticDecTypeNamed_NamedTypeOfNamedOne_AddedTypeIsNamedActualType(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol namedTypeName = MySymbol_MakeSymbol("namedType");
    mySymbol buildIntTypeName = MySymbol_MakeSymbol("int");
    MySemantic_Dec_Type_Named(namedTypeName, buildIntTypeName);

    mySymbol newTypeName = MySymbol_MakeSymbol("newType");
    bool result = MySemantic_Dec_Type_Named(newTypeName, namedTypeName);

    myType actualType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        newTypeName);
    myType expectedType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        buildIntTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(expectedType, actualType->u.typeNamed->type->u.typeNamed->type);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeRecord_FieldTypeNotDefined_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol varName = makeOneSymbol();
    mySymbol typeName = MySymbol_MakeSymbol("not defined type");
    mySymbol recordTypeName = makeOneSymbol();
    myTyFieldList tyFields = makeMyTyFieldList(makeMyTyField(varName, typeName), NULL);

    bool result = MySemantic_Dec_Type_Record(recordTypeName, tyFields);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeRecord_LegalParam_AddOneRecordType(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), MySymbol_MakeSymbol("int")),
        NULL);
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");

    bool result = MySemantic_Dec_Type_Record(recordTypeName, tyFields);

    bool recordAdded = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        recordTypeName) != NULL; 
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(recordAdded);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeArray_ElementTypeNotDefined_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("elementType");

    bool result = MySemantic_Dec_Type_Array(newTypeName, elementTypeName);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeArray_LegalParam_TypeAdded(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("int");

    bool result = MySemantic_Dec_Type_Array(newTypeName, elementTypeName);

    bool typeAdded = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        newTypeName) != NULL;
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(typeAdded);
}

/////////////////////////////////////////////////////////////////////////////

//  forwards
void test_IllegalShortFormVar_ReturnFalse(myShortFormVar var);

///////////////

void test_MySemanticDecVarShortForm_IllegalValueExp_ReturnFalse(void)
{
    myShortFormVar illegalShortVar = makeMyShortFormVar(
        makeOneSymbol(), makeOneIllegalExp_Integer());

    test_IllegalShortFormVar_ReturnFalse(illegalShortVar);
}

void test_MySemanticDecVarShortForm_NilOrNoReturnTypeExp_ReturnFalse(void)
{
    myShortFormVar nilShortVar = makeMyShortFormVar(
        makeOneSymbol(), makeOneExp_Nil());
    myShortFormVar noReturnVar = makeMyShortFormVar(
        makeOneSymbol(), makeOneExp_NoValue());

    test_IllegalShortFormVar_ReturnFalse(nilShortVar);
    test_IllegalShortFormVar_ReturnFalse(noReturnVar);
}

void test_MySemanticDecVarShortForm_LegalIntShortVar_ValueTypedAdded(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol typeName = makeOneSymbol(); 
    myShortFormVar legalShortVar = makeMyShortFormVar(
        typeName, makeOneExp_Integer());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_ShortForm(legalShortVar, &resultPtr);

    myType addedType = MyEnvironment_getVarType(
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(),
            typeName));
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeInt(addedType));

    //  clean up
    MySemantic_leaveNewLevel();
}

//  a parameterized test
void test_IllegalShortFormVar_ReturnFalse(myShortFormVar var)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_ShortForm(var, &resultPtr);

    CU_ASSERT_EQUAL(result, false);

    //  clean up
    MySemantic_leaveNewLevel();
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecVarLongForm_VariableTypeNotDefined_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), makeOneSymbol(), makeOneExp_Integer());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_LongForm(longFormVar, &resultPtr);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecVarLongForm_VariableTypeNotMatchExpType_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), MySymbol_MakeSymbol("int"), makeOneExp_String());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_LongForm(longFormVar, &resultPtr);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecVarLongForm_NilValueOfNonRecordType_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), MySymbol_MakeSymbol("int"), makeOneExp_Nil());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_LongForm(longFormVar, &resultPtr);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecVarLongForm_NilValueOfRecordType_VarAdded(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeAndAddOneType_NoFieldRecord(
        recordTypeName);

    mySymbol varName = makeOneSymbol();
    myLongFormVar longFormVar = makeMyLongFormVar(
        varName , recordTypeName, makeOneExp_Nil());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_LongForm(longFormVar, &resultPtr);

    myVarAndFuncEntry varEntry = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(),
        varName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isVarEntry(varEntry));
    CU_ASSERT(isTypeEqual(MyEnvironment_getVarType(varEntry), recordType));

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecVarLongForm_LegalParam_VarAdded(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol varName = makeOneSymbol(); 
    myLongFormVar longFormVar = makeMyLongFormVar(
        varName, MySymbol_MakeSymbol("int"), makeOneExp_Integer());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_LongForm(longFormVar, &resultPtr);

    myVarAndFuncEntry varEntry = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(),
        varName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isVarEntry(varEntry));
    CU_ASSERT(isTypeInt(MyEnvironment_getVarType(varEntry)));

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecVarLongForm_NilValueOfRecordType_Succeed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());
    mySymbol recordTypeName = MySymbol_MakeSymbol("typeee");
    makeAndAddOneType_NoFieldRecord(
        recordTypeName);
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), recordTypeName, makeOneExp_Nil());

    IR_myStatement resultPtr;
    bool result = MySemantic_Dec_Var_LongForm(longFormVar, &resultPtr);

    CU_ASSERT_EQUAL(result, true);

    //  clean up
    MySemantic_leaveNewLevel();
}

/////////////////////////////////////////////////////////////////////////////

//  forwards
void test_IllegalProcedureDec_ReturnFalse(
    myTyFieldList funcFields, myExp funcBody);

///////////////

void test_MySemanticDecFuncProcedure_ParamTypeNotDefined_ReturnFalse(void)
{
    mySymbol notDefinedFieldTypeName = makeOneSymbol();
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), notDefinedFieldTypeName),
        NULL);

    test_IllegalProcedureDec_ReturnFalse(
        funcFields, makeOneExp_NoValue());
}

void test_MySemanticDecFuncProcedure_BodyExpNotLegal_ReturnFalse(void)
{
    MySemantic_enterNewLevel(Trans_outermostLevel());
    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp illegalBodyExp = makeOneIllegalExp_Integer(); 

    test_IllegalProcedureDec_ReturnFalse(
        funcFields, illegalBodyExp);
    
    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecFuncProcedure_BodyExpNotNoReturn_ReturnFalse(void)
{
    MySemantic_enterNewLevel(Trans_outermostLevel());
    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp bodyExpNotNoReturn = makeOneExp_Integer();

    test_IllegalProcedureDec_ReturnFalse(
        funcFields, bodyExpNotNoReturn);
    
    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecFuncProcedure_LegalDec_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp funcBody = makeOneExp_NoValue();
    mySymbol funcName = makeOneSymbol(); 
    myProcedureDec procedureDec = makeMyProcedureDec(
        funcName, funcFields, funcBody);


    bool result = 
        MySemantic_Dec_Func_Procedure(procedureDec);

    CU_ASSERT_EQUAL(result, true);

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecFuncProcedure_LegalDec_FormalsCanBeUsedInBody(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol formalName = MySymbol_MakeSymbol("formal");
    myExp formalUsedBody = makeMyExp_Assignment(makeOnePos(),
        makeMyAssignmentExp(
                makeMyLValue(makeOnePos(), formalName, NULL),
                makeOneExp_Integer())); 
    myProcedureDec procedureDec = makeMyProcedureDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(
            formalName, makeSymbol_Int()),NULL),
        formalUsedBody);


    bool result = 
        MySemantic_Dec_Func_Procedure(procedureDec);

    CU_ASSERT_EQUAL(result, true);

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecFuncProcedure_LegalDec_FuncAdded(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp funcBody = makeOneExp_NoValue();
    mySymbol funcName = makeOneSymbol(); 
    myProcedureDec procedureDec = makeMyProcedureDec(
        funcName, funcFields, funcBody);


    bool result = 
        MySemantic_Dec_Func_Procedure(procedureDec);

    myVarAndFuncEntry funcEntry =
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(),
            funcName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isFuncEntry(funcEntry));
    CU_ASSERT(isTypeInt(MyEnvironment_getFuncFormalTypes(funcEntry)->head));
    CU_ASSERT(isTypeNoReturn(MyEnvironment_getFuncReturnType(funcEntry)));

    //  clean up
    MySemantic_leaveNewLevel();
}

//  a parameterized test
void test_IllegalProcedureDec_ReturnFalse(
    myTyFieldList funcFields, myExp funcBody)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol funcName = makeOneSymbol(); 
    myProcedureDec procedureDec = makeMyProcedureDec(
        funcName, funcFields, funcBody);

    bool result = 
        MySemantic_Dec_Func_Procedure(procedureDec);

    CU_ASSERT_EQUAL(result, false);
}

/////////////////////////////////////////////////////////////////////////////

//  forwards
void test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
    myFunctionDec functionDec);

/////////////////

void test_MySemanticDecFuncFunction_ParamTypesNotDefined_ReturnFalseAndFunctionNotAdd(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol notDefinedParamTypeName = makeOneSymbol();
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), notDefinedParamTypeName), NULL),
        makeSymbol_Int(),
        makeOneExp_Integer());

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(dec);
}

void test_MySemanticDecFuncFunction_ReturnTypeNotDefined_ReturnFalseAndFunctionNotAdd(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol notDefinedReturnType = makeOneSymbol();
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeOneSymbol()), NULL),
        notDefinedReturnType,
        makeOneExp_Integer());

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(dec);
}

void test_MySemanticDecFuncFunction_BodyNotlegal_ReturnFalseAndFunctionNotAdd(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myExp illegalBody = makeOneIllegalExp_Integer(); 
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeOneSymbol()), NULL),
        makeOneSymbol(),
        illegalBody);

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(dec);
}

void test_MySemanticDecFuncFunction_ReturnTypeNotMatch_ReturnFalseAndFunctionNotAdd(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myExp intBody = makeOneExp_Integer(); 
    mySymbol stringReturnType = makeSymbol_String();
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeOneSymbol()), NULL),
        stringReturnType,
        intBody);

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(dec);
}

void test_MySemanticDecFuncFunction_LegalFunctionDec_ReturnTrueAndFunctionAdded(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        makeSymbol_Int(),
        makeOneExp_Integer());

    bool result = MySemantic_Dec_Func_Function(dec);

    myVarAndFuncEntry funcEntry =
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(),
            dec->name);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isFuncEntry(funcEntry));
    CU_ASSERT(isTypeInt(MyEnvironment_getFuncFormalTypes(funcEntry)->head));
    CU_ASSERT(isTypeInt(MyEnvironment_getFuncReturnType(funcEntry)));

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecFuncFunction_LegalFunctionDec_FormalsCanBeUsedInBody(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());
    mySymbol formalVarName = MySymbol_MakeSymbol("formal var");
    
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(formalVarName, makeSymbol_Int()), NULL),
        makeSymbol_Int(),
        makeMyExp_LValue(makeOnePos(),makeMyLValue(makeOnePos(), formalVarName, NULL)));

    bool result = MySemantic_Dec_Func_Function(dec);

    CU_ASSERT_EQUAL(result, true);    

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecFuncFunction_NilBodyOfRecordReturnType_Succeed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());
    mySymbol recordTypeName = MySymbol_MakeSymbol("record type");
    makeAndAddOneType_NoFieldRecord(
        recordTypeName);
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        recordTypeName,
        makeOneExp_Nil());

    bool result = MySemantic_Dec_Func_Function(dec);

    CU_ASSERT_EQUAL(result, true);

    //  clean up
    MySemantic_leaveNewLevel();
}

//  a parameterized test
void test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
    myFunctionDec functionDec)
{
    bool result = MySemantic_Dec_Func_Function(functionDec);

    bool functionNotAdded = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(),
        functionDec->name)
        == NULL;
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT(functionNotAdded);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecs_DecsContainsIllegalDec_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    myVarDec varDec = makeMyVarDec_ShortForm(
        makeMyShortFormVar(makeOneSymbol(), makeOneExp_Integer()));
    myFuncDec funcDec = makeMyFuncDec_Procedure(
        makeMyProcedureDec(makeOneSymbol(), NULL, makeOneExp_NoValue()));
    myTypeDec illegalTypeDec = makeMyTyDec(
        makeOneSymbol(), makeMyTy_Named(makeOnePos(), makeOneSymbol())); 
    myDecList decs = makeMyDecList(
        makeMyDec_Var(makeOnePos(),varDec),
        makeMyDecList(
            makeMyDec_Func(makeOnePos(), funcDec),
            makeMyDecList(
                makeMyDec_Type(makeOnePos(), illegalTypeDec),
                NULL)));

    IR_myStatement resultPtr;
    bool result = MySemantic_Decs(decs, &resultPtr);

    CU_ASSERT_EQUAL(result, false);

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecs_ConsecutiveSameFuncOrTypeDecs_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myFuncDec funcDec = makeMyFuncDec_Function(
        makeMyFunctionDec(MySymbol_MakeSymbol("funcname"), NULL, makeSymbol_String(), makeOneExp_String()));
    myTypeDec typeDec = makeMyTyDec(
        MySymbol_MakeSymbol("typename"), makeMyTy_Named(makeOnePos(), makeSymbol_Int()));

    myDecList sameFuncDecs = makeMyDecList(
        makeMyDec_Func(makeOnePos(),funcDec),
        makeMyDecList(
            makeMyDec_Func(makeOnePos(), funcDec),NULL));
    myDecList sameTypeDecs = makeMyDecList(
        makeMyDec_Type(makeOnePos(),typeDec),
        makeMyDecList(
            makeMyDec_Type(makeOnePos(), typeDec),NULL));

    IR_myStatement resultPtr;
    bool resultSameFunc = MySemantic_Decs(sameFuncDecs, &resultPtr);
    bool resultSameType = MySemantic_Decs(sameTypeDecs, &resultPtr);

    CU_ASSERT_EQUAL(resultSameFunc, false);
    CU_ASSERT_EQUAL(resultSameType, false);
}

void test_MySemanticDecs_NotConsecutiveSameFuncOrTypeDecs_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    myFuncDec funcDec = makeMyFuncDec_Function(
        makeMyFunctionDec(MySymbol_MakeSymbol("funcname"), NULL, makeSymbol_String(), makeOneExp_String()));
    myTypeDec typeDec = makeMyTyDec(
        MySymbol_MakeSymbol("typename"), makeMyTy_Named(makeOnePos(), makeSymbol_Int()));

    myDecList notConsecutiveFuncs = makeMyDecList(
        makeMyDec_Func(makeOnePos(),funcDec),
        makeMyDecList(
            makeMyDec_Type(makeOnePos(), typeDec),
                makeMyDecList(
                makeMyDec_Func(makeOnePos(), funcDec),NULL)));
    myDecList notConsecutiveTypes = makeMyDecList(
        makeMyDec_Type(makeOnePos(),typeDec),
        makeMyDecList(
            makeMyDec_Func(makeOnePos(), funcDec),
            makeMyDecList(
            makeMyDec_Type(makeOnePos(), typeDec),NULL)));


    IR_myStatement resultPtr;
    bool resultFunc = MySemantic_Decs(notConsecutiveFuncs, &resultPtr);
    bool resultType = MySemantic_Decs(notConsecutiveTypes, &resultPtr);

    CU_ASSERT_EQUAL(resultFunc, true);
    CU_ASSERT_EQUAL(resultType, true);

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecs_ConsecutiveSameVarDecs_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    myVarDec varDec = makeMyVarDec_ShortForm(
        makeMyShortFormVar(MySymbol_MakeSymbol("var name"), makeOneExp_Integer()));
    myDecList sameVarDecs = makeMyDecList(
        makeMyDec_Var(makeOnePos(), varDec),
        makeMyDecList(
            makeMyDec_Var(makeOnePos(), varDec),NULL));

    IR_myStatement resultPtr;
    bool resultSameVar = MySemantic_Decs(sameVarDecs, &resultPtr);

    CU_ASSERT_EQUAL(resultSameVar, true);

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticDecs_DecsContainsLegalDec_ReturnTrueAndDecsAdded(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol varName = MySymbol_MakeSymbol("variable");
    mySymbol typeName = MySymbol_MakeSymbol("typeName");
    mySymbol funcName = MySymbol_MakeSymbol("funcName");

    myVarDec varDec = makeMyVarDec_ShortForm(
        makeMyShortFormVar(varName, makeOneExp_Integer()));
    myFuncDec funcDec = makeMyFuncDec_Function(
        makeMyFunctionDec(funcName, NULL, makeSymbol_String(), makeOneExp_String()));
    myTypeDec typeDec = makeMyTyDec(
        typeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int())); 
    myDecList decs = makeMyDecList(
        makeMyDec_Var(makeOnePos(),varDec),
        makeMyDecList(
            makeMyDec_Func(makeOnePos(), funcDec),
            makeMyDecList(
                makeMyDec_Type(makeOnePos(), typeDec),
                NULL)));

    IR_myStatement resultPtr;
    bool result = MySemantic_Decs(decs, &resultPtr);

    myVarAndFuncEntry funcEntry =
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(),
            funcName);
    myVarAndFuncEntry varEntry =
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(),
            varName);
    myType type = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        typeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isFuncEntry(funcEntry));
    CU_ASSERT(isTypeString(MyEnvironment_getFuncReturnType(funcEntry)));
    CU_ASSERT(myEnvironment_isVarEntry(varEntry));
    CU_ASSERT(isTypeInt(type->u.typeNamed->type));

    //  clean up
    MySemantic_leaveNewLevel();
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticLetExp_IllegalDeclarations_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myDecList decs = makeMyDecList(
        makeMyDec_Var(makeOnePos(), 
            makeMyVarDec_ShortForm(
            makeMyShortFormVar(makeOneSymbol(), makeOneExp_NoValue()))),
        NULL);
    myExpList exps = makeMyExpList(makeOneExp_Integer(), NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(letExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLetExp_IllegalBody_ReturnSemanticError(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myDecList decs = NULL;
    myExpList exps = makeMyExpList(makeOneIllegalExp_Integer(), NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(letExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLetExp_LegalExp_ReturnLastExpType(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    myDecList decs = NULL;
    myExpList exps = makeMyExpList(makeOneExp_Integer(), NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(letExp);

    CU_ASSERT(isTypeInt(result->type));
}

void test_MySemanticLetExp_LegalExp_VarInDecsCanBeUsedInExps(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol varName = makeOneSymbol();
    myDecList decs = makeMyDecList(
        makeMyDec_Var(makeOnePos(), 
            makeMyVarDec_ShortForm(
            makeMyShortFormVar(varName, makeOneExp_Integer()))),
        NULL);
    myExpList exps = makeMyExpList(makeMyExp_LValue(makeOnePos(),
        makeMyLValue(makeOnePos(), varName, NULL)),
        NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(letExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
    CU_ASSERT(isTypeInt(result->type));

    //  clean up
    MySemantic_leaveNewLevel();
}

void test_MySemanticLetExp_LegalExp_TypesInDecsCanBeUsedInExps(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol typeName = MySymbol_MakeSymbol("typeName");
    myTypeDec typeDec = makeMyTyDec(
        typeName, makeMyTy_Array(makeOnePos(), makeSymbol_Int())); 
    myDecList decs = makeMyDecList(
                makeMyDec_Type(makeOnePos(), typeDec),NULL);
    myExpList exps = makeMyExpList(
        makeMyExp_ArrayCreation(makeOnePos(), makeMyArrayCreationExp(
            typeName, makeOneExp_Integer(), makeOneExp_Integer())),
        NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(letExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
    CU_ASSERT(isTypeArray(result->type));

    //  clean up
    MySemantic_leaveNewLevel();
} 

void test_MySemanticLetExp_LegalExp_FuncsInDecsCanBeUsedInExps(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_enterNewLevel(Trans_outermostLevel());

    mySymbol funcName = MySymbol_MakeSymbol("funct");
    myFuncDec funcDec = makeMyFuncDec_Function(
        makeMyFunctionDec(funcName, NULL, makeSymbol_String(), makeOneExp_String()));
    myDecList decs = makeMyDecList(
                makeMyDec_Func(makeOnePos(), funcDec),NULL);
    myExpList exps = makeMyExpList(
        makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
            makeMyNoParamFunctionCallExp(funcName))),
        NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(letExp);

    myType letReturnType = makeType_String();
    CU_ASSERT(result != SEMANTIC_ERROR);
    CU_ASSERT(isTypeEqual(result->type, letReturnType));

    //  clean up
    MySemantic_leaveNewLevel();
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticExp_IllegalBreak_Failed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myExp illegalExp = makeMyExp_IfThen(makeOnePos(),
        makeMyIfThenExp(makeOneExp_Integer(), makeOneExp_Break()));

    myTranslationAndType result = MySemantic_Exp(illegalExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticExp_LegalBreak_Succeed(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myExp legalExp = makeMyExp_While(makeOnePos(),
        makeMyWhileExp(makeOneExp_Integer(), makeOneExp_Break()));

    myTranslationAndType result = MySemantic_Exp(legalExp);

    CU_ASSERT_NOT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
    CU_pSuite suite;
    if (!initialTests(&suite))      return -1;

    //	add tests
    CU_TestInfo tests[] = {
        { "", test_GetterAndSetter_GetWhatSetted },

        { "", test_typeContainsLValueAux_CheckRecord_ReturnTypeOfOneField },
        { "", test_typeContainsLValueAux_CheckRecord_ReturnTypeOfNestedField },
        { "", test_typeContainsLValueAux_CheckArray_ReturnTypeOfOneField },
        { "", test_typeContainsLValueAux_CheckArray_ReturnTypeOfNestedField },
        
        { "", test_MySemanticLValueExpSimpleVar_VarNotDeclared_ReturnSemanticError },
        { "", test_MySemanticLValueExpSimpleVar_LegalExp_ReturnTypeOfSimpleVar },

        { "", test_MySemanticLValueExpRecordField_VariableNotDeclared_ReturnSemanticError },
        { "", test_MySemanticLValueExpRecordField_VariableNotRecord_ReturnSemanticError },
        { "", test_MySemanticLValueExpRecordField_FieldNamesNotMatch_ReturnSemanticError },
        { "", test_MySemanticLValueExpRecordField_LegalExp_ReturnTypeOfRecordField },

        { "", test_MySemanticLValueExpArraySubscript_ArrayVarNotDeclared_ReturnSemanticError },
        { "", test_MySemanticLValueExpArraySubscript_VarNotArrayType_ReturnSemanticError },
        { "", test_MySemanticLValueExpArraySubscript_SubscriptNotInt_ReturnSemanticError },
        { "", test_MySemanticLValueExpArraySubscript_LegalExp_ReturnTypeOfArray },

        { "", test_MySemanticNilExp_ByDefault_ReturnNilType },
        { "", test_MySemanticIntegerLiteralExp_ByDefault_ReturnIntType },
        { "", test_MySemanticStringLiteralExp_ByDefault_ReturnStringType },
        { "", test_MySemanticNoValueExp_ByDefault_ReturnNoReturnType },

        { "", test_MySemanticBreakExp_InsideLoop_ReturnNoReturnType },
        { "", test_MySemanticBreakExp_NotInsideLoop_ReturnSemanticError },

        { "", test_MySemanticFunctionCallExpNoParam_LegalExp_ReturnTypeOfFunctionResult },
        { "", test_MySemanticFunctionCallExpNoParam_FunctionNotDefined_ReturnSemanticError },

        { "", test_MySemanticFunctionCallExpParam_FunctionNotDefined_ReturnSemanticError },
        { "", test_MySemanticFunctionCallExpParam_ParamsTypeNotMatch_ReturnSemanticError },
        { "", test_MySemanticFunctionCallExpParam_LegalExp_ReturnTypeOfFunctionResult },
        { "", test_MySemanticFunctionCallExpParam_NilValueOfRecordParamType_Succeed },

        { "", test_MySemanticArrayCreationExp_LegalExp_ReturnArrayType },
        { "", test_MySemanticArrayCreationExp_NilInitialValueOfRecordElement_Succeed },
        { "", test_MySemanticArrayCreationExp_ArrayTypeNotDefined_ReturnSemanticError },
        { "", test_MySemanticArrayCreationExp_OnlyLengthPartIllegal_ReturnSemanticError },
        { "", test_MySemanticArrayCreationExp_OnlyInitialPartIllegal_ReturnSemanticError },

        { "", test_MySemanticRecordCreationExpNoField_LegalExp_ReturnRecordType },
        { "", test_MySemanticRecordCreationExpField_NilValueOfRecordField_Succeed },
        { "", test_MySemanticRecordCreationExpNoField_RecordTypeNotDefined_ReturnSemanticError },
        { "", test_MySemanticRecordCreationExpNoField_TypeNotRecord_ReturnSemanticError },

        { "", test_MySemanticRecordCreationExpField_LegalExp_ReturnRecordType },
        { "", test_MySemanticRecordCreationExpField_RecordTypeNotDefined_ReturnSemanticError },
        { "", test_MySemanticRecordCreationExpField_RecordFieldsNotMatch_ReturnSemanticError },
        { "", test_MySemanticRecordCreationExpField_TypeNotRecord_ReturnSemanticError },

        { "", test_MySemanticArithmeticExp_LegalExp_ReturnIntType },
        { "", test_MySemanticArithmeticExp_IllegalExp_ReturnSemanticError },

        { "", test_MySemanticParenthesesExp_BeDefault_ReturnInnerResult },

        { "", test_MySemanticSequencingExp_AnyOneIllegal_ReturnSemanticError },
        { "", test_MySemanticSequencingExp_LegalExp_ReturnLastExpressionResult },

        { "", test_MySemanticForExp_LegalForExp_ReturnTypeNoReturn },
        { "", test_MySemanticForExp_OnlyLowRangeNotInt_ReturnSemanticError },
        { "", test_MySemanticForExp_OnlyHighRangeNotInt_ReturnSemanticError },
        { "", test_MySemanticForExp_OnlybodyNotTypeNoReturn_ReturnSemanticErrort },
        { "", test_MySemanticForExp_AssignValueToLoopVarInBody_ReturnSemanticError },
        { "", test_MySemanticForExp_LegalForExp_ConditionVarCanBeUsedInBody },

        { "", test_MySemanticIfThenElseExp_LegalExpression_ReturnClauseType },
        { "", test_MySemanticIfThenElseExp_OneNilAnotherRecord_Succeed },
        { "", test_MySemanticIfThenElseExp_OnlyIfClauseNotInt_ReturnSemanticError },
        { "", test_MySemanticIfThenElseExp_OnlyThenClauseNotInt_ReturnSemanticError },
        { "", test_MySemanticIfThenElseExp_OnlyElseClauseNotInt_ReturnSemanticError },
        { "", test_MySemanticIfThenElseExp_OnlyThenElseTypeMismatch_ReturnSemanticError },

        { "", test_MySemanticIfThenExp_OnlyIfconditionNotInt_ReturnSemanticError },
        { "", test_MySemanticIfThenExp_OnlyThenClauseNotNoReturn_ReturnSemanticError },
        { "", test_MySemanticIfThenExp_LegalExp_ReturnNoReturnType },

        { "", test_MySemanticComparisonExp_IllegalLeftOperand_ReturnSemanticError },
        { "", test_MySemanticComparisonExp_IllegalRightOperand_ReturnSemanticError },
        { "", test_MySemanticComparisonExp_OperandsTypeNotEqual_ReturnSemanticError },
        { "", test_MySemanticComparisonExp_OperandsTypesAllNoReturn_ReturnSemanticError },
        { "", test_MySemanticComparisonExp_LegalExp_ReturnIntType },
        { "", test_MySemanticComparisonExp_OneRecordAnotherNil_Succeed },

        { "", test_MySemanticBooleanOperateExp_OnlyLeftOperandNotInt_ReturnSemanticError },
        { "", test_MySemanticBooleanOperateExp_OnlyRightOperandNotInt_ReturnSemanticError },
        { "", test_MySemanticBooleanOperateExp_LegalExp_ReturnIntType },

        { "", test_MySemanticAssignmentExp_OnlyLeftOperandIllegal_ReturnSemanticError },
        { "", test_MySemanticAssignmentExp_OnlyRightOperandIllegal_ReturnSemanticError },
        { "", test_MySemanticAssignmentExp_OperandsTypeNotMatch_ReturnSemanticError },
        { "", test_MySemanticAssignmentExp_RightOperandTypeNoReturn_ReturnSemanticError },
        { "", test_MySemanticAssignmentExp_LegalExp_ReturnNoReturnType },
        { "", test_MySemanticAssignmentExp_LeftRecordRightNil_Succeed },

        { "", test_MySemanticWhileExp_OnlyConditionNotInt_ReturnSemanticError },
        { "", test_MySemanticWhileExp_OnlyBodyNotNoReturn_ReturnSemanticError },
        { "", test_MySemanticWhileExp_LegalExp_ReturnNoReturnType },

        { "", test_MySemanticNegationExp_ExpNotInt_ReturnSemanticError },
        { "", test_MySemanticNegationExp_LegalExp_ReturnIntType },

        { "", test_MySemanticDecTypeNamed_NamedTypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeNamed_NamedTypeOfBuildInt_AddedActualTypeIsInt },
        { "", test_MySemanticDecTypeNamed_NamedTypeOfNamedOne_AddedTypeIsNamedActualType },

        { "", test_MySemanticDecTypeRecord_FieldTypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeRecord_LegalParam_AddOneRecordType },

        { "", test_MySemanticDecTypeArray_ElementTypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeArray_LegalParam_TypeAdded },

        { "", test_MySemanticDecVarShortForm_IllegalValueExp_ReturnFalse },
        { "", test_MySemanticDecVarShortForm_NilOrNoReturnTypeExp_ReturnFalse },
        { "", test_MySemanticDecVarShortForm_LegalIntShortVar_ValueTypedAdded },

        { "", test_MySemanticDecVarLongForm_VariableTypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecVarLongForm_VariableTypeNotMatchExpType_ReturnFalse },
        { "", test_MySemanticDecVarLongForm_NilValueOfNonRecordType_ReturnFalse },
        { "", test_MySemanticDecVarLongForm_NilValueOfRecordType_VarAdded },
        { "", test_MySemanticDecVarLongForm_LegalParam_VarAdded },
        { "", test_MySemanticDecVarLongForm_NilValueOfRecordType_Succeed },

        { "", test_MySemanticDecFuncProcedure_ParamTypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecFuncProcedure_BodyExpNotLegal_ReturnFalse },
        { "", test_MySemanticDecFuncProcedure_BodyExpNotNoReturn_ReturnFalse },
        { "", test_MySemanticDecFuncProcedure_LegalDec_ReturnTrue },
        { "", test_MySemanticDecFuncProcedure_LegalDec_FuncAdded },
        { "", test_MySemanticDecFuncProcedure_LegalDec_FormalsCanBeUsedInBody },

        { "", test_MySemanticDecFuncFunction_ParamTypesNotDefined_ReturnFalseAndFunctionNotAdd },
        { "", test_MySemanticDecFuncFunction_ReturnTypeNotDefined_ReturnFalseAndFunctionNotAdd },
        { "", test_MySemanticDecFuncFunction_BodyNotlegal_ReturnFalseAndFunctionNotAdd },
        { "", test_MySemanticDecFuncFunction_ReturnTypeNotMatch_ReturnFalseAndFunctionNotAdd },
        { "", test_MySemanticDecFuncFunction_LegalFunctionDec_ReturnTrueAndFunctionAdded },
        { "", test_MySemanticDecFuncFunction_NilBodyOfRecordReturnType_Succeed },
        { "", test_MySemanticDecFuncFunction_LegalFunctionDec_FormalsCanBeUsedInBody },

        { "", test_MySemanticDecs_DecsContainsIllegalDec_ReturnFalse },
        { "", test_MySemanticDecs_ConsecutiveSameFuncOrTypeDecs_ReturnFalse },
        { "", test_MySemanticDecs_NotConsecutiveSameFuncOrTypeDecs_ReturnTrue },
        { "", test_MySemanticDecs_ConsecutiveSameVarDecs_ReturnTrue },
        { "", test_MySemanticDecs_DecsContainsLegalDec_ReturnTrueAndDecsAdded },

        { "", test_MySemanticLetExp_IllegalDeclarations_ReturnSemanticError },
        { "", test_MySemanticLetExp_IllegalBody_ReturnSemanticError },
        { "", test_MySemanticLetExp_LegalExp_ReturnLastExpType },
        { "", test_MySemanticLetExp_LegalExp_VarInDecsCanBeUsedInExps },
        { "", test_MySemanticLetExp_LegalExp_TypesInDecsCanBeUsedInExps },
        { "", test_MySemanticLetExp_LegalExp_FuncsInDecsCanBeUsedInExps },

        { "", test_MySemanticExp_IllegalBreak_Failed },
        { "", test_MySemanticExp_LegalBreak_Succeed },
    };
    
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;
    if (!runTests(&suite))      
        return -1;

    freeMemoryChain();
    return 0;
}
