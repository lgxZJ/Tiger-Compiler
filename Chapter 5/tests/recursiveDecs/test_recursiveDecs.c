#include "../testHelper.h"
#include "../testFixture.h"

#include "../../recursiveDecs.h"
#include "../../myEnvironment.h"
#include "../../mySemantic.h"
#include "../../abstractSyntaxMaker.h"
#include "../../typeChecker.h"
#include "../../typeMaker.h"

//////////////////////////////////////////////////////////////////////
//  forwards of tested functions
bool MySemantic_Dec_Type_OnePass(myTypeDec typeDec);
bool MySemantic_Dec_Func_Procedure_OnePass(
    myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_OnePass(
    myFunctionDec functionDec);
bool MySemantic_Dec_Type_Named_TwoPass(
    mySymbol newTypeName, mySymbol existedTypeName);
bool MySemantic_Dec_Type_Record_TwoPass(
    mySymbol newTypeName, myTyFieldList fields);
bool MySemantic_Dec_Type_Array_TwoPass(
    mySymbol newTypeName, mySymbol elementTypeName);
bool MySemantic_Dec_Type_TwoPass(myTypeDec typeDec);
bool MySemantic_Dec_Func_Procedure_TwoPass(
    myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_TwoPass(
    myFunctionDec functionDec);
bool MySemantic_Decs_Recursive(
    myDecList decs);

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeOnePass_ByDefault_ReturnTrueAndAddNullType(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol typeName = MySymbol_MakeSymbol("typeName");
    myTypeDec typeDec = makeMyTyDec(typeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int()));


    bool result = MySemantic_Dec_Type_OnePass(typeDec);

    myType typeGot = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(),
        typeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(typeGot->u.typeNamed->type, NULL);
} 

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecFuncProcedureOnePass_ParamTypesNotDefined_ReturnFalseAndNotAdd(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol procedureName = MySymbol_MakeSymbol("procedureName");
    mySymbol notDefinedTypeName=  MySymbol_MakeSymbol("not defined");
    myProcedureDec dec = makeMyProcedureDec(
        procedureName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), notDefinedTypeName), NULL),
        makeOneExp_NoValue());


    bool result = MySemantic_Dec_Func_Procedure_OnePass(dec);

    myVarAndFuncEntry entryGot = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(), procedureName);
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT_EQUAL(entryGot, NULL);
}

void test_MySemanticDecFuncProcedureOnePass_ParamTypesDefined_ReturnTrueAndAdded(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol procedureName = MySymbol_MakeSymbol("procedureName");
    myProcedureDec dec = makeMyProcedureDec(
        procedureName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        makeOneExp_NoValue());


    bool result = MySemantic_Dec_Func_Procedure_OnePass(dec);

    myVarAndFuncEntry entryGot = MyEnvironment_getVarOrFuncFromName(
        MySemantic_getVarAndFuncEnvironment(), procedureName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_NOT_EQUAL(entryGot, NULL);
    CU_ASSERT(myEnvironment_isFuncEntry(entryGot));
}

/////////////////////////////////////////////////////////

//  forwards
void test_IllegalFunctionDecOnePass_ReturnFalseAndFunctionNotAdd(
    mySymbol paramTypeName, mySymbol returnTypeName);

////////////////////////

void test_MySemanticDecFuncFunctionOnePass_ParamTypesNotDefined_ReturnFalseAndFuncNotAdd(void)
{
    mySymbol paramTypeName = MySymbol_MakeSymbol("notDefined");
    mySymbol returnTypeName = makeSymbol_Int();

    test_IllegalFunctionDecOnePass_ReturnFalseAndFunctionNotAdd(
        paramTypeName, returnTypeName);
}

void test_MySemanticDecFuncFunctionOnePass_ReturnTypesNotDefined_ReturnFalseAndFuncNotAdd(void)
{
    mySymbol paramTypeName = makeSymbol_Int();
    mySymbol returnTypeName = MySymbol_MakeSymbol("returnType");

    test_IllegalFunctionDecOnePass_ReturnFalseAndFunctionNotAdd(
        paramTypeName, returnTypeName);
}

void test_MySemanticDecFuncFunctionOnePass_TypesAllDefined_ReturnTrueAndFunctionAdded(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionName = MySymbol_MakeSymbol("name");
    myFunctionDec dec = makeMyFunctionDec(
        functionName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        makeSymbol_Int(),
        makeOneExp_NoValue());

    bool result =
        MySemantic_Dec_Func_Function_OnePass(dec);

    myVarAndFuncEntry entry =
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(), functionName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_NOT_EQUAL(entry, NULL);
    CU_ASSERT(myEnvironment_isFuncEntry(entry));
}

//  a parameterized test
void test_IllegalFunctionDecOnePass_ReturnFalseAndFunctionNotAdd(
    mySymbol paramTypeName, mySymbol returnTypeName)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionName = MySymbol_MakeSymbol("name");
    myFunctionDec dec = makeMyFunctionDec(
        functionName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), paramTypeName), NULL),
        returnTypeName,
        makeOneExp_NoValue());

    bool result =
        MySemantic_Dec_Func_Function_OnePass(dec);

    myVarAndFuncEntry entry =
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(), functionName);
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT_EQUAL(entry, NULL);
}

//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeNamedTwoPass_TypeNotDefined_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol undefinedTypeName = MySymbol_MakeSymbol("undefined");
    mySymbol typeName = makeOneSymbol(); 

    bool result = MySemantic_Dec_Type_Named_TwoPass(
        typeName , undefinedTypeName);

    CU_ASSERT_EQUAL(result, false);
}


void test_MySemanticDecTypeNamedTwoPass_TypeDefinedBodyReal_ReturnTrueAndTypeSet(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol newTypeName = MySymbol_MakeSymbol("newType");
    myTypeDec typeDec = makeMyTyDec(
        newTypeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int()));
    MySemantic_Dec_Type_OnePass(typeDec);


    bool result = MySemantic_Dec_Type_Named_TwoPass(
        newTypeName, makeSymbol_Int());

    myType typeGot = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), newTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeNamed(typeGot));   
}

void test_MySemanticDecTypeNamedTwoPass_TypeDefinedBodyEmpty_ReturnTrueAndTypeBLinkToTypeA(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol typeNameB = MySymbol_MakeSymbol("typeB");
    mySymbol typeNameA = MySymbol_MakeSymbol("typeA");
    myTypeDec typeDec = makeMyTyDec(
        typeNameB, makeMyTy_Named(makeOnePos(), typeNameA));
    MySemantic_Dec_Type_OnePass(
        makeMyTyDec(typeNameA, makeMyTy_Named(makeOnePos(), makeSymbol_Int())));
    MySemantic_Dec_Type_OnePass(typeDec);


    bool result = MySemantic_Dec_Type_Named_TwoPass(
        typeNameB, typeNameA);

    myType typeGotB = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), typeNameB);
    myType typeGotA = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), typeNameA);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(typeGotA, typeGotB->u.typeNamed->type);
}


void test_MySemanticDecTypeNamedPass_ManuallyRecursiveNamedType_Succeed(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    mySymbol typeNameB = MySymbol_MakeSymbol("B");
    myTypeDec typeDecFirst = makeMyTyDec(
        typeNameA, makeMyTy_Named(makeOnePos(), typeNameB));
    myTypeDec typeDecSecond = makeMyTyDec(
        typeNameB, makeMyTy_Named(makeOnePos(), makeSymbol_Int()));


    MySemantic_Dec_Type_OnePass(typeDecFirst);
    MySemantic_Dec_Type_OnePass(typeDecSecond);
    MySemantic_Dec_Type_TwoPass(typeDecFirst);
    MySemantic_Dec_Type_TwoPass(typeDecSecond);

    myType typeA = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), typeNameA);
    myType typeB = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), typeNameB);
    CU_ASSERT(isTypeInt(typeB->u.typeNamed->type));
    CU_ASSERT(isTypeEqual(
        typeA->u.typeNamed->type->u.typeNamed->type, 
        typeB->u.typeNamed->type));
}

void test_MySemanticDecTypeNamedPass_RecursiveNamed_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    myTypeDec typeDec = makeMyTyDec(
        typeNameA, makeMyTy_Named(makeOnePos(), typeNameA));


    MySemantic_Dec_Type_OnePass(typeDec);
    bool result = MySemantic_Dec_Type_TwoPass(typeDec);

    CU_ASSERT_EQUAL(result, false);
}

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeRecordTwoPass_FieldTypesNotDefined_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol varName = makeOneSymbol();
    mySymbol typeName = MySymbol_MakeSymbol("typeA");
    mySymbol recordTypeName = makeOneSymbol();
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(varName, typeName), NULL);
    MySemantic_Dec_Type_OnePass(makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields)));

    bool result = MySemantic_Dec_Type_Record_TwoPass(
        recordTypeName, tyFields);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeRecordTwoPass_FieldTypesDefined_ReturnTrueAndSetType(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), MySymbol_MakeSymbol("int")),
        NULL);
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    MySemantic_Dec_Type_OnePass(makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields)));
    

    bool result = MySemantic_Dec_Type_Record_TwoPass(
        recordTypeName, tyFields);

    myType recordType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), recordTypeName); 
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeRecord(recordType));
    CU_ASSERT(isTypeInt(recordType->u.typeRecord->fieldList->head->type));
}

void test_MySemanticDecTypeRecordTwoPass_ManuallyRecursiveType_ReturnTrueAndLink(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol typeNameA = MySymbol_MakeSymbol("typeA");
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    MySemantic_Dec_Type_OnePass(
        makeMyTyDec(typeNameA, makeMyTy_Named(makeOnePos(), recordTypeName)));

    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL);
    MySemantic_Dec_Type_OnePass(makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields)));
    

    MySemantic_Dec_Type_Named_TwoPass(
        typeNameA, recordTypeName);
    bool result = MySemantic_Dec_Type_Record_TwoPass(
        recordTypeName, tyFields);

    myType typeA = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), typeNameA);
    myType recordType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), recordTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(
        typeA->u.typeNamed->type,
        recordType);
}

void test_MySemanticDecTypeRecordTwoPass_RecursiveType_ReturnTrueAndLink(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol recordTypeName = MySymbol_MakeSymbol("recursiveType");
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), recordTypeName), NULL);
    myTypeDec typeDec = makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields));

    MySemantic_Dec_Type_OnePass(typeDec);
    bool result =
        MySemantic_Dec_Type_Record_TwoPass(recordTypeName, tyFields);

    myType typeGot = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), recordTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(typeGot->u.typeRecord->fieldList->head->type, typeGot);
}

////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeArrayTwoPass_ElementTypeNotDefined_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("elementType");

    bool result = MySemantic_Dec_Type_Array_TwoPass(
        newTypeName, elementTypeName);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeArray_ElementTypeDefinedAndNotEmpty_ReturnTrueAndSetType(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("int");
    MySemantic_Dec_Type_OnePass(makeMyTyDec(
        newTypeName, makeMyTy_Array(makeOnePos(), elementTypeName)));


    bool result = MySemantic_Dec_Type_Array_TwoPass(
        newTypeName, elementTypeName);

    myType arrayType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), newTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeArray(arrayType));
    CU_ASSERT(isTypeInt(arrayType->u.typeArray->type));
}

void test_MySemanticDecTypeArray_ManuallyRecursiveType_ReturnTrueAndLinkToElementType(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol outerArrayTypeName = MySymbol_MakeSymbol("newTypeName");

    mySymbol innerArrayTypeName = MySymbol_MakeSymbol("files");

    MySemantic_Dec_Type_OnePass(makeMyTyDec(
        outerArrayTypeName, makeMyTy_Array(makeOnePos(), innerArrayTypeName)));
    MySemantic_Dec_Type_OnePass(makeMyTyDec(
        innerArrayTypeName, makeMyTy_Array(makeOnePos(), MySymbol_MakeSymbol("int"))));
    

    bool resultOuter = MySemantic_Dec_Type_Array_TwoPass(
        outerArrayTypeName, innerArrayTypeName); 
    bool resultInner = MySemantic_Dec_Type_Array_TwoPass(
        innerArrayTypeName, makeSymbol_Int());

    myType outerArrayType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), outerArrayTypeName);
    myType innerArrayType = MyEnvironment_getTypeFromName(
        MySemantic_getTypeEnvironment(), innerArrayTypeName);  
    CU_ASSERT_EQUAL(resultInner, true);
    CU_ASSERT_EQUAL(resultOuter, true);
    CU_ASSERT_EQUAL(outerArrayType->u.typeArray->type, innerArrayType);
}

void test_MySemanticDecTypeArray_RecursievType_ReturnFalse(void)
{
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol arrayTypeName = MySymbol_MakeSymbol("arrayTypeName");
    myTypeDec recursiveTypeDec = makeMyTyDec(
        arrayTypeName, makeMyTy_Array(makeOnePos(), arrayTypeName));

    MySemantic_Dec_Type_OnePass(recursiveTypeDec);
    bool result =
        MySemantic_Dec_Type_Array_TwoPass(recursiveTypeDec->name,
            recursiveTypeDec->type->u.arrayTypeName);

    CU_ASSERT_EQUAL(result, false);
}

//////////////////////////////////////////////////////////////////////

//  forwards
void testProcedureTwoPass_BodyExp_ReturnTrueOrFalse(myExp body, bool value);

//////////////

void test_MySemanticDecFuncProcedureTwoPass_BodyTypeNotNoReturn_ReturnFalse(void)
{
    myExp procedureBody = makeOneExp_Integer();
    testProcedureTwoPass_BodyExp_ReturnTrueOrFalse(procedureBody, false);
}

void test_MySemanticDecFuncProcedureTwoPass_BodyTypeNoReturn_ReturnTrue(void)
{
    myExp procedureBody = makeOneExp_NoValue();
    testProcedureTwoPass_BodyExp_ReturnTrueOrFalse(procedureBody, true);
}

void test_MySemanticDecFuncProcedureTwoPass_RecursiveProcedure_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionName = MySymbol_MakeSymbol("procedureName"); 
    myProcedureDec proceOne = makeMyProcedureDec(
        functionName,
        NULL,
        makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
            makeMyNoParamFunctionCallExp(functionName))));


    MySemantic_Dec_Func_Procedure_OnePass(proceOne);
    bool result =
        MySemantic_Dec_Func_Procedure_TwoPass(proceOne);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncProcedureTwoPass_ManuallyRecursiveProcedure_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionNameA = MySymbol_MakeSymbol("procedureNameA");
    mySymbol functionNameB = MySymbol_MakeSymbol("procedureNameB");

    myProcedureDec proceOneA = makeMyProcedureDec(
        functionNameA,
        NULL,
        makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
            makeMyNoParamFunctionCallExp(functionNameB))));
    myProcedureDec proceOneB = makeMyProcedureDec(
        functionNameB,
        NULL,
        makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
            makeMyNoParamFunctionCallExp(functionNameA))));


    MySemantic_Dec_Func_Procedure_OnePass(proceOneA);
    MySemantic_Dec_Func_Procedure_OnePass(proceOneB);
    bool resultA = 
        MySemantic_Dec_Func_Procedure_TwoPass(proceOneA);
    bool resultB =
        MySemantic_Dec_Func_Procedure_TwoPass(proceOneB);

    CU_ASSERT_EQUAL(resultA, true);
    CU_ASSERT_EQUAL(resultB, true);
}

//  a parameterized test
void testProcedureTwoPass_BodyExp_ReturnTrueOrFalse(myExp body, bool value)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myProcedureDec dec = makeOneProcedureDec_Body(body);

    bool result = MySemantic_Dec_Func_Procedure_TwoPass(dec);

    CU_ASSERT_EQUAL(result, value);
}

/////////////////////////////////////////////////////////////////////

//  forwards
void testFunctionTwoPass_BodyExp_ReturnTrueOrFalse(
    myExp body, mySymbol returnTypeName, bool value);

//////////////

void test_MySemanticDecFuncFunctionTwoPass_BodyIllegal_ReturnFalse(void)
{
    myExp body = makeOneIllegalExp_Integer();
    testFunctionTwoPass_BodyExp_ReturnTrueOrFalse(body, makeSymbol_Int(), false);
}

void test_MySemanticDecFuncFunctionTwoPass_ReturnTypeNotMatches_ReturnFalse(void)
{
    myExp body = makeOneExp_Integer();
    mySymbol returnTypeName = MySymbol_MakeSymbol("string");
    testFunctionTwoPass_BodyExp_ReturnTrueOrFalse(body, returnTypeName, false);
}

void test_MySemanticDecFuncFunctionTwoPass_ReturnTypeMatches_ReturnTrue(void)
{
    myExp intBody = makeOneExp_Integer();
    mySymbol intReturnTypeName = makeSymbol_Int();

    testFunctionTwoPass_BodyExp_ReturnTrueOrFalse(intBody, intReturnTypeName, true);
}

void test_MySemanticDecFuncFunctionTwoPass_ReturnTypeRecordBodyNil_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeType_Record(NULL);
    MyEnvironment_addNewType(MySemantic_getTypeEnvironment(),
        recordTypeName, recordType); 

    myExp nilBody = makeMyExp_Nil(makeOnePos(), makeMyNilExp());
    myFunctionDec dec = makeOneFunctionDec_Body(nilBody, recordTypeName);


    bool result = MySemantic_Dec_Func_Function_TwoPass(dec);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncFunctionTwoPass_RecursiveFunction_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionName = MySymbol_MakeSymbol("functionName");
    myExp body = makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_Param(
        makeMyParamFunctionCallExp(functionName, 
        makeMyExpList(makeOneExp_Integer(), NULL))));
    myFunctionDec dec = makeOneFunctionDec_Int(functionName, body, makeSymbol_Int());
    

    MySemantic_Dec_Func_Function_OnePass(dec);
    bool result = MySemantic_Dec_Func_Function_TwoPass(dec);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncFunctionTwoPass_ManuallyRecursiveFunction_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol functionNameA = MySymbol_MakeSymbol("functionNameA");
    mySymbol functionNameB = MySymbol_MakeSymbol("functionNameB");

    myFunctionDec funcOneA = makeMyFunctionDec(
        functionNameA,
        NULL,
        makeSymbol_Int(),
        makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
            makeMyNoParamFunctionCallExp(functionNameB))));
    myFunctionDec funcOneB = makeMyFunctionDec(
        functionNameB,
        NULL,
        makeSymbol_Int(),
        makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
            makeMyNoParamFunctionCallExp(functionNameA))));


    MySemantic_Dec_Func_Function_OnePass(funcOneA);
    MySemantic_Dec_Func_Function_OnePass(funcOneB);
    bool resultA = 
        MySemantic_Dec_Func_Function_TwoPass(funcOneA);
    bool resultB =
        MySemantic_Dec_Func_Function_TwoPass(funcOneB);

    CU_ASSERT_EQUAL(resultA, true);
    CU_ASSERT_EQUAL(resultB, true);
}

//  a parameterized test
void testFunctionTwoPass_BodyExp_ReturnTrueOrFalse(
    myExp body, mySymbol returnTypeName, bool value)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myFunctionDec dec = makeOneFunctionDec_Body(body, returnTypeName);

    bool result = MySemantic_Dec_Func_Function_TwoPass(dec);

    CU_ASSERT_EQUAL(result, value);
}

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecsRecursive_LegalVars_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myDecList varDecs = makeLegalVars();

    bool result = MySemantic_Decs_Recursive(varDecs);

    CU_ASSERT_EQUAL(result, true); 
}

void test_MySemanticDecsRecursive_IllegalVars_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    myDecList varDecs = makeIllegalVars();

    bool result = MySemantic_Decs_Recursive(varDecs);

    CU_ASSERT_EQUAL(result, false); 
}

void test_MySemanticDecsRecursive_LegalRecursiveTypes_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    mySymbol typeNameB = MySymbol_MakeSymbol("B");
    mySymbol typeNameC = MySymbol_MakeSymbol("C");
    myDecList typeDecs = makeLegalRecursiveNamedDecs(typeNameA, typeNameB, typeNameC);


    bool result = MySemantic_Decs_Recursive(typeDecs);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecsRecursive_IllegalRecursiveTypes_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    mySymbol typeNameB = MySymbol_MakeSymbol("B");
    mySymbol typeNameC = MySymbol_MakeSymbol("C");
    myDecList typeDecsOne = makeIllegalRecursiveNamedDecs_Manual(
        typeNameA, typeNameB, typeNameC);
    myDecList typeDecsTwo = makeIllegalRecursiveNamedDecs_Recursive();



    bool resultOne = MySemantic_Decs_Recursive(typeDecsOne);
    bool resultTwo = MySemantic_Decs_Recursive(typeDecsTwo);

    CU_ASSERT_EQUAL(resultOne, false);
    CU_ASSERT_EQUAL(resultTwo, false);
}

void test_MySemanticDecsRecursive_LegalRecursiveFuncs_ReturnTrue(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol funcNameA = MySymbol_MakeSymbol("A");
    mySymbol funcNameB = MySymbol_MakeSymbol("B");
    mySymbol funcNameC = MySymbol_MakeSymbol("C");
    myDecList funcDecs = makeLegalRecursiveFuncDecs(funcNameA, funcNameB, funcNameC);


    bool result = MySemantic_Decs_Recursive(funcDecs);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecsRecursive_IllegalRecursiveFuncs_ReturnFalse(void)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());

    mySymbol funcNameA = MySymbol_MakeSymbol("A");
    mySymbol funcNameB = MySymbol_MakeSymbol("B");
    mySymbol funcNameC = MySymbol_MakeSymbol("C");
    myDecList funcDecs = makeIllegalRecursiveFuncDecs();


    bool result = MySemantic_Decs_Recursive(funcDecs);

    CU_ASSERT_EQUAL(result, false);
}

//////////////////////////////////////////////////////////////////////

int main ()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return -1;

    CU_TestInfo tests[] = {
        { "", test_MySemanticDecTypeOnePass_ByDefault_ReturnTrueAndAddNullType },

        { "", test_MySemanticDecFuncProcedureOnePass_ParamTypesNotDefined_ReturnFalseAndNotAdd },
        { "", test_MySemanticDecFuncProcedureOnePass_ParamTypesDefined_ReturnTrueAndAdded },

        { "", test_MySemanticDecFuncFunctionOnePass_ParamTypesNotDefined_ReturnFalseAndFuncNotAdd },
        { "", test_MySemanticDecFuncFunctionOnePass_ReturnTypesNotDefined_ReturnFalseAndFuncNotAdd },
        { "", test_MySemanticDecFuncFunctionOnePass_TypesAllDefined_ReturnTrueAndFunctionAdded },

                        //////////////////

        { "", test_MySemanticDecTypeNamedTwoPass_TypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeNamedTwoPass_TypeDefinedBodyReal_ReturnTrueAndTypeSet },
        { "", test_MySemanticDecTypeNamedTwoPass_TypeDefinedBodyEmpty_ReturnTrueAndTypeBLinkToTypeA },
        { "", test_MySemanticDecTypeNamedPass_ManuallyRecursiveNamedType_Succeed },
        { "", test_MySemanticDecTypeNamedPass_RecursiveNamed_ReturnFalse },

        { "", test_MySemanticDecTypeRecordTwoPass_FieldTypesNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeRecordTwoPass_FieldTypesDefined_ReturnTrueAndSetType },
        { "", test_MySemanticDecTypeRecordTwoPass_ManuallyRecursiveType_ReturnTrueAndLink },
        { "", test_MySemanticDecTypeRecordTwoPass_RecursiveType_ReturnTrueAndLink },

        { "", test_MySemanticDecTypeArrayTwoPass_ElementTypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeArray_ElementTypeDefinedAndNotEmpty_ReturnTrueAndSetType },
        { "", test_MySemanticDecTypeArray_ManuallyRecursiveType_ReturnTrueAndLinkToElementType },
        { "", test_MySemanticDecTypeArray_RecursievType_ReturnFalse },

        { "", test_MySemanticDecFuncProcedureTwoPass_BodyTypeNotNoReturn_ReturnFalse },
        { "", test_MySemanticDecFuncProcedureTwoPass_BodyTypeNoReturn_ReturnTrue },
        { "", test_MySemanticDecFuncProcedureTwoPass_RecursiveProcedure_ReturnTrue },
        { "", test_MySemanticDecFuncProcedureTwoPass_ManuallyRecursiveProcedure_ReturnTrue },

        { "", test_MySemanticDecFuncFunctionTwoPass_BodyIllegal_ReturnFalse },
        { "", test_MySemanticDecFuncFunctionTwoPass_ReturnTypeNotMatches_ReturnFalse },
        { "", test_MySemanticDecFuncFunctionTwoPass_ReturnTypeMatches_ReturnTrue },
        { "", test_MySemanticDecFuncFunctionTwoPass_ReturnTypeRecordBodyNil_ReturnTrue },
        { "", test_MySemanticDecFuncFunctionTwoPass_RecursiveFunction_ReturnTrue },
        { "", test_MySemanticDecFuncFunctionTwoPass_ManuallyRecursiveFunction_ReturnTrue },

        { "", test_MySemanticDecsRecursive_LegalVars_ReturnTrue },
        { "", test_MySemanticDecsRecursive_IllegalVars_ReturnFalse },
        { "", test_MySemanticDecsRecursive_LegalRecursiveTypes_ReturnTrue },
        { "", test_MySemanticDecsRecursive_IllegalRecursiveTypes_ReturnFalse },
        { "",test_MySemanticDecsRecursive_LegalRecursiveFuncs_ReturnTrue },
        { "", test_MySemanticDecsRecursive_IllegalRecursiveFuncs_ReturnFalse }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;

    if (!runTests(&suite))  return -1;
    freeMemoryChain();
    return 0;
}