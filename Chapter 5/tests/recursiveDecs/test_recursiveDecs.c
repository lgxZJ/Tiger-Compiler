#include "../testHelper.h"

#include "../../recursiveDecs.h"
#include "../../myEnvironment.h"
#include "../../abstractSyntaxMaker.h"
#include "../../typeChecker.h"
#include "../../typeMaker.h"

//////////////////////////////////////////////////////////////////////
//  forwards of tested functions
bool MySemantic_Dec_Type_OnePass(
    myTable typeEnv, myTypeDec typeDec);
bool MySemantic_Dec_Func_Procedure_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);
bool MySemantic_Dec_Type_Named_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol existedTypeName);
bool MySemantic_Dec_Type_Record_TwoPass(
    myTable typeEnv, mySymbol newTypeName, myTyFieldList fields);
bool MySemantic_Dec_Type_Array_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol elementTypeName);
bool MySemantic_Dec_Type_TwoPass(myTable typeEnv, myTypeDec typeDec);
bool MySemantic_Dec_Func_Procedure_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec);
bool MySemantic_Dec_Func_Function_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);
bool MySemantic_Decs_Recursive(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs);

//////////////////////////////////////////////////////////////////////

myPos makeOnePos()
{
    myPos pos;
    pos.column = pos.line;
    return pos;
}

myExp makeOneExp_NoValue(void)
{
    return makeMyExp_NoValue(
        makeOnePos(), makeMyNoValueExp());
}

myExp makeOneExp_Integer(void)
{
    return makeMyExp_IntegerLiteral(
            makeOnePos(),
            makeMyIntegerLiteralExp(1));
}

myExp makeOneIllegalExp_Integer(void)
{
    return makeMyExp_IfThenElse(makeOnePos(),
        makeMyIfThenElseExp(makeOneExp_NoValue(),
            makeOneExp_Integer(), makeOneExp_Integer()));
}

mySymbol makeOneSymbol(void)
{
    return MySymbol_MakeSymbol("randomSymbol");
}

mySymbol makeSymbol_Int(void)
{
    return MySymbol_MakeSymbol("int");
}

myProcedureDec makeOneProcedureDec_Body(myExp body)
{
    return makeMyProcedureDec(
        MySymbol_MakeSymbol("procedureName"),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        body);
}

myFunctionDec makeOneFunctionDec_Body(myExp body, mySymbol returnTypeName)
{
    return makeMyFunctionDec(
        MySymbol_MakeSymbol("procedureName"),
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        returnTypeName,
        body);
}

myFunctionDec makeOneFunctionDec_Int(
    mySymbol functionName, myExp body, mySymbol returnTypeName)
{
    return makeMyFunctionDec(
        functionName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        returnTypeName,
        body);
}

myProcedureDec makeOneProcedureDec_NoReturn(void)
{
    return makeOneProcedureDec_Body(makeOneExp_NoValue());
}

myProcedureDec makeOneProcedureDec_Integer(void)
{
    return makeOneProcedureDec_Body(makeOneExp_Integer());
}

myExp makeOneExp_Record(mySymbol recordTypeName)
{
    return makeMyExp_RecordCreation(makeOnePos(),
        makeMyRecordCreationExp_NoField(
            makeMyNoFieldRecordCreationExp(recordTypeName)));
}

myExp makeOneExp_NoParamprocedureCall(mySymbol funcName)
{
    return makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
        makeMyNoParamFunctionCallExp(funcName)));
}

myDecList makeLegalVars(void)
{
    return makeMyDecList(makeMyDec_Var(makeOnePos(),
        makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_Integer()))),
        makeMyDecList(makeMyDec_Var(makeOnePos(),
            makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_Integer()))),
            NULL));
}

myDecList makeIllegalVars(void)
{
    return makeMyDecList(makeMyDec_Var(makeOnePos(),
        makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_Integer()))),
        makeMyDecList(makeMyDec_Var(makeOnePos(),
            makeMyVarDec_ShortForm(makeMyShortFormVar(makeOneSymbol(), makeOneExp_NoValue()))),
            NULL));
}

myDecList makeLegalRecursiveNamedDecs(mySymbol A, mySymbol B, mySymbol C)
{
    return makeMyDecList(makeMyDec_Type(makeOnePos(),
        makeMyTyDec(A, makeMyTy_Named(makeOnePos(), B))),
            makeMyDecList(makeMyDec_Type(makeOnePos(),
            makeMyTyDec(B, makeMyTy_Named(makeOnePos(), C))),
                makeMyDecList(makeMyDec_Type(makeOnePos(),
                makeMyTyDec(C, makeMyTy_Named(makeOnePos(), makeSymbol_Int()))),
            NULL)));
}

myDecList makeIllegalRecursiveNamedDecs_Manual(
    mySymbol typeNameA, mySymbol typeNameB, mySymbol typeNameC)
{
    return makeMyDecList(makeMyDec_Type(makeOnePos(),
        makeMyTyDec(typeNameA, makeMyTy_Named(makeOnePos(), typeNameB))),
            makeMyDecList(makeMyDec_Type(makeOnePos(),
            makeMyTyDec(typeNameB, makeMyTy_Named(makeOnePos(), typeNameC))),
                makeMyDecList(makeMyDec_Type(makeOnePos(),
                makeMyTyDec(typeNameC, makeMyTy_Named(makeOnePos(), typeNameA))),
            NULL)));
}

myDecList makeIllegalRecursiveNamedDecs_Recursive(void)
{
    mySymbol sametypeName = makeOneSymbol();
    return makeMyDecList(makeMyDec_Type(makeOnePos(),
        makeMyTyDec(sametypeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int()))),
            makeMyDecList(makeMyDec_Type(makeOnePos(),
            makeMyTyDec(sametypeName, makeMyTy_Named(makeOnePos(), MySymbol_MakeSymbol("string")))), NULL));
}

myDecList makeLegalRecursiveFuncDecs(
    mySymbol funcNameA, mySymbol funcNameB, mySymbol funcNameC)
{
    return makeMyDecList(makeMyDec_Func(makeOnePos(), 
        makeMyFuncDec_Procedure(makeMyProcedureDec(funcNameA, NULL, 
            makeOneExp_NoParamprocedureCall(funcNameB)))), 
        makeMyDecList(makeMyDec_Func(makeOnePos(), 
            makeMyFuncDec_Procedure(makeMyProcedureDec(funcNameB, NULL, 
                makeOneExp_NoParamprocedureCall(funcNameC)))), 
        makeMyDecList(makeMyDec_Func(makeOnePos(), 
            makeMyFuncDec_Procedure(makeMyProcedureDec(funcNameC, NULL, 
                makeOneExp_NoParamprocedureCall(funcNameA)))), NULL)));
}

myDecList makeIllegalRecursiveFuncDecs(void)
{
    mySymbol sameFuncName = makeOneSymbol();
    return makeMyDecList(makeMyDec_Func(makeOnePos(), 
        makeMyFuncDec_Procedure(makeMyProcedureDec(sameFuncName, NULL, 
            makeOneExp_NoValue()))), 
        makeMyDecList(makeMyDec_Func(makeOnePos(), 
            makeMyFuncDec_Procedure(makeMyProcedureDec(sameFuncName, NULL, 
                makeOneExp_NoValue()))), NULL));
}

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeOnePass_ByDefault_ReturnTrueAndAddNullType(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeName = MySymbol_MakeSymbol("typeName");
    myTypeDec typeDec = makeMyTyDec(typeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int()));


    bool result = MySemantic_Dec_Type_OnePass(typeEnv, typeDec);

    myType typeGot = MyEnvironment_getTypeFromName(typeEnv, typeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(typeGot->u.typeNamed->type, NULL);
} 

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecFuncProcedureOnePass_ParamTypesNotDefined_ReturnFalseAndNotAdd(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol procedureName = MySymbol_MakeSymbol("procedureName");
    mySymbol notDefinedTypeName=  MySymbol_MakeSymbol("not defined");
    myProcedureDec dec = makeMyProcedureDec(
        procedureName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), notDefinedTypeName), NULL),
        makeOneExp_NoValue());


    bool result = MySemantic_Dec_Func_Procedure_OnePass(varAndFuncEnv, typeEnv, dec);

    myVarAndFuncEntry entryGot = MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, procedureName);
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT_EQUAL(entryGot, NULL);
}

void test_MySemanticDecFuncProcedureOnePass_ParamTypesDefined_ReturnTrueAndAdded(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol procedureName = MySymbol_MakeSymbol("procedureName");
    myProcedureDec dec = makeMyProcedureDec(
        procedureName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        makeOneExp_NoValue());


    bool result = MySemantic_Dec_Func_Procedure_OnePass(varAndFuncEnv, typeEnv, dec);

    myVarAndFuncEntry entryGot = MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, procedureName);
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
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol functionName = MySymbol_MakeSymbol("name");
    myFunctionDec dec = makeMyFunctionDec(
        functionName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL),
        makeSymbol_Int(),
        makeOneExp_NoValue());

    bool result =
        MySemantic_Dec_Func_Function_OnePass(varAndFuncEnv, typeEnv, dec);

    myVarAndFuncEntry entry =
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, functionName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_NOT_EQUAL(entry, NULL);
    CU_ASSERT(myEnvironment_isFuncEntry(entry));
}

//  a parameterized test
void test_IllegalFunctionDecOnePass_ReturnFalseAndFunctionNotAdd(
    mySymbol paramTypeName, mySymbol returnTypeName)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol functionName = MySymbol_MakeSymbol("name");
    myFunctionDec dec = makeMyFunctionDec(
        functionName,
        makeMyTyFieldList(makeMyTyField(makeOneSymbol(), paramTypeName), NULL),
        returnTypeName,
        makeOneExp_NoValue());

    bool result =
        MySemantic_Dec_Func_Function_OnePass(varAndFuncEnv, typeEnv, dec);

    myVarAndFuncEntry entry =
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, functionName);
    CU_ASSERT_EQUAL(result, false);
    CU_ASSERT_EQUAL(entry, NULL);
}

//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeNamedTwoPass_TypeNotDefined_ReturnFalse(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol undefinedTypeName = MySymbol_MakeSymbol("undefined");
    mySymbol typeName = makeOneSymbol(); 

    bool result = MySemantic_Dec_Type_Named_TwoPass(
        typeEnv, typeName , undefinedTypeName);

    CU_ASSERT_EQUAL(result, false);
}


void test_MySemanticDecTypeNamedTwoPass_TypeDefinedBodyReal_ReturnTrueAndTypeSet(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol newTypeName = MySymbol_MakeSymbol("newType");
    myTypeDec typeDec = makeMyTyDec(
        newTypeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int()));
    MySemantic_Dec_Type_OnePass(typeEnv, typeDec);


    bool result = MySemantic_Dec_Type_Named_TwoPass(
        typeEnv, newTypeName, makeSymbol_Int());

    myType typeGot = MyEnvironment_getTypeFromName(typeEnv, newTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeNamed(typeGot));   
}

void test_MySemanticDecTypeNamedTwoPass_TypeDefinedBodyEmpty_ReturnTrueAndTypeBLinkToTypeA(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeNameB = MySymbol_MakeSymbol("typeB");
    mySymbol typeNameA = MySymbol_MakeSymbol("typeA");
    myTypeDec typeDec = makeMyTyDec(
        typeNameB, makeMyTy_Named(makeOnePos(), typeNameA));
    MySemantic_Dec_Type_OnePass(typeEnv,
        makeMyTyDec(typeNameA, makeMyTy_Named(makeOnePos(), makeSymbol_Int())));
    MySemantic_Dec_Type_OnePass(typeEnv, typeDec);


    bool result = MySemantic_Dec_Type_Named_TwoPass(
        typeEnv, typeNameB, typeNameA);

    myType typeGotB = MyEnvironment_getTypeFromName(typeEnv, typeNameB);
    myType typeGotA = MyEnvironment_getTypeFromName(typeEnv, typeNameA);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(typeGotA, typeGotB->u.typeNamed->type);
}


void test_MySemanticDecTypeNamedPass_ManuallyRecursiveNamedType_Succeed(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    mySymbol typeNameB = MySymbol_MakeSymbol("B");
    myTypeDec typeDecFirst = makeMyTyDec(
        typeNameA, makeMyTy_Named(makeOnePos(), typeNameB));
    myTypeDec typeDecSecond = makeMyTyDec(
        typeNameB, makeMyTy_Named(makeOnePos(), makeSymbol_Int()));


    MySemantic_Dec_Type_OnePass(typeEnv, typeDecFirst);
    MySemantic_Dec_Type_OnePass(typeEnv, typeDecSecond);
    MySemantic_Dec_Type_TwoPass(typeEnv, typeDecFirst);
    MySemantic_Dec_Type_TwoPass(typeEnv, typeDecSecond);

    myType typeA = MyEnvironment_getTypeFromName(typeEnv, typeNameA);
    myType typeB = MyEnvironment_getTypeFromName(typeEnv, typeNameB);
    CU_ASSERT(isTypeInt(typeB->u.typeNamed->type));
    CU_ASSERT(isTypeEqual(
        typeA->u.typeNamed->type->u.typeNamed->type, 
        typeB->u.typeNamed->type));
}

void test_MySemanticDecTypeNamedPass_RecursiveNamed_ReturnFalse(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    myTypeDec typeDec = makeMyTyDec(
        typeNameA, makeMyTy_Named(makeOnePos(), typeNameA));


    MySemantic_Dec_Type_OnePass(typeEnv, typeDec);
    bool result = MySemantic_Dec_Type_TwoPass(typeEnv, typeDec);

    CU_ASSERT_EQUAL(result, false);
}

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeRecordTwoPass_FieldTypesNotDefined_ReturnFalse(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol varName = makeOneSymbol();
    mySymbol typeName = MySymbol_MakeSymbol("typeA");
    mySymbol recordTypeName = makeOneSymbol();
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(varName, typeName), NULL);
    MySemantic_Dec_Type_OnePass(typeEnv, makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields)));

    bool result = MySemantic_Dec_Type_Record_TwoPass(
        typeEnv, recordTypeName, tyFields);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeRecordTwoPass_FieldTypesDefined_ReturnTrueAndSetType(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), MySymbol_MakeSymbol("int")),
        NULL);
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    MySemantic_Dec_Type_OnePass(typeEnv, makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields)));
    

    bool result = MySemantic_Dec_Type_Record_TwoPass(
        typeEnv, recordTypeName, tyFields);

    myType recordType = MyEnvironment_getTypeFromName(typeEnv, recordTypeName); 
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeRecord(recordType));
    CU_ASSERT(isTypeInt(recordType->u.typeRecord->fieldList->head->type));
}

void test_MySemanticDecTypeRecordTwoPass_ManuallyRecursiveType_ReturnTrueAndLink(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeNameA = MySymbol_MakeSymbol("typeA");
    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    MySemantic_Dec_Type_OnePass(typeEnv,
        makeMyTyDec(typeNameA, makeMyTy_Named(makeOnePos(), recordTypeName)));

    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), makeSymbol_Int()), NULL);
    MySemantic_Dec_Type_OnePass(typeEnv, makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields)));
    

    MySemantic_Dec_Type_Named_TwoPass(
        typeEnv, typeNameA, recordTypeName);
    bool result = MySemantic_Dec_Type_Record_TwoPass(
        typeEnv, recordTypeName, tyFields);

    myType typeA = MyEnvironment_getTypeFromName(typeEnv, typeNameA);
    myType recordType = MyEnvironment_getTypeFromName(typeEnv, recordTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(
        typeA->u.typeNamed->type,
        recordType);
}

void test_MySemanticDecTypeRecordTwoPass_RecursiveType_ReturnTrueAndLink(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol recordTypeName = MySymbol_MakeSymbol("recursiveType");
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(makeOneSymbol(), recordTypeName), NULL);
    myTypeDec typeDec = makeMyTyDec(
        recordTypeName, makeMyTy_Record(makeOnePos(), tyFields));

    MySemantic_Dec_Type_OnePass(typeEnv, typeDec);
    bool result =
        MySemantic_Dec_Type_Record_TwoPass(typeEnv, recordTypeName, tyFields);

    myType typeGot = MyEnvironment_getTypeFromName(typeEnv, recordTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT_EQUAL(typeGot->u.typeRecord->fieldList->head->type, typeGot);
}

////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeArrayTwoPass_ElementTypeNotDefined_ReturnFalse(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("elementType");

    bool result = MySemantic_Dec_Type_Array_TwoPass(
        typeEnv, newTypeName, elementTypeName);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeArray_ElementTypeDefinedAndNotEmpty_ReturnTrueAndSetType(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol newTypeName = MySymbol_MakeSymbol("newTypeName");
    mySymbol elementTypeName = MySymbol_MakeSymbol("int");
    MySemantic_Dec_Type_OnePass(typeEnv, makeMyTyDec(
        newTypeName, makeMyTy_Array(makeOnePos(), elementTypeName)));


    bool result = MySemantic_Dec_Type_Array_TwoPass(
        typeEnv, newTypeName, elementTypeName);

    myType arrayType = MyEnvironment_getTypeFromName(typeEnv, newTypeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeArray(arrayType));
    CU_ASSERT(isTypeInt(arrayType->u.typeArray->type));
}

void test_MySemanticDecTypeArray_ManuallyRecursiveType_ReturnTrueAndLinkToElementType(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol outerArrayTypeName = MySymbol_MakeSymbol("newTypeName");

    mySymbol innerArrayTypeName = MySymbol_MakeSymbol("files");

    MySemantic_Dec_Type_OnePass(typeEnv, makeMyTyDec(
        outerArrayTypeName, makeMyTy_Array(makeOnePos(), innerArrayTypeName)));
    MySemantic_Dec_Type_OnePass(typeEnv, makeMyTyDec(
        innerArrayTypeName, makeMyTy_Array(makeOnePos(), MySymbol_MakeSymbol("int"))));
    

    bool resultOuter = MySemantic_Dec_Type_Array_TwoPass(
        typeEnv, outerArrayTypeName, innerArrayTypeName); 
    bool resultInner = MySemantic_Dec_Type_Array_TwoPass(
        typeEnv, innerArrayTypeName, makeSymbol_Int());

    myType outerArrayType = MyEnvironment_getTypeFromName(typeEnv, outerArrayTypeName);
    myType innerArrayType = MyEnvironment_getTypeFromName(typeEnv, innerArrayTypeName);  
    CU_ASSERT_EQUAL(resultInner, true);
    CU_ASSERT_EQUAL(resultOuter, true);
    CU_ASSERT_EQUAL(outerArrayType->u.typeArray->type, innerArrayType);
}

void test_MySemanticDecTypeArray_RecursievType_ReturnFalse(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol arrayTypeName = MySymbol_MakeSymbol("arrayTypeName");
    myTypeDec recursiveTypeDec = makeMyTyDec(
        arrayTypeName, makeMyTy_Array(makeOnePos(), arrayTypeName));

    MySemantic_Dec_Type_OnePass(typeEnv, recursiveTypeDec);
    bool result =
        MySemantic_Dec_Type_Array_TwoPass(typeEnv, recursiveTypeDec->name,
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
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol functionName = MySymbol_MakeSymbol("procedureName"); 
    myProcedureDec proceOne = makeMyProcedureDec(
        functionName,
        NULL,
        makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_NoParam(
            makeMyNoParamFunctionCallExp(functionName))));


    MySemantic_Dec_Func_Procedure_OnePass(varAndFuncEnv, typeEnv, proceOne);
    bool result =
        MySemantic_Dec_Func_Procedure_TwoPass(varAndFuncEnv, typeEnv, proceOne);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncProcedureTwoPass_ManuallyRecursiveProcedure_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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


    MySemantic_Dec_Func_Procedure_OnePass(varAndFuncEnv, typeEnv, proceOneA);
    MySemantic_Dec_Func_Procedure_OnePass(varAndFuncEnv, typeEnv, proceOneB);
    bool resultA = 
        MySemantic_Dec_Func_Procedure_TwoPass(varAndFuncEnv, typeEnv, proceOneA);
    bool resultB =
        MySemantic_Dec_Func_Procedure_TwoPass(varAndFuncEnv, typeEnv, proceOneB);

    CU_ASSERT_EQUAL(resultA, true);
    CU_ASSERT_EQUAL(resultB, true);
}

//  a parameterized test
void testProcedureTwoPass_BodyExp_ReturnTrueOrFalse(myExp body, bool value)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myProcedureDec dec = makeOneProcedureDec_Body(body);

    bool result = MySemantic_Dec_Func_Procedure_TwoPass(varAndFuncEnv, typeEnv, dec);

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
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol recordTypeName = MySymbol_MakeSymbol("recordType");
    myType recordType = makeType_Record(NULL);
    MyEnvironment_addNewType(typeEnv, recordTypeName, recordType); 

    myExp nilBody = makeMyExp_Nil(makeOnePos(), makeMyNilExp());
    myFunctionDec dec = makeOneFunctionDec_Body(nilBody, recordTypeName);


    bool result = MySemantic_Dec_Func_Function_TwoPass(varAndFuncEnv, typeEnv, dec);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncFunctionTwoPass_RecursiveFunction_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol functionName = MySymbol_MakeSymbol("functionName");
    myExp body = makeMyExp_FunctionCall(makeOnePos(), makeMyFunctionCallExp_Param(
        makeMyParamFunctionCallExp(functionName, 
        makeMyExpList(makeOneExp_Integer(), NULL))));
    myFunctionDec dec = makeOneFunctionDec_Int(functionName, body, makeSymbol_Int());
    

    MySemantic_Dec_Func_Function_OnePass(varAndFuncEnv, typeEnv, dec);
    bool result = MySemantic_Dec_Func_Function_TwoPass(varAndFuncEnv, typeEnv, dec);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecFuncFunctionTwoPass_ManuallyRecursiveFunction_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

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


    MySemantic_Dec_Func_Function_OnePass(varAndFuncEnv, typeEnv, funcOneA);
    MySemantic_Dec_Func_Function_OnePass(varAndFuncEnv, typeEnv, funcOneB);
    bool resultA = 
        MySemantic_Dec_Func_Function_TwoPass(varAndFuncEnv, typeEnv, funcOneA);
    bool resultB =
        MySemantic_Dec_Func_Function_TwoPass(varAndFuncEnv, typeEnv, funcOneB);

    CU_ASSERT_EQUAL(resultA, true);
    CU_ASSERT_EQUAL(resultB, true);
}

//  a parameterized test
void testFunctionTwoPass_BodyExp_ReturnTrueOrFalse(
    myExp body, mySymbol returnTypeName, bool value)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myFunctionDec dec = makeOneFunctionDec_Body(body, returnTypeName);

    bool result = MySemantic_Dec_Func_Function_TwoPass(varAndFuncEnv, typeEnv, dec);

    CU_ASSERT_EQUAL(result, value);
}

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecsRecursive_LegalVars_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myDecList varDecs = makeLegalVars();

    bool result = MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, varDecs);

    CU_ASSERT_EQUAL(result, true); 
}

void test_MySemanticDecsRecursive_IllegalVars_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();
    myDecList varDecs = makeIllegalVars();

    bool result = MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, varDecs);

    CU_ASSERT_EQUAL(result, false); 
}

void test_MySemanticDecsRecursive_LegalRecursiveTypes_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    mySymbol typeNameB = MySymbol_MakeSymbol("B");
    mySymbol typeNameC = MySymbol_MakeSymbol("C");
    myDecList typeDecs = makeLegalRecursiveNamedDecs(typeNameA, typeNameB, typeNameC);


    bool result = MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, typeDecs);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecsRecursive_IllegalRecursiveTypes_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeNameA = MySymbol_MakeSymbol("A");
    mySymbol typeNameB = MySymbol_MakeSymbol("B");
    mySymbol typeNameC = MySymbol_MakeSymbol("C");
    myDecList typeDecsOne = makeIllegalRecursiveNamedDecs_Manual(
        typeNameA, typeNameB, typeNameC);
    myDecList typeDecsTwo = makeIllegalRecursiveNamedDecs_Recursive();



    bool resultOne = MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, typeDecsOne);
    bool resultTwo = MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, typeDecsTwo);

    CU_ASSERT_EQUAL(resultOne, false);
    CU_ASSERT_EQUAL(resultTwo, false);
}

void test_MySemanticDecsRecursive_LegalRecursiveFuncs_ReturnTrue(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol funcNameA = MySymbol_MakeSymbol("A");
    mySymbol funcNameB = MySymbol_MakeSymbol("B");
    mySymbol funcNameC = MySymbol_MakeSymbol("C");
    myDecList funcDecs = makeLegalRecursiveFuncDecs(funcNameA, funcNameB, funcNameC);


    bool result = MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, funcDecs);

    CU_ASSERT_EQUAL(result, true);
}

void test_MySemanticDecsRecursive_IllegalRecursiveFuncs_ReturnFalse(void)
{
    myTable varAndFuncEnv = myEnvironment_BaseVarAndFunc();
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol funcNameA = MySymbol_MakeSymbol("A");
    mySymbol funcNameB = MySymbol_MakeSymbol("B");
    mySymbol funcNameC = MySymbol_MakeSymbol("C");
    myDecList funcDecs = makeIllegalRecursiveFuncDecs();


    bool result = MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, funcDecs);

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