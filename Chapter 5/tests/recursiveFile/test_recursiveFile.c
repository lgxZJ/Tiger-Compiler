#include "../testHelper.h"

#include "../../recursiveFile.h"
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
    myTable typeEnv, mySymbol newTypeName,mySymbol existedTypeName);
bool MySemantic_Dec_Type_Record_TwoPass(
    myTable typeEnv, mySymbol newTypeName, myTyFieldList fields);
bool MySemantic_Dec_Type_Array_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol elementTypeName);

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

mySymbol makeOneSymbol(void)
{
    return MySymbol_MakeSymbol("randomSymbol");
}

mySymbol makeSymbol_Int(void)
{
    return MySymbol_MakeSymbol("int");
}

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeOnePass_ByDefault_ReturnTrueAndAddEmptyTypeToEnvironment(void)
{
    myTable typeEnv = myEnvironment_BaseType();

    mySymbol typeName = MySymbol_MakeSymbol("typeName");
    myTypeDec typeDec = makeMyTyDec(typeName, makeMyTy_Named(makeOnePos(), makeSymbol_Int()));


    bool result = MySemantic_Dec_Type_OnePass(typeEnv, typeDec);

    myType typeGot = MyEnvironment_getTypeFromName(typeEnv, typeName);
    CU_ASSERT_EQUAL(result, true);
    CU_ASSERT(isTypeEmpty(typeGot));
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

void test_MySemanticDecTypeNamedTwoPass_TypeNotDefined_ReturnFalse(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol undefinedTypeName = MySymbol_MakeSymbol("undefined");
    mySymbol typeName = makeOneSymbol(); 

    bool result = MySemantic_Dec_Type_Named_TwoPass(
        typeEnv, typeName , undefinedTypeName);

    CU_ASSERT_EQUAL(result, false);
}


void test_MySemanticDecTypeNamedTwoPass_TypeDefinedAndNotEmptyType_ReturnTrueAndTypeSetted(void)
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
    CU_ASSERT(!isTypeEmpty(typeGot));
    CU_ASSERT(isTypeInt(typeGot));   
}

//////////////////////////////////////////////////////////////////////

void test_MySemanticDecTypeRecordTwoPass_FieldTypesNotDefined_ReturnFalse(void)
{
    myTable typeEnv = myEnvironment_BaseType();
    mySymbol varName = makeOneSymbol();
    mySymbol typeName = MySymbol_MakeSymbol("not defined type");
    mySymbol recordTypeName = makeOneSymbol();
    myTyFieldList tyFields = makeMyTyFieldList(
        makeMyTyField(varName, typeName), NULL);

    bool result = MySemantic_Dec_Type_Record_TwoPass(
        typeEnv, recordTypeName, tyFields);

    CU_ASSERT_EQUAL(result, false);
}

void test_MySemanticDecTypeRecordTwoPass_LegalParam_ReturnTrueAndSetEmptyToThisRecordType(void)
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
    CU_ASSERT(!isTypeEmpty(recordType));
    CU_ASSERT(isTypeInt(recordType->u.typeRecord->fieldList->head->type));
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

void test_MySemanticDecTypeArray_LegalParam_ReturnTrueAndSetEmptyTypeToThisArrayType(void)
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
    CU_ASSERT(!isTypeEmpty(arrayType));
    CU_ASSERT(isTypeArray(arrayType));
    CU_ASSERT(isTypeInt(arrayType->u.typeArray->type));
}

//////////////////////////////////////////////////////////////////////

int main ()
{
    CU_pSuite suite;
    if (!initialTests(&suite))  return -1;

    CU_TestInfo tests[] = {
        { "", test_MySemanticDecTypeOnePass_ByDefault_ReturnTrueAndAddEmptyTypeToEnvironment },

        { "", test_MySemanticDecFuncProcedureOnePass_ParamTypesNotDefined_ReturnFalseAndNotAdd },
        { "", test_MySemanticDecFuncProcedureOnePass_ParamTypesDefined_ReturnTrueAndAdded },

        { "", test_MySemanticDecFuncFunctionOnePass_ParamTypesNotDefined_ReturnFalseAndFuncNotAdd },
        { "", test_MySemanticDecFuncFunctionOnePass_ReturnTypesNotDefined_ReturnFalseAndFuncNotAdd },
        { "", test_MySemanticDecFuncFunctionOnePass_TypesAllDefined_ReturnTrueAndFunctionAdded },

        { "", test_MySemanticDecTypeNamedTwoPass_TypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeNamedTwoPass_TypeDefinedAndNotEmptyType_ReturnTrueAndTypeSetted },

        { "", test_MySemanticDecTypeRecordTwoPass_FieldTypesNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeRecordTwoPass_LegalParam_ReturnTrueAndSetEmptyToThisRecordType },

        { "", test_MySemanticDecTypeArrayTwoPass_ElementTypeNotDefined_ReturnFalse },
        { "", test_MySemanticDecTypeArray_LegalParam_ReturnTrueAndSetEmptyTypeToThisArrayType }
    };
    if (!addTests(&suite, tests, sizeof(tests) / sizeof(tests[0])))
        return -1;

    if (!runTests(&suite))  return -1;
    freeMemoryChain();
    return 0;
}