#include "../testHelper.h"

#include "../../internalForwards.h"
#include "../../mySemantic.h"
#include "../../myEnvironment.h"
#include "../../typeMaker.h"
#include "../../typeChecker.h"
#include "../../abstractSyntaxMaker.h"

#include <stdlib.h>
#include <stdio.h>
//  todo: change name *NULL to *SemanticError
///////////////////////////////////////////////////////////////////
//  help functions

mySymbol makeOneSymbol(void)
{
    return MySymbol_MakeSymbol("randomSymbol");
}

mySymbol makeSymbol_Int(void)
{
    return MySymbol_MakeSymbol("int");
}

mySymbol makeSymbol_String(void)
{
    return MySymbol_MakeSymbol("string");
}

myPos makeOnePos()
{
    myPos pos;
    pos.column = pos.line;
    return pos;
}

myExp makeOneExp_Integer(void)
{
    return makeMyExp_IntegerLiteral(
            makeOnePos(),
            makeMyIntegerLiteralExp(1));
}

myExp makeOneExp_String(void)
{
    return makeMyExp_StringLiteral(
            makeOnePos(),
            makeMyStringLiteralExp("1"));
}

myExp makeOneExp_Nil(void)
{
    return makeMyExp_Nil(makeOnePos(), makeMyNilExp());
}

myExp makeOneExp_Break(void)
{
    return makeMyExp_Break(makeOnePos(), makeMyBreakExp());
}

myExp makeOneExp_NoValue(void)
{
    return makeMyExp_NoValue(
        makeOnePos(), makeMyNoValueExp());
}

myExp makeOneExp_Record(mySymbol recordTypeName)
{
    return makeMyExp_RecordCreation(makeOnePos(),
        makeMyRecordCreationExp_NoField(
            makeMyNoFieldRecordCreationExp(recordTypeName)));
}

myExp makeOneIllegalExp_Integer(void)
{
    return makeMyExp_IfThenElse(makeOnePos(),
        makeMyIfThenElseExp(makeOneExp_String(),
            makeOneExp_Integer(), makeOneExp_Integer()));
}

myLValueExp makeOneLegalLValueExp_SimpleVar_Int(
    myTable varAndFuncEnv, myTable typeEnv)
{
    mySymbol variableName = makeOneSymbol();
    MySymbol_Enter(varAndFuncEnv, variableName, myEnvironment_makeVarEntry(makeType_Int()));

    return makeMyLValue(makeOnePos(), variableName, NULL);
}

myLValueExp makeOneLegalLValueExp_Record(
    myTable varAndFuncEnv, myType recordType)
{
    mySymbol variableName = makeOneSymbol();
    MySymbol_Enter(varAndFuncEnv, variableName, myEnvironment_makeVarEntry(recordType));

    return makeMyLValue(makeOnePos(), variableName, NULL);
}

myNoFieldRecordCreationExp makeOneNoFieldCreationExp(void)
{
    return makeMyNoFieldRecordCreationExp(MySymbol_MakeSymbol("record creation"));
}

myLValueExp makeOneLValueExp_TwoIntArraySubscript(
    mySymbol arrayName, myExp subscript1, myExp subscript2)
{
    return makeMyLValue(makeOnePos(), arrayName,
        makeMyLValueAux(NULL,
            subscript1,
            makeMyLValueAux(NULL,
            subscript2,
            NULL)));
}

myType makeOneArray_StringArrayArray(void)
{
    return makeType_Array(makeType_Array(makeType_String()));
}

myType makeAndAddOneType_NoFieldRecord(myTable typeEnv, mySymbol recordTypeName)
{
    myType recordType = makeType_Record(makeType_TypeFieldList(NULL, NULL));
    MyEnvironment_addNewType(typeEnv, recordTypeName, recordType);
    return recordType;
}

///////////////////////////////////////////////////////////////////////

void test_typeContainsLValueAux_CheckRecord_ReturnTypeOfOneField(void)
{
    mySymbol symbol1 = MySymbol_MakeSymbol("field1");
    mySymbol symbol2 = MySymbol_MakeSymbol("field2");
    mySymbol symbolRecord = MySymbol_MakeSymbol("recordd");

    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol1, makeType_Int()),
        makeType_TypeFieldList(
            makeType_TypeField(symbol2, makeType_String()),
            NULL))));
    MySymbol_Enter(varAndFuncEnv, symbolRecord, entry);

    myLValueAux aux1 = makeMyLValueAux(symbol1, NULL, NULL);
    myType typeReturn1 = typeContainsLValueAux(
        varAndFuncEnv, NULL, MyEnvironment_getVarType(entry), aux1);
    myLValueAux aux2 = makeMyLValueAux(symbol2, NULL, NULL);


    myType typeReturn2 = typeContainsLValueAux(
        varAndFuncEnv, NULL, MyEnvironment_getVarType(entry), aux2);
    
    CU_ASSERT(typeReturn1->kind == TypeInt);
    CU_ASSERT(typeReturn2->kind == TypeString);
}

void test_typeContainsLValueAux_CheckRecord_ReturnTypeOfNestedField(void)
{
    mySymbol symbol1 = MySymbol_MakeSymbol("field1");
    mySymbol symbol2 = MySymbol_MakeSymbol("field2");
    mySymbol symbolRecord = MySymbol_MakeSymbol("recordd");

    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myType nestedRecordType = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol2, makeType_String()), NULL));
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbol1, nestedRecordType), NULL)));
    MySymbol_Enter(varAndFuncEnv, symbolRecord, entry);

    myLValueAux aux = makeMyLValueAux(symbol1, NULL,
        makeMyLValueAux(symbol2, NULL, NULL));


    myType typeReturn = typeContainsLValueAux(
        varAndFuncEnv, NULL, MyEnvironment_getVarType(entry), aux);
    
    CU_ASSERT(typeReturn->kind == TypeString);
}

void test_typeContainsLValueAux_CheckArray_ReturnTypeOfOneField(void)
{
    mySymbol symbol = MySymbol_MakeSymbol("field1");
    mySymbol symbolArray = MySymbol_MakeSymbol("recordd");

    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Array(
        makeType_Int()));
    MySymbol_Enter(varAndFuncEnv, symbolArray, entry);

    myLValueAux aux = makeMyLValueAux(NULL, 
        makeOneExp_Integer(), NULL);


    myType typeReturn = typeContainsLValueAux(
        varAndFuncEnv, typeEnv, MyEnvironment_getVarType(entry), aux);
    
    CU_ASSERT(typeReturn->kind == TypeInt);
}

void test_typeContainsLValueAux_CheckArray_ReturnTypeOfNestedField(void)
{
    mySymbol symbolArray = MySymbol_MakeSymbol("recordd");

    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myType arrayType = makeType_Array(makeType_String());
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Array(arrayType));
    MySymbol_Enter(varAndFuncEnv, symbolArray, entry);

    myExp exp = makeOneExp_Integer();
    myLValueAux aux = makeMyLValueAux(NULL, 
        exp, makeMyLValueAux(NULL, exp, NULL));


    myType typeReturn = typeContainsLValueAux(
        varAndFuncEnv, typeEnv, MyEnvironment_getVarType(entry), aux);
    
    CU_ASSERT(typeReturn->kind == TypeString);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticLValueExpSimpleVar_VarNotDeclared_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myLValueExp simpleVarExp = makeMyLValue(
        makeOnePos(), makeOneSymbol(), NULL);

    myTranslationAndType result = MySemantic_LValueExp_SimpleVar(
        varAndFuncEnv, simpleVarExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpSimpleVar_LegalExp_ReturnTypeOfSimpleVar(void)
{
    mySymbol symbol = MySymbol_MakeSymbol("bolOne");
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Record(NULL));
    MySymbol_Enter(varAndFuncEnv, symbol, entry);

    myLValueExp exp = makeMyLValue(makeOnePos(), symbol, NULL);


    myTranslationAndType result =
        MySemantic_LValueExp_SimpleVar(varAndFuncEnv, exp);

    CU_ASSERT(isTypeRecord(result->type));
}

//////////////////////////////////////////////////////////////////////////////

void test_MySemanticLValueExpRecordField_VariableNotDeclared_ReturnNull(void)
{
    myTable varAndFuncenv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myLValueExp recordExp = makeMyLValue(makeOnePos(), makeOneSymbol(), 
        makeMyLValueAux(makeOneSymbol(), NULL, NULL));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(
        varAndFuncenv, typeEnv, recordExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpRecordField_VariableNotRecord_ReturnNull(void)
{
    myTable varAndFuncenv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol symbolRecord = MySymbol_MakeSymbol("record");
    mySymbol symbolField = MySymbol_MakeSymbol("field");

    myVarAndFuncEntry intEntry = myEnvironment_makeVarEntry(makeType_Int());
    MySymbol_Enter(varAndFuncenv, symbolRecord, intEntry);

    myLValueExp recordExp = makeMyLValue(makeOnePos(), symbolRecord, 
        makeMyLValueAux(symbolField, NULL, NULL));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(
        varAndFuncenv, typeEnv, recordExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpRecordField_FieldNamesNotMatch_ReturnNull(void)
{
    mySymbol symbolRecord = MySymbol_MakeSymbol("record");
    mySymbol symbolField = MySymbol_MakeSymbol("field");

    myTable variableSymbolTable = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbolField, makeType_Int()),
        NULL)));
    MySymbol_Enter(variableSymbolTable, symbolRecord, entry);

    mySymbol anotherField = MySymbol_MakeSymbol("anotherField");
    myLValueExp exp = makeMyLValue(makeOnePos(), symbolRecord, 
        makeMyLValueAux(anotherField, NULL, NULL));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(
        variableSymbolTable, typeEnv, exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpRecordField_LegalExp_ReturnTypeOfRecordField(void)
{
    mySymbol symbolRecord = MySymbol_MakeSymbol("record");
    mySymbol symbolField = MySymbol_MakeSymbol("field");

    myTable variableSymbolTable = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myType innerRecordType = makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbolField, makeType_Int()),
        NULL));
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Record(makeType_TypeFieldList(
        makeType_TypeField(symbolField, innerRecordType),
        NULL)));
    MySymbol_Enter(variableSymbolTable, symbolRecord, entry);

    myLValueExp exp = makeMyLValue(makeOnePos(), symbolRecord, 
        makeMyLValueAux(symbolField, NULL, 
            makeMyLValueAux(symbolField, NULL, NULL)));


    myTranslationAndType result = MySemantic_LValueExp_RecordField(
        variableSymbolTable, typeEnv, exp);

    CU_ASSERT(isTypeInt(result->type));
}

///////////////////////////////////////////////////////////////////////////////

 void test_MySemanticLValueExpArraySubscript_ArrayVarNotDeclared_ReturnNull(void)
 {
     myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
     myTable typeEnv = myEnvironment_BaseType();
     myLValueExp lValueExp = makeOneLValueExp_TwoIntArraySubscript(
         makeOneSymbol(), makeOneExp_Integer(), makeOneExp_Integer());

    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(
        varAndFuncEnv, typeEnv, lValueExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
 }

void test_MySemanticLValueExpArraySubscript_VarNotArrayType_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol arrayName = makeOneSymbol();
    myLValueExp lValueExp = makeOneLValueExp_TwoIntArraySubscript(
         arrayName, makeOneExp_Integer(), makeOneExp_Integer());

    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeType_Int()); 
    MySymbol_Enter(varAndFuncEnv, arrayName, entry);


    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(
        varAndFuncEnv, typeEnv, lValueExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpArraySubscript_SubscriptNotInt_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol arrayName = makeOneSymbol();
    myLValueExp lValueExp = makeOneLValueExp_TwoIntArraySubscript(
         arrayName, makeOneExp_Integer(), makeOneExp_String());

    myType arrayType = makeOneArray_StringArrayArray();
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(arrayType);
    MySymbol_Enter(varAndFuncEnv, arrayName, entry);
    MySymbol_Enter(typeEnv, arrayName, arrayType);


    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(
        varAndFuncEnv, typeEnv, lValueExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLValueExpArraySubscript_LegalExp_ReturnTypeOfArray(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    myTable variableSymbolTable = myEnvironment_BaseVarAndFunc();
    myVarAndFuncEntry entry = myEnvironment_makeVarEntry(makeOneArray_StringArrayArray());

    mySymbol symbolArray = MySymbol_MakeSymbol("array");
    MySymbol_Enter(variableSymbolTable, symbolArray, entry);
    MySymbol_Enter(typeEnv, symbolArray, MyEnvironment_getVarType(entry));

    myLValueExp exp = makeOneLValueExp_TwoIntArraySubscript(symbolArray,
        makeOneExp_Integer(), makeOneExp_Integer());


    myTranslationAndType result = MySemantic_LValueExp_ArraySubscript(
        variableSymbolTable, typeEnv, exp);

    CU_ASSERT(isTypeString(result->type));
}

///////////////////////////////////////////////////////////////////////////////

void test_MySemanticNilExp_ByDefault_ReturnNilType(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;

    myPos pos;  pos.column = pos.line = 0;
    myNilExp exp = makeMyNilExp();

    myTranslationAndType result = MySemantic_NilExp(varAndFuncEnv, typeEnv, exp);

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
        MySemantic_IntegerLiteralExp(varAndFuncEnv, typeEnv, exp);

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
        MySemantic_StringLiteralExp(varAndFuncEnv, typeEnv, exp);

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
        MySemantic_NoValueExp(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(isTypeNoReturn(result->type));
}

///////////////////////////////////////////////////////////////////////////////

void test_MySemanticBreakExp_ByDefault_ReturnNoReturnType(void)
{
    myTable varAndFuncEnv = NULL;
    myTable typeEnv = NULL;

    myPos pos;  pos.column = pos.line = 0;
    myBreakExp exp = makeMyBreakExp();

    myTranslationAndType result = 
        MySemantic_BreakExp(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(result != NULL && isTypeNoReturn(result->type));
}

//////////////////////////////////////////////////////////////////////////////////

void test_MySemanticFunctionCallExpParam_FunctionNotDefined_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myParamFunctionCallExp exp = makeMyParamFunctionCallExp(
        makeOneSymbol(),
        makeMyExpList(
            makeOneExp_Integer(),
            makeMyExpList(makeOneExp_String(),NULL)));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        varAndFuncEnv, typeEnv, exp);

    
    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticFunctionCallExpParam_ParamsTypeNotMatch_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol functionSymbol = MySymbol_MakeSymbol("function");
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        makeType_TypeList(
            makeType_String(),
            makeType_TypeList(makeType_String(), NULL)),
        makeType_Int());
    MySymbol_Enter(varAndFuncEnv, functionSymbol, functionEntry);

    myParamFunctionCallExp functionExp = makeMyParamFunctionCallExp(
        functionSymbol,
        makeMyExpList(
            makeOneExp_Integer(),
            makeMyExpList(makeOneExp_String(),NULL)));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        varAndFuncEnv, typeEnv, functionExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticFunctionCallExpParam_LegalExp_ReturnTypeOfFunctionResult(void)
{ 
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol functionSymbol = MySymbol_MakeSymbol("function");
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        makeType_TypeList(
            makeType_Int(),
            makeType_TypeList(makeType_String(), NULL)),
        makeType_Int());
    MySymbol_Enter(varAndFuncEnv, functionSymbol, functionEntry);

    myParamFunctionCallExp exp = makeMyParamFunctionCallExp(
        functionSymbol,
        makeMyExpList(
            makeOneExp_Integer(),
            makeMyExpList(makeOneExp_String(),NULL)));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        varAndFuncEnv, typeEnv, exp);

    
    CU_ASSERT(result != SEMANTIC_ERROR && isTypeInt(result->type));
}

void test_MySemanticFunctionCallExpParam_NilValueOfRecordParamType_Succeed(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol recordTypeName = MySymbol_MakeSymbol("record type");
    myType recordType = makeAndAddOneType_NoFieldRecord(typeEnv, recordTypeName);

    mySymbol functionSymbol = MySymbol_MakeSymbol("function");
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        makeType_TypeList(
            makeType_Record(makeType_TypeFieldList(
                makeType_TypeField(makeOneSymbol(), recordType),
                NULL)),
            NULL),
        makeType_Int());
    MySymbol_Enter(varAndFuncEnv, functionSymbol, functionEntry);

    myParamFunctionCallExp exp = makeMyParamFunctionCallExp(
        functionSymbol,
        makeMyExpList(
            makeOneExp_Nil(), NULL));


    myTranslationAndType result = MySemantic_FunctionCallExp_Param(
        varAndFuncEnv, typeEnv, exp);

    
    CU_ASSERT(result != SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////////////

void test_MySemanticFunctionCallExpNoParam_FunctionNotDefined_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myNoParamFunctionCallExp exp = makeMyNoParamFunctionCallExp(
        makeOneSymbol());

    myTranslationAndType result = MySemantic_FunctionCallExp_NoParam(
        varAndFuncEnv, typeEnv, exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticFunctionCallExpNoParam_LegalExp_ReturnTypeOfFunctionResult(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol functionSymbol = MySymbol_MakeSymbol("function"); 
    myVarAndFuncEntry functionEntry = myEnvironment_makeFuncEntry(
        NULL, makeType_Int());
    MySymbol_Enter(varAndFuncEnv, functionSymbol, functionEntry);

    myNoParamFunctionCallExp exp = makeMyNoParamFunctionCallExp(
        functionSymbol);

    myTranslationAndType result = MySemantic_FunctionCallExp_NoParam(
        varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(result != SEMANTIC_ERROR && isTypeInt(result->type));
}

/////////////////////////////////////////////////////////////////////////////

//  test template for illegal `ArrayCreationExp`
void test_IllegalIntArrayCreation_ReturnNull(myExp length, myExp initial);

/////////////////////////////

void test_MySemanticArrayCreationExp_ArrayTypeNotDefined_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myArrayCreationExp arrayCreationExp = makeMyArrayCreationExp(
        makeOneSymbol(), makeOneExp_Integer(), makeOneExp_Integer());


    myTranslationAndType result = MySemantic_ArrayCreationExp(
        varAndFuncEnv, typeEnv, arrayCreationExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticArrayCreationExp_OnlyLengthPartIllegal_ReturnNull(void)
{
    myExp lengthExpNotInt = makeOneExp_String();
    myExp legalInitialExp = makeOneExp_Integer();

    test_IllegalIntArrayCreation_ReturnNull(lengthExpNotInt, legalInitialExp);
}

void test_MySemanticArrayCreationExp_OnlyInitialPartIllegal_ReturnNull(void)
{
    myExp legalLength = makeOneExp_Integer();
    myExp initialExpNotArrayElementType = makeOneExp_NoValue();

    test_IllegalIntArrayCreation_ReturnNull(legalLength, initialExpNotArrayElementType);
}

void test_MySemanticArrayCreationExp_LegalExp_ReturnArrayType(void)
{
    mySymbol arrayName = MySymbol_MakeSymbol("array");

    myExp length = makeOneExp_Integer();
    myExp initial = makeOneExp_Integer();
    myArrayCreationExp exp = makeMyArrayCreationExp(arrayName, length, initial);

    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myType arrayType = makeType_Array(makeType_Int());
    MySymbol_Enter(typeEnv, arrayName, arrayType);

    myTranslationAndType result = 
        MySemantic_ArrayCreationExp(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(result != SEMANTIC_ERROR && isTypeEqual(result->type, arrayType));
}

void test_MySemanticArrayCreationExp_NilInitialValueOfRecordElement_Succeed(void)
{
    mySymbol arrayName = MySymbol_MakeSymbol("array");
    myArrayCreationExp nilValueExp = makeMyArrayCreationExp(
        arrayName, makeOneExp_Integer(), makeOneExp_Nil());

    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myType arrayType = makeType_Array(
        makeAndAddOneType_NoFieldRecord(typeEnv, makeOneSymbol()));
    MySymbol_Enter(typeEnv, arrayName, arrayType);


    myTranslationAndType result = 
        MySemantic_ArrayCreationExp(varAndFuncEnv, typeEnv, nilValueExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalIntArrayCreation_ReturnNull(myExp length, myExp initial)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol arrayTypeName = MySymbol_MakeSymbol("array");
    myArrayCreationExp exp = makeMyArrayCreationExp(arrayTypeName, length, initial);

    myType arrayType = makeType_Array(makeType_Int());
    MySymbol_Enter(typeEnv, arrayTypeName, arrayType);


    myTranslationAndType result = MySemantic_ArrayCreationExp(
        varAndFuncEnv, typeEnv, exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticRecordCreationExpNoField_RecordTypeNotDefined_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myNoFieldRecordCreationExp noFieldRecordCreationExp = 
        makeOneNoFieldCreationExp();

    myTranslationAndType result =
        MySemantic_RecordCreationExp_NoField(varAndFuncEnv, typeEnv,
            noFieldRecordCreationExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticRecordCreationExpNoField_TypeNotRecord_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    MySymbol_Enter(typeEnv, recordTypeName, makeType_Int());

    myNoFieldRecordCreationExp noFieldRecordCreationExp =
        makeMyNoFieldRecordCreationExp(recordTypeName);

    
    myTranslationAndType result = MySemantic_RecordCreationExp_NoField(
        varAndFuncEnv, typeEnv, noFieldRecordCreationExp);

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
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    MySymbol_Enter(typeEnv, recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_NoField(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(isTypeEqual(result->type, recordType));    
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticRecordCreationExpField_TypeNotRecord_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol fieldName = MySymbol_MakeSymbol("field");
    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(fieldName, makeOneExp_Integer()),
                NULL));

    MySymbol_Enter(typeEnv, recordTypeName, makeType_Int());

    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticRecordCreationExpField_RecordTypeNotDefined_ReturnNull(void)
{
    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol fieldName = MySymbol_MakeSymbol("field");

    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(fieldName, makeOneExp_Integer()),
                NULL));

    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);   
}

void test_MySemanticRecordCreationExpField_RecordFieldsNotMatch_ReturnNull(void)
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
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    MySymbol_Enter(typeEnv, recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(varAndFuncEnv, typeEnv, exp);

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
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    MySymbol_Enter(typeEnv, recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(isTypeEqual(result->type, recordType));   
}

void test_MySemanticRecordCreationExpField_NilValueOfRecordField_Succeed(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol recordTypeName = MySymbol_MakeSymbol("FieldRecord");
    mySymbol fieldName = MySymbol_MakeSymbol("field");
    mySymbol anotherRecordTypeName = MySymbol_MakeSymbol("fieldfield");
    myType anotherRecordType = makeAndAddOneType_NoFieldRecord(
        typeEnv, anotherRecordTypeName);

    myFieldRecordCreationExp exp = 
        makeMyFieldRecordCreationExp(
            recordTypeName,
            makeMyRecordFieldList(
                makeMyRecordField(fieldName, makeOneExp_Nil()),
                NULL));
    myType recordType = makeType_Record(makeType_TypeFieldList(
            makeType_TypeField(fieldName, anotherRecordType),
            NULL));
    MySymbol_Enter(typeEnv, recordTypeName, recordType);


    myTranslationAndType result = 
        MySemantic_RecordCreationExp_Field(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(result != SEMANTIC_ERROR);   
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticArithmeticExp_IllegalExp_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myArithmeticExp incorrectLeftOperand = makeMyArithmeticExp_Divide(
        makeOneExp_Integer(), makeOneExp_String());
    myArithmeticExp incorrectRightOperand = makeMyArithmeticExp_Divide(
        makeOneExp_String(), makeOneExp_Integer());
    myArithmeticExp incorrectBothOperands = makeMyArithmeticExp_Divide(
        makeOneExp_NoValue(), makeOneExp_String());

    myTranslationAndType resultLeftOperand = 
        MySemantic_ArithmeticExp(varAndFuncEnv, typeEnv, incorrectLeftOperand);
    myTranslationAndType resultRightOperand = 
        MySemantic_ArithmeticExp(varAndFuncEnv, typeEnv, incorrectRightOperand);
    myTranslationAndType resultBothOperands = 
        MySemantic_ArithmeticExp(varAndFuncEnv, typeEnv, incorrectBothOperands);

    CU_ASSERT_EQUAL(resultLeftOperand, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultRightOperand, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultBothOperands, SEMANTIC_ERROR);
}

void test_MySemanticArithmeticExp_LegalExp_ReturnIntType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myArithmeticExp correctExp = makeMyArithmeticExp_Divide(
        makeOneExp_Integer(), makeOneExp_Integer());
    
    myTranslationAndType correctResult = 
        MySemantic_ArithmeticExp(varAndFuncEnv, typeEnv, correctExp);

    CU_ASSERT(correctResult != SEMANTIC_ERROR && isTypeInt(correctResult->type));
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticParenthesesExp_BeDefault_ReturnInnerResult(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myIntegerLiteralExp innerExp = makeMyIntegerLiteralExp(1);
    myParenthesesExp exp = makeMyParenthesesExp(
        makeMyExp_IntegerLiteral(makeOnePos(), innerExp));

    myTranslationAndType result =
        MySemantic_ParenthesesExp(varAndFuncEnv, typeEnv, exp);
    myTranslationAndType resultInner =
        MySemantic_IntegerLiteralExp(varAndFuncEnv, typeEnv, innerExp);

    CU_ASSERT(isTypeEqual(resultInner->type, result->type)); 
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticSequencingExp_AnyOneIllegal_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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
        MySemantic_SequencingExp(varAndFuncEnv, typeEnv, illegalOne);
    myTranslationAndType resultTwo =
        MySemantic_SequencingExp(varAndFuncEnv, typeEnv, illegalTwo);
    myTranslationAndType resultThree =
        MySemantic_SequencingExp(varAndFuncEnv, typeEnv, illegalThree);

    CU_ASSERT_EQUAL(resultOne, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultTwo, SEMANTIC_ERROR);
    CU_ASSERT_EQUAL(resultThree, SEMANTIC_ERROR);
}

void test_MySemanticSequencingExp_LegalExp_ReturnLastExpressionResult(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySequencingExp expOne = makeMySequencingExp(
        makeOneExp_Integer(), makeOneExp_Integer(), NULL);
    mySequencingExp expTwo = makeMySequencingExp(
        makeOneExp_Integer(), makeOneExp_String(),
        makeMyExpList(
            makeOneExp_String(),
            makeMyExpList(makeOneExp_String(), NULL)));
    
    myTranslationAndType resultOne = 
        MySemantic_SequencingExp(varAndFuncEnv, typeEnv, expOne);
    myTranslationAndType resultTwo = 
        MySemantic_SequencingExp(varAndFuncEnv, typeEnv, expTwo);
    
    CU_ASSERT(isTypeInt(resultOne->type));
    CU_ASSERT(isTypeString(resultTwo->type));
}

//////////////////////////////////////////////////////////////////////////////

//  forward declarations
void test_IllegalForExp_ReturnNull(myExp low, myExp high, myExp body);

////////////////////

void test_MySemanticForExp_OnlyLowRangeNotInt_ReturnNull(void)
{
    myExp illegalLow = makeOneExp_NoValue();
    myExp legalHigh = makeOneExp_Integer();
    myExp legalBody = makeOneExp_NoValue();

    test_IllegalForExp_ReturnNull(illegalLow, legalHigh, legalBody);
}

void test_MySemanticForExp_OnlyHighRangeNotInt_ReturnNull(void)
{
    myExp legalLow = makeOneExp_Integer();
    myExp illegalHigh = makeOneExp_NoValue();
    myExp legalBody = makeOneExp_NoValue();

    test_IllegalForExp_ReturnNull(legalLow, illegalHigh, legalBody);
}

void test_MySemanticForExp_OnlybodyNotTypeNoReturn_ReturnNullt(void)
{
    myExp legalLow = makeOneExp_Integer();
    myExp legalHigh = makeOneExp_Integer();
    myExp illegalBody = makeOneExp_Integer();

    test_IllegalForExp_ReturnNull(legalLow, legalHigh, illegalBody);
}

void test_MySemanticForExp_LegalForExp_ReturnTypeNoReturn(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol forVarName = MySymbol_MakeSymbol("var"); 
    myForExp forExp = makeMyForExp(
        forVarName,
        makeOneExp_Integer(),
        makeOneExp_Integer(),
        makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_ForExp(varAndFuncEnv, typeEnv, forExp);
    
    CU_ASSERT(isTypeNoReturn(result->type));
}

void test_MySemanticForExp_LegalForExp_ConditionVarCanBeUsedInBody(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol forVarName = MySymbol_MakeSymbol("var"); 
    myForExp forExp = makeMyForExp(
        forVarName,
        makeOneExp_Integer(),
        makeOneExp_Integer(),
        makeMyExp_Assignment(makeOnePos(), 
            makeMyAssignmentExp(
                makeMyLValue(makeOnePos(), forVarName, NULL),
                makeOneExp_Integer())));

    myTranslationAndType result =
        MySemantic_ForExp(varAndFuncEnv, typeEnv, forExp);
    
    CU_ASSERT_NOT_EQUAL(result, SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalForExp_ReturnNull(myExp low, myExp high, myExp body)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol forVarName = MySymbol_MakeSymbol("var"); 
    myForExp forExp = makeMyForExp(forVarName, low, high, body);

    myTranslationAndType result =
        MySemantic_ForExp(varAndFuncEnv, typeEnv, forExp);
    
    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

//////////////////////////////////////////////////////////////////////////////

//  a parameterized tester for illegal IfThenElseExp
void test_IllegalIfThenElseExp_ReturnNull(myExp ifExp, myExp thenExp, myExp elseExp);

//////////////////

void test_MySemanticIfThenElseExp_OnlyIfClauseNotInt_ReturnNull(void)
{
    myExp illegalIf = makeOneExp_NoValue();
    myExp legalThen = makeOneExp_Integer();
    myExp legalElse = makeOneExp_Integer();

    test_IllegalIfThenElseExp_ReturnNull(illegalIf, legalThen, legalElse);
}

void test_MySemanticIfThenElseExp_OnlyThenClauseNotInt_ReturnNull(void)
{
    myExp legalIf = makeOneExp_Integer();
    myExp illegalThen = makeMyExp_IfThenElse(makeOnePos(),
        makeMyIfThenElseExp(
            makeOneExp_NoValue(), makeOneExp_String(), makeOneExp_String()));
    myExp legalElse = makeOneExp_Integer();

    test_IllegalIfThenElseExp_ReturnNull(legalIf, illegalThen, legalElse);
}

void test_MySemanticIfThenElseExp_OnlyElseClauseNotInt_ReturnNull(void)
{
    myExp legalIf = makeOneExp_Integer();
    myExp legalThen = makeOneExp_Integer();
    myExp illegalElse= makeMyExp_IfThenElse(makeOnePos(),
        makeMyIfThenElseExp(
            makeOneExp_NoValue(), makeOneExp_String(), makeOneExp_String()));

    test_IllegalIfThenElseExp_ReturnNull(legalIf, legalThen, illegalElse);
}

void test_MySemanticIfThenElseExp_OnlyThenElseTypeMismatch_ReturnNull(void)
{
    myExp legalIf = makeOneExp_Integer();
    myExp intThen = makeOneExp_Integer();
    myExp stringElse = makeOneExp_String();

    test_IllegalIfThenElseExp_ReturnNull(legalIf, intThen, stringElse);
}

void test_MySemanticIfThenElseExp_LegalExpression_ReturnClauseType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myIfThenElseExp exp = makeMyIfThenElseExp(
        makeOneExp_Integer(), makeOneExp_String(), makeOneExp_String());

    myTranslationAndType result = 
        MySemantic_IfThenElseExp(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(isTypeString(result->type));
}

void test_MySemanticIfThenElseExp_OneNilAnotherRecord_Succeed(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol recordTypeName = MySymbol_MakeSymbol("record type");
    myType recordType = makeAndAddOneType_NoFieldRecord(typeEnv, recordTypeName);

    myIfThenElseExp expOne = makeMyIfThenElseExp(
        makeOneExp_Integer(), makeOneExp_Record(recordTypeName), makeOneExp_Nil());
    myIfThenElseExp expTwo = makeMyIfThenElseExp(
        makeOneExp_Integer(), makeOneExp_Nil(), makeOneExp_Record(recordTypeName));

    myTranslationAndType resultOne = 
        MySemantic_IfThenElseExp(varAndFuncEnv, typeEnv, expOne);
    myTranslationAndType resultTwo = 
        MySemantic_IfThenElseExp(varAndFuncEnv, typeEnv, expTwo);

    CU_ASSERT(resultOne != SEMANTIC_ERROR);
    CU_ASSERT(resultTwo != SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalIfThenElseExp_ReturnNull(myExp ifExp, myExp thenExp, myExp elseExp)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myIfThenElseExp exp = makeMyIfThenElseExp(ifExp, thenExp, elseExp);

    myTranslationAndType result = 
        MySemantic_IfThenElseExp(varAndFuncEnv, typeEnv, exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

///////////////////////////////////////////////////////////////////////////////

//  a template test
void test_IllegalIfThenExp_ReturnNull(myExp ifExp, myExp thenExp);

//////////////////////////////

void test_MySemanticIfThenExp_OnlyIfconditionNotInt_ReturnNull(void)
{
    myExp noReturnIfCondition= makeOneExp_NoValue();
    myExp legalThen = makeOneExp_NoValue();

    test_IllegalIfThenExp_ReturnNull(noReturnIfCondition, legalThen);
}

void test_MySemanticIfThenExp_OnlyThenClauseNotNoReturn_ReturnNull(void)
{
    myExp legalIfCondition = makeOneExp_Integer();
    myExp intTypeThenClause = makeOneExp_Integer();

    test_IllegalIfThenExp_ReturnNull(legalIfCondition, intTypeThenClause);
}

void test_MySemanticIfThenExp_LegalExp_ReturnNoReturnType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myIfThenExp ifThenExp = makeMyIfThenExp(
        makeOneExp_Integer(), makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_IfThenExp(varAndFuncEnv, typeEnv, ifThenExp);

    CU_ASSERT(isTypeNoReturn(result->type));
}

//  a parameterized test
void test_IllegalIfThenExp_ReturnNull(myExp ifExp, myExp thenExp)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myIfThenExp ifThenExp = makeMyIfThenExp(ifExp, thenExp);

    myTranslationAndType result =
        MySemantic_IfThenExp(varAndFuncEnv, typeEnv, ifThenExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

//  a parameterized test
void test_IllegalComparisonExp_ReturnNull(myExp lhs, myExp rhs);

///////////////////

void test_MySemanticComparisonExp_IllegalLeftOperand_ReturnNull(void)
{
    myExp leftIllegalOperand = makeOneIllegalExp_Integer();
    myExp rightLegalOperand = makeOneExp_Integer();

    test_IllegalComparisonExp_ReturnNull(leftIllegalOperand, rightLegalOperand);
}

void test_MySemanticComparisonExp_IllegalRightOperand_ReturnNull(void)
{
    myExp leftLegalOperand = makeOneExp_Integer();
    myExp rightIllegalOperand = makeOneIllegalExp_Integer();

    test_IllegalComparisonExp_ReturnNull(leftLegalOperand, rightIllegalOperand);
}

void test_MySemanticComparisonExp_AnyOperandsTypeNotInt_ReturnNull(void)
{
    myExp intOperand = makeOneExp_Integer();
    myExp breakOperand = makeOneExp_Break();

    test_IllegalComparisonExp_ReturnNull(intOperand, breakOperand);
    test_IllegalComparisonExp_ReturnNull(breakOperand, intOperand);
}

void test_MySemanticComparisonExp_LegalExp_ReturnIntType(void)
{
    myTable varAndFuncenv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myComparisonExp intComparison = makeMyComparisonExp_Equal(
        makeOneExp_Integer(), makeOneExp_Integer());
    
    myTranslationAndType result = MySemantic_ComparisonExp(
        varAndFuncenv, typeEnv, intComparison);

    CU_ASSERT(isTypeInt(result->type));
}

void test_MySemanticComparisonExp_OneRecordAnotherNil_Succeed(void)
{
    myTable varAndFuncenv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol recordTypeName = makeOneSymbol();
    makeAndAddOneType_NoFieldRecord(typeEnv, recordTypeName);

    myComparisonExp intComparisonOne = makeMyComparisonExp_Equal(
       makeOneExp_Record(recordTypeName) , makeOneExp_Nil());
    myComparisonExp intComparisonTwo = makeMyComparisonExp_Equal(
       makeOneExp_Nil(), makeOneExp_Record(recordTypeName));
    
    myTranslationAndType resultOne = MySemantic_ComparisonExp(
        varAndFuncenv, typeEnv, intComparisonOne);
    myTranslationAndType resultTwo = MySemantic_ComparisonExp(
        varAndFuncenv, typeEnv, intComparisonTwo);

    CU_ASSERT(resultOne != SEMANTIC_ERROR);
    CU_ASSERT(resultTwo != SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalComparisonExp_ReturnNull(myExp lhs, myExp rhs)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myComparisonExp comparisonExp = makeMyComparisonExp_Equal(lhs, rhs);

    myTranslationAndType result = MySemantic_ComparisonExp(
        varAndFuncEnv, typeEnv, comparisonExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

//  a template test
void test_IllegalBooleanOperateExp_ReturnNull(
    myExp leftOperand, myExp rightOperand);

///////////////

void test_MySemanticBooleanOperateExp_OnlyLeftOperandNotInt_ReturnNull(void)
{
    myExp stringLeftOperand = makeOneExp_String();
    myExp intRightOperand =  makeOneExp_Integer();

    test_IllegalBooleanOperateExp_ReturnNull(stringLeftOperand, intRightOperand);
}

void test_MySemanticBooleanOperateExp_OnlyRightOperandNotInt_ReturnNull(void)
{
    myExp intLeftOperand = makeOneExp_Integer();
    myExp stringRightOperand =  makeOneExp_String();

    test_IllegalBooleanOperateExp_ReturnNull(intLeftOperand, stringRightOperand);
}

void test_MySemanticBooleanOperateExp_LegalExp_ReturnIntType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myBooleanOperateExp exp = makeMyBooleanOperateExp_Or(
        makeOneExp_Integer(), makeOneExp_Integer());

    myTranslationAndType result = MySemantic_BooleanOperateExp(
        varAndFuncEnv, typeEnv, exp);

    CU_ASSERT(isTypeInt(result->type));
}

//  a parameterized test
void test_IllegalBooleanOperateExp_ReturnNull(
    myExp leftOperand, myExp rightOperand)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myBooleanOperateExp exp = makeMyBooleanOperateExp_Or(
        leftOperand, rightOperand);

    myTranslationAndType result = MySemantic_BooleanOperateExp(
        varAndFuncEnv, typeEnv, exp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

//  forwards
void test_IllegalAssignmentExp_ReturnNull(
    myTable varAndFuncEnv, myTable typeEnv,
    myLValueExp leftOperand, myExp rightOperand);

////////////////////

void test_MySemanticAssignmentExp_OnlyLeftOperandIllegal_ReturnNull(void)
{
    myLValueExp illegalLeftOperand =
        makeMyLValue(makeOnePos(), makeOneSymbol(), NULL);
    myExp legalRightOperand = makeOneExp_Integer();

    test_IllegalAssignmentExp_ReturnNull(
        myEnvironment_BaseVarAndFunc(),
        myEnvironment_BaseType(),
        illegalLeftOperand, legalRightOperand);
}



void test_MySemanticAssignmentExp_OnlyRightOperandIllegal_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myLValueExp legalLeftOperand =
        makeOneLegalLValueExp_SimpleVar_Int(varAndFuncEnv, typeEnv);
    myExp illegalRightOperand = makeOneIllegalExp_Integer();

    test_IllegalAssignmentExp_ReturnNull(
        varAndFuncEnv, typeEnv, legalLeftOperand, illegalRightOperand);    
}

void test_MySemanticAssignmentExp_OperandsTypeNotMatch_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myLValueExp intLeftOperand =
        makeOneLegalLValueExp_SimpleVar_Int(varAndFuncEnv, typeEnv); 
    myExp stringRightOperand = makeOneExp_String();

    test_IllegalAssignmentExp_ReturnNull(
        varAndFuncEnv, typeEnv, 
        intLeftOperand, stringRightOperand);
}

void test_MySemanticAssignmentExp_LegalExp_ReturnNoReturnType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myAssignmentExp assignmentExp =
        makeMyAssignmentExp(
            makeOneLegalLValueExp_SimpleVar_Int(varAndFuncEnv, typeEnv),
            makeOneExp_Integer());

    myTranslationAndType result = MySemantic_AssignmentExp(
        varAndFuncEnv, typeEnv, assignmentExp);

    CU_ASSERT(isTypeNoReturn(result->type));
}

void test_MySemanticAssignmentExp_LeftRecordRightNil_Succeed(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myAssignmentExp assignmentExp =
        makeMyAssignmentExp(
            makeOneLegalLValueExp_Record(
                varAndFuncEnv, 
                makeAndAddOneType_NoFieldRecord(typeEnv, makeOneSymbol())),    
            makeOneExp_Nil());


    myTranslationAndType result = MySemantic_AssignmentExp(
        varAndFuncEnv, typeEnv, assignmentExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
}

//  a parameterized test
void test_IllegalAssignmentExp_ReturnNull(
    myTable varAndFuncEnv, myTable typeEnv,
    myLValueExp leftOperand, myExp rightOperand)
{
    myAssignmentExp assignmentExp = makeMyAssignmentExp(
        leftOperand, rightOperand);

    myTranslationAndType result = MySemantic_AssignmentExp(
        varAndFuncEnv, typeEnv, assignmentExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

////////////////////////////////////////////////////////////////////////////

//  a parameterized test
void test_IllegalWhileExp_ReturnNull(myExp conditionExp, myExp bodyExp);

//////////////////////

void test_MySemanticWhileExp_OnlyConditionNotInt_ReturnNull(void)
{
    myExp noReturnCondition = makeOneExp_NoValue();
    myExp noReturnBody = makeOneExp_NoValue();

    test_IllegalWhileExp_ReturnNull(noReturnCondition, noReturnBody);
}

void test_MySemanticWhileExp_OnlyBodyNotNoReturn_ReturnNull(void)
{
    myExp intCondition = makeOneExp_Integer();
    myExp notNoReturnBody = makeOneExp_Integer();

    test_IllegalWhileExp_ReturnNull(intCondition, notNoReturnBody);
}

void test_MySemanticWhileExp_LegalExp_ReturnNoReturnType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myWhileExp whileExp = makeMyWhileExp(
        makeOneExp_Integer(), makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_WhileExp(varAndFuncEnv, typeEnv, whileExp);

    CU_ASSERT(isTypeNoReturn(result->type));
}

//  a parameterized test
void test_IllegalWhileExp_ReturnNull(myExp conditionExp, myExp bodyExp)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myWhileExp whileExp = makeMyWhileExp(conditionExp, bodyExp);

    myTranslationAndType result =
        MySemantic_WhileExp(varAndFuncEnv, typeEnv, whileExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticNegationExp_ExpNotInt_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myNegationExp negationExp = makeMyNegationExp(makeOneExp_NoValue());

    myTranslationAndType result =
        MySemantic_NegationExp(varAndFuncEnv, typeEnv, negationExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticNegationExp_LegalExp_ReturnIntType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myNegationExp negationExp = makeMyNegationExp(makeOneExp_Integer());

    myTranslationAndType result =
        MySemantic_NegationExp(varAndFuncEnv, typeEnv, negationExp);

    CU_ASSERT(isTypeInt(result->type));
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeNamed_NamedTypeNotDefined_TypeNotAdd(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol undefinedTypeName = MySymbol_MakeSymbol("undefined");
    mySymbol typeName = makeOneSymbol(); 

    bool result = MySemantic_Dec_Type_Named(
        typeEnv, typeName , undefinedTypeName);

    bool typeNotAdd = MyEnvironment_getTypeFromName(typeEnv, typeName) == NULL; 
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT(typeNotAdd);
}

void test_MySemanticDecTypeNamed_NamedTypeOfBuildInt_AddedTypeIsInt(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol newTypeName = MySymbol_MakeSymbol("newType");
    mySymbol buildIntTypeName = MySymbol_MakeSymbol("int");

    bool result =MySemantic_Dec_Type_Named(typeEnv, newTypeName, buildIntTypeName);

    myType expectedType = MyEnvironment_getTypeFromName(typeEnv, buildIntTypeName);
    myType actualType = MyEnvironment_getTypeFromName(typeEnv, newTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(expectedType, actualType);
}

void test_MySemanticDecTypeNamed_NamedTypeOfNamedOne_AddedTypeIsNamedActualType(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol namedTypeName = MySymbol_MakeSymbol("namedType");
    mySymbol buildIntTypeName = MySymbol_MakeSymbol("int");
    MySemantic_Dec_Type_Named(typeEnv, namedTypeName, buildIntTypeName);

    mySymbol newTypeName = MySymbol_MakeSymbol("newType");
    bool result = MySemantic_Dec_Type_Named(typeEnv, newTypeName, namedTypeName);

    myType actualType = MyEnvironment_getTypeFromName(typeEnv, newTypeName);
    myType expectedType = MyEnvironment_getTypeFromName(typeEnv, buildIntTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(expectedType, actualType);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeRecord_FieldTypeNotDefined_TypeNotAdd(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol varName = makeOneSymbol();
    mySymbol typeName = MySymbol_MakeSymbol("not defined type");
    mySymbol recordTypeName = makeOneSymbol();
    myTyFieldList tyFields = makeMyTyFieldList(makeMyTyField(varName, typeName), NULL);

    bool result = MySemantic_Dec_Type_Record(typeEnv, recordTypeName, tyFields);

    bool recordNotAdd = MyEnvironment_getTypeFromName(typeEnv, recordTypeName) == NULL; 
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT(recordNotAdd);
}

void test_MySemanticDecTypeRecord_LegalParam_AddOneRecordType(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), MySymbol_MakeSymbol("int")),
        NULL);
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");

    bool result = MySemantic_Dec_Type_Record(typeEnv, recordTypeName, tyFields);

    bool recordAdded = MyEnvironment_getTypeFromName(typeEnv, recordTypeName) != NULL; 
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(recordAdded);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeArray_ElementTypeNotDefined_TypeNotAdd(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("elementType");

    bool result = MySemantic_Dec_Type_Array(typeEnv, newTypeName, elementTypeName);

    bool typeNotAdd = MyEnvironment_getTypeFromName(typeEnv, newTypeName) == NULL;
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT(typeNotAdd);
}

void test_MySemanticDecTypeArray_LegalParam_TypeAdded(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("int");

    bool result = MySemantic_Dec_Type_Array(typeEnv, newTypeName, elementTypeName);

    bool typeAdded = MyEnvironment_getTypeFromName(typeEnv, newTypeName) != NULL;
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
    myTable typeEnv = myEnvironment_BaseType();
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    mySymbol typeName = makeOneSymbol(); 
    myShortFormVar legalShortVar = makeMyShortFormVar(
        typeName, makeOneExp_Integer());

    bool result = 
        MySemantic_Dec_Var_ShortForm(varAndFuncEnv, typeEnv, legalShortVar);

    myType addedType = MyEnvironment_getVarType(
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, typeName));
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeInt(addedType));
}

//  a parameterized test
void test_IllegalShortFormVar_ReturnFalse(myShortFormVar var)
{
    myTable varAndFncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    bool result = MySemantic_Dec_Var_ShortForm(varAndFncEnv, typeEnv, var);

    CU_ASSERT_EQUAL(result, false);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecVarLongForm_VariableTypeNotDefined_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), makeOneSymbol(), makeOneExp_Integer());

    bool result =
        MySemantic_Dec_Var_LongForm(varAndFuncEnv, typeEnv, longFormVar);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecVarLongForm_VariableTypeNotMatchExpType_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), MySymbol_MakeSymbol("int"), makeOneExp_String());

    bool result =
        MySemantic_Dec_Var_LongForm(varAndFuncEnv, typeEnv, longFormVar);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecVarLongForm_NilValueOfNonRecordType_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), MySymbol_MakeSymbol("int"), makeOneExp_Nil());

    bool result =
        MySemantic_Dec_Var_LongForm(varAndFuncEnv, typeEnv, longFormVar);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecVarLongForm_NilValueOfRecordType_VarAdded(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeAndAddOneType_NoFieldRecord(typeEnv, recordTypeName);

    mySymbol varName = makeOneSymbol();
    myLongFormVar longFormVar = makeMyLongFormVar(
        varName , recordTypeName, makeOneExp_Nil());

    bool result =
        MySemantic_Dec_Var_LongForm(varAndFuncEnv, typeEnv, longFormVar);

    myVarAndFuncEntry varEntry = MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, varName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isVarEntry(varEntry));
    CU_ASSERT(isTypeEqual(MyEnvironment_getVarType(varEntry), recordType));
}

void test_MySemanticDecVarLongForm_LegalParam_VarAdded(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol varName = makeOneSymbol(); 
    myLongFormVar longFormVar = makeMyLongFormVar(
        varName, MySymbol_MakeSymbol("int"), makeOneExp_Integer());

    bool result =
        MySemantic_Dec_Var_LongForm(varAndFuncEnv, typeEnv, longFormVar);

    myVarAndFuncEntry varEntry = MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, varName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isVarEntry(varEntry));
    CU_ASSERT(isTypeInt(MyEnvironment_getVarType(varEntry)));
}

void test_MySemanticDecVarLongForm_NilValueOfRecordType_Succeed(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol recordTypeName = MySymbol_MakeSymbol("typeee");
    makeAndAddOneType_NoFieldRecord(typeEnv, recordTypeName);
    myLongFormVar longFormVar = makeMyLongFormVar(
        makeOneSymbol(), recordTypeName, makeOneExp_Nil());

    bool result =
        MySemantic_Dec_Var_LongForm(varAndFuncEnv, typeEnv, longFormVar);

    CU_ASSERT_EQUAL(result, true);
}

/////////////////////////////////////////////////////////////////////////////

//  forwards
void test_IllegalProcedureDec_ReturnFalseAndFuncNotAdd(
    myTyFieldList funcFields, myExp funcBody);

///////////////

void test_MySemanticDecFuncProcedure_ParamTypeNotDefined_ReturnFalseAndFuncNotAdd(void)
{
    mySymbol notDefinedFieldTypeName = makeOneSymbol();
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), notDefinedFieldTypeName),
        NULL);

    test_IllegalProcedureDec_ReturnFalseAndFuncNotAdd(
        funcFields, makeOneExp_NoValue());
}

void test_MySemanticDecFuncProcedure_BodyExpNotLegal_ReturnFalseAndFuncNotAdd(void)
{
    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp illegalBodyExp = makeOneIllegalExp_Integer(); 

    test_IllegalProcedureDec_ReturnFalseAndFuncNotAdd(
        funcFields, illegalBodyExp);
}

void test_MySemanticDecFuncProcedure_BodyExpNotNoReturn_ReturnFalseAndFuncNotAdd(void)
{
    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp bodyExpNotNoReturn = makeOneExp_Integer();

    test_IllegalProcedureDec_ReturnFalseAndFuncNotAdd(
        funcFields, bodyExpNotNoReturn);
}

void test_MySemanticDecFuncProcedure_LegalDec_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp funcBody = makeOneExp_NoValue();
    mySymbol funcName = makeOneSymbol(); 
    myProcedureDec procedureDec = makeMyProcedureDec(
        funcName, funcFields, funcBody);


    bool result = 
        MySemantic_Dec_Func_Procedure(varAndFuncEnv, typeEnv, procedureDec);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncProcedure_LegalDec_FormalsCanBeUsedInBody(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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
        MySemantic_Dec_Func_Procedure(varAndFuncEnv, typeEnv, procedureDec);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncProcedure_LegalDec_FuncAdded(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol fieldTypeName = MySymbol_MakeSymbol("int");
    myTyFieldList funcFields = makeMyTyFieldList(makeMyTyField(
        makeOneSymbol(), fieldTypeName),
        NULL);
    myExp funcBody = makeOneExp_NoValue();
    mySymbol funcName = makeOneSymbol(); 
    myProcedureDec procedureDec = makeMyProcedureDec(
        funcName, funcFields, funcBody);


    bool result = 
        MySemantic_Dec_Func_Procedure(varAndFuncEnv, typeEnv, procedureDec);

    myVarAndFuncEntry funcEntry =
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, funcName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isFuncEntry(funcEntry));
    CU_ASSERT(isTypeInt(MyEnvironment_getFuncFormalTypes(funcEntry)->head));
    CU_ASSERT(isTypeNoReturn(MyEnvironment_getFuncReturnType(funcEntry)));
}

//  a parameterized test
void test_IllegalProcedureDec_ReturnFalseAndFuncNotAdd(
    myTyFieldList funcFields, myExp funcBody)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol funcName = makeOneSymbol(); 
    myProcedureDec procedureDec = makeMyProcedureDec(
        funcName, funcFields, funcBody);

    bool result = 
        MySemantic_Dec_Func_Procedure(varAndFuncEnv, typeEnv, procedureDec);

    bool funcNotAdd =
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, funcName) == NULL;
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT_EQUAL(funcNotAdd, true);
}

/////////////////////////////////////////////////////////////////////////////

//  forwards
void test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);

/////////////////

void test_MySemanticDecFuncFunction_ParamTypesNotDefined_ReturnFalseAndFunctionNotAdd(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol notDefinedParamTypeName = makeOneSymbol();
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), notDefinedParamTypeName), NULL),
        makeSymbol_Int(),
        makeOneExp_Integer());

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
        varAndFuncEnv, typeEnv, dec);
}

void test_MySemanticDecFuncFunction_ReturnTypeNotDefined_ReturnFalseAndFunctionNotAdd(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol notDefinedReturnType = makeOneSymbol();
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeOneSymbol()), NULL),
        notDefinedReturnType,
        makeOneExp_Integer());

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
        varAndFuncEnv, typeEnv, dec);
}

void test_MySemanticDecFuncFunction_BodyNotlegal_ReturnFalseAndFunctionNotAdd(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myExp illegalBody = makeOneIllegalExp_Integer(); 
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeOneSymbol()), NULL),
        makeOneSymbol(),
        illegalBody);

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
        varAndFuncEnv, typeEnv, dec);
}

void test_MySemanticDecFuncFunction_ReturnTypeNotMatch_ReturnFalseAndFunctionNotAdd(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myExp intBody = makeOneExp_Integer(); 
    mySymbol stringReturnType = makeSymbol_String();
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeOneSymbol()), NULL),
        stringReturnType,
        intBody);

    test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
        varAndFuncEnv, typeEnv, dec);
}

void test_MySemanticDecFuncFunction_LegalFunctionDec_ReturnTrueAndFunctionAdded(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        makeSymbol_Int(),
        makeOneExp_Integer());

    bool result = MySemantic_Dec_Func_Function(varAndFuncEnv, typeEnv, dec);

    myVarAndFuncEntry funcEntry =
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, dec->name);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isFuncEntry(funcEntry));
    CU_ASSERT(isTypeInt(MyEnvironment_getFuncFormalTypes(funcEntry)->head));
    CU_ASSERT(isTypeInt(MyEnvironment_getFuncReturnType(funcEntry)));
}

void test_MySemanticDecFuncFunction_LegalFunctionDec_FormalsCanBeUsedInBody(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol formalVarName = MySymbol_MakeSymbol("formal var");
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(formalVarName, makeSymbol_Int()), NULL),
        makeSymbol_Int(),
        makeMyExp_LValue(makeOnePos(),makeMyLValue(makeOnePos(), formalVarName, NULL)));

    bool result = MySemantic_Dec_Func_Function(varAndFuncEnv, typeEnv, dec);

    CU_ASSERT_EQUAL(result, true);    
}

void test_MySemanticDecFuncFunction_NilBodyOfRecordReturnType_Succeed(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol recordTypeName = MySymbol_MakeSymbol("record type");
    makeAndAddOneType_NoFieldRecord(typeEnv, recordTypeName);
    myFunctionDec dec = makeMyFunctionDec(
        makeOneSymbol(),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        recordTypeName,
        makeOneExp_Nil());

    bool result = MySemantic_Dec_Func_Function(varAndFuncEnv, typeEnv, dec);

    CU_ASSERT_EQUAL(result, true);
}

//  a parameterized test
void test_IllegalFunctionDec_ReturnFalseAndFunctionNotAdded(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec)
{
    bool result = MySemantic_Dec_Func_Function(varAndFuncEnv, typeEnv, functionDec);

    bool functionNotAdded = MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, functionDec->name)
        == NULL;
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT(functionNotAdded);
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticDecs_DecsContainsIllegalDec_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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

    bool result = MySemantic_Decs(varAndFuncEnv, typeEnv, decs);

    CU_ASSERT_EQUAL(result, false);
}

/*void test_MySemanticDecs_ConsecutiveSameFuncOrTypeDecs_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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


    bool resultSameFunc = MySemantic_Decs(varAndFuncEnv, typeEnv, sameFuncDecs);
    bool resultSameType = MySemantic_Decs(varAndFuncEnv, typeEnv, sameTypeDecs);

    CU_ASSERT_EQUAL(resultSameFunc, false);
    CU_ASSERT_EQUAL(resultSameType, false);
}

void test_MySemanticDecs_NotConsecutiveSameFuncOrTypeDecs_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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


    bool resultFunc = MySemantic_Decs(varAndFuncEnv, typeEnv, notConsecutiveFuncs);
    bool resultType = MySemantic_Decs(varAndFuncEnv, typeEnv, notConsecutiveTypes);

    CU_ASSERT_EQUAL(resultFunc, true);
    CU_ASSERT_EQUAL(resultType, true);
}*/

void test_MySemanticDecs_ConsecutiveSameVarDecs_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myVarDec varDec = makeMyVarDec_ShortForm(
        makeMyShortFormVar(MySymbol_MakeSymbol("var name"), makeOneExp_Integer()));
    myDecList sameVarDecs = makeMyDecList(
        makeMyDec_Var(makeOnePos(), varDec),
        makeMyDecList(
            makeMyDec_Var(makeOnePos(), varDec),NULL));


    bool resultSameVar = MySemantic_Decs(varAndFuncEnv, typeEnv, sameVarDecs);

    CU_ASSERT_EQUAL(resultSameVar, true);
}

void test_MySemanticDecs_DecsContainslegalDec_ReturnTrueAndDecsAdded(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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

    bool result = MySemantic_Decs(varAndFuncEnv, typeEnv, decs);

    myVarAndFuncEntry funcEntry =
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, funcName);
    myVarAndFuncEntry varEntry =
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, varName);
    myType type = MyEnvironment_getTypeFromName(typeEnv, typeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(myEnvironment_isFuncEntry(funcEntry));
    CU_ASSERT(isTypeString(MyEnvironment_getFuncReturnType(funcEntry)));
    CU_ASSERT(myEnvironment_isVarEntry(varEntry));
    CU_ASSERT(isTypeInt(type));
}

/////////////////////////////////////////////////////////////////////////////

void test_MySemanticLetExp_IllegalDeclarations_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myDecList decs = makeMyDecList(
        makeMyDec_Var(makeOnePos(), 
            makeMyVarDec_ShortForm(
            makeMyShortFormVar(makeOneSymbol(), makeOneExp_NoValue()))),
        NULL);
    myExpList exps = makeMyExpList(makeOneExp_Integer(), NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(varAndFuncEnv, typeEnv, letExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLetExp_IllegalBody_ReturnNull(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myDecList decs = NULL;
    myExpList exps = makeMyExpList(makeOneIllegalExp_Integer(), NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(varAndFuncEnv, typeEnv, letExp);

    CU_ASSERT_EQUAL(result, SEMANTIC_ERROR);
}

void test_MySemanticLetExp_LegalExp_ReturnLastExpType(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    myDecList decs = NULL;
    myExpList exps = makeMyExpList(makeOneExp_Integer(), NULL);
    myLetExp letExp = makeMyLetExp(decs, exps);


    myTranslationAndType result =
        MySemantic_LetExp(varAndFuncEnv, typeEnv, letExp);

    CU_ASSERT(isTypeInt(result->type));
}

void test_MySemanticLetExp_LegalExp_VarInDecsCanBeUsedInExps(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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
        MySemantic_LetExp(varAndFuncEnv, typeEnv, letExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
    CU_ASSERT(isTypeInt(result->type));
}

void test_MySemanticLetExp_LegalExp_TypesInDecsCanBeUsedInExps(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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
        MySemantic_LetExp(varAndFuncEnv, typeEnv, letExp);

    CU_ASSERT(result != SEMANTIC_ERROR);
    CU_ASSERT(isTypeArray(result->type));
} 

void test_MySemanticLetExp_LegalExp_FuncsInDecsCanBeUsedInExps(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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
        MySemantic_LetExp(varAndFuncEnv, typeEnv, letExp);

    myType letReturnType = makeType_String();
    CU_ASSERT(result != SEMANTIC_ERROR);
    CU_ASSERT(isTypeEqual(result->type, letReturnType));
}

/////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
    CU_pSuite suite;
    if (!initialTests(&suite))      return -1;

    //	add tests
    CU_TestInfo tests[] = {
        { "", test_typeContainsLValueAux_CheckRecord_ReturnTypeOfOneField },
        { "", test_typeContainsLValueAux_CheckRecord_ReturnTypeOfNestedField },
        { "", test_typeContainsLValueAux_CheckArray_ReturnTypeOfOneField },
        { "", test_typeContainsLValueAux_CheckArray_ReturnTypeOfNestedField },

        { "", test_MySemanticLValueExpSimpleVar_VarNotDeclared_ReturnNull },
        { "", test_MySemanticLValueExpSimpleVar_LegalExp_ReturnTypeOfSimpleVar },

        { "", test_MySemanticLValueExpRecordField_VariableNotDeclared_ReturnNull },
        { "", test_MySemanticLValueExpRecordField_VariableNotRecord_ReturnNull },
        { "", test_MySemanticLValueExpRecordField_FieldNamesNotMatch_ReturnNull },
        { "", test_MySemanticLValueExpRecordField_LegalExp_ReturnTypeOfRecordField },

        { "", test_MySemanticLValueExpArraySubscript_ArrayVarNotDeclared_ReturnNull },
        { "", test_MySemanticLValueExpArraySubscript_VarNotArrayType_ReturnNull },
        { "", test_MySemanticLValueExpArraySubscript_SubscriptNotInt_ReturnNull },
        { "", test_MySemanticLValueExpArraySubscript_LegalExp_ReturnTypeOfArray },

        { "", test_MySemanticNilExp_ByDefault_ReturnNilType },
        { "", test_MySemanticIntegerLiteralExp_ByDefault_ReturnIntType },
        { "", test_MySemanticStringLiteralExp_ByDefault_ReturnStringType },
        { "", test_MySemanticNoValueExp_ByDefault_ReturnNoReturnType },
        { "", test_MySemanticBreakExp_ByDefault_ReturnNoReturnType },

        { "", test_MySemanticFunctionCallExpNoParam_LegalExp_ReturnTypeOfFunctionResult },
        { "", test_MySemanticFunctionCallExpNoParam_FunctionNotDefined_ReturnNull },

        { "", test_MySemanticFunctionCallExpParam_FunctionNotDefined_ReturnNull },
        { "", test_MySemanticFunctionCallExpParam_ParamsTypeNotMatch_ReturnNull },
        { "", test_MySemanticFunctionCallExpParam_LegalExp_ReturnTypeOfFunctionResult },
        { "", test_MySemanticFunctionCallExpParam_NilValueOfRecordParamType_Succeed },

        { "", test_MySemanticArrayCreationExp_LegalExp_ReturnArrayType },
        { "", test_MySemanticArrayCreationExp_NilInitialValueOfRecordElement_Succeed },
        { "", test_MySemanticArrayCreationExp_ArrayTypeNotDefined_ReturnNull },
        { "", test_MySemanticArrayCreationExp_OnlyLengthPartIllegal_ReturnNull },
        { "", test_MySemanticArrayCreationExp_OnlyInitialPartIllegal_ReturnNull },

        { "", test_MySemanticRecordCreationExpNoField_LegalExp_ReturnRecordType },
        { "", test_MySemanticRecordCreationExpField_NilValueOfRecordField_Succeed },
        { "", test_MySemanticRecordCreationExpNoField_RecordTypeNotDefined_ReturnNull },
        { "", test_MySemanticRecordCreationExpNoField_TypeNotRecord_ReturnNull },

        { "", test_MySemanticRecordCreationExpField_LegalExp_ReturnRecordType },
        { "", test_MySemanticRecordCreationExpField_RecordTypeNotDefined_ReturnNull },
        { "", test_MySemanticRecordCreationExpField_RecordFieldsNotMatch_ReturnNull },
        { "", test_MySemanticRecordCreationExpField_TypeNotRecord_ReturnNull },

        { "", test_MySemanticArithmeticExp_LegalExp_ReturnIntType },
        { "", test_MySemanticArithmeticExp_IllegalExp_ReturnNull },

        { "", test_MySemanticParenthesesExp_BeDefault_ReturnInnerResult },

        { "", test_MySemanticSequencingExp_AnyOneIllegal_ReturnNull },
        { "", test_MySemanticSequencingExp_LegalExp_ReturnLastExpressionResult },

        { "", test_MySemanticForExp_LegalForExp_ReturnTypeNoReturn },
        { "", test_MySemanticForExp_OnlyLowRangeNotInt_ReturnNull },
        { "", test_MySemanticForExp_OnlyHighRangeNotInt_ReturnNull },
        { "", test_MySemanticForExp_OnlybodyNotTypeNoReturn_ReturnNullt },
        { "", test_MySemanticForExp_LegalForExp_ConditionVarCanBeUsedInBody },

        { "", test_MySemanticIfThenElseExp_LegalExpression_ReturnClauseType },
        { "", test_MySemanticIfThenElseExp_OneNilAnotherRecord_Succeed },
        { "", test_MySemanticIfThenElseExp_OnlyIfClauseNotInt_ReturnNull },
        { "", test_MySemanticIfThenElseExp_OnlyThenClauseNotInt_ReturnNull },
        { "", test_MySemanticIfThenElseExp_OnlyElseClauseNotInt_ReturnNull },
        { "", test_MySemanticIfThenElseExp_OnlyThenElseTypeMismatch_ReturnNull },

        { "", test_MySemanticIfThenExp_OnlyIfconditionNotInt_ReturnNull },
        { "", test_MySemanticIfThenExp_OnlyThenClauseNotNoReturn_ReturnNull },
        { "", test_MySemanticIfThenExp_LegalExp_ReturnNoReturnType },

        { "", test_MySemanticComparisonExp_IllegalLeftOperand_ReturnNull },
        { "", test_MySemanticComparisonExp_IllegalRightOperand_ReturnNull },
        { "", test_MySemanticComparisonExp_AnyOperandsTypeNotInt_ReturnNull },
        { "", test_MySemanticComparisonExp_LegalExp_ReturnIntType },
        { "", test_MySemanticComparisonExp_OneRecordAnotherNil_Succeed },

        { "", test_MySemanticBooleanOperateExp_OnlyLeftOperandNotInt_ReturnNull },
        { "", test_MySemanticBooleanOperateExp_OnlyRightOperandNotInt_ReturnNull },
        { "", test_MySemanticBooleanOperateExp_LegalExp_ReturnIntType },

        { "", test_MySemanticAssignmentExp_OnlyLeftOperandIllegal_ReturnNull },
        { "", test_MySemanticAssignmentExp_OnlyRightOperandIllegal_ReturnNull },
        { "", test_MySemanticAssignmentExp_OperandsTypeNotMatch_ReturnNull },
        { "", test_MySemanticAssignmentExp_LegalExp_ReturnNoReturnType },
        { "", test_MySemanticAssignmentExp_LeftRecordRightNil_Succeed },

        { "", test_MySemanticWhileExp_OnlyConditionNotInt_ReturnNull },
        { "", test_MySemanticWhileExp_OnlyBodyNotNoReturn_ReturnNull },
        { "", test_MySemanticWhileExp_LegalExp_ReturnNoReturnType },

        { "", test_MySemanticNegationExp_ExpNotInt_ReturnNull },
        { "", test_MySemanticNegationExp_LegalExp_ReturnIntType },

        { "", test_MySemanticDecTypeNamed_NamedTypeNotDefined_TypeNotAdd },
        { "", test_MySemanticDecTypeNamed_NamedTypeOfBuildInt_AddedTypeIsInt },
        { "", test_MySemanticDecTypeNamed_NamedTypeOfNamedOne_AddedTypeIsNamedActualType },

        { "", test_MySemanticDecTypeRecord_FieldTypeNotDefined_TypeNotAdd },
        { "", test_MySemanticDecTypeRecord_LegalParam_AddOneRecordType },

        { "", test_MySemanticDecTypeArray_ElementTypeNotDefined_TypeNotAdd },
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

        { "", test_MySemanticDecFuncProcedure_ParamTypeNotDefined_ReturnFalseAndFuncNotAdd },
        { "", test_MySemanticDecFuncProcedure_BodyExpNotLegal_ReturnFalseAndFuncNotAdd },
        { "", test_MySemanticDecFuncProcedure_BodyExpNotNoReturn_ReturnFalseAndFuncNotAdd },
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
        /*{ "", test_MySemanticDecs_ConsecutiveSameFuncOrTypeDecs_ReturnFalse },
        { "", test_MySemanticDecs_NotConsecutiveSameFuncOrTypeDecs_ReturnTrue },*/
        { "", test_MySemanticDecs_ConsecutiveSameVarDecs_ReturnTrue },
        { "", test_MySemanticDecs_DecsContainslegalDec_ReturnTrueAndDecsAdded },

        { "", test_MySemanticLetExp_IllegalDeclarations_ReturnNull },
        { "", test_MySemanticLetExp_IllegalBody_ReturnNull },
        { "", test_MySemanticLetExp_LegalExp_ReturnLastExpType },
        { "", test_MySemanticLetExp_LegalExp_VarInDecsCanBeUsedInExps },
        { "", test_MySemanticLetExp_LegalExp_TypesInDecsCanBeUsedInExps },
        { "", test_MySemanticLetExp_LegalExp_FuncsInDecsCanBeUsedInExps }
    };
    
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;
    if (!runTests(&suite))      
        return -1;

    freeMemoryChain();
    return 0;
}