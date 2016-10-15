#include "recursiveFile.h"
#include "typeMaker.h"
#include "typeChecker.h"
#include "myEnvironment.h"
#include "mySemantic.h"
#include "myError.h"

#include <stdlib.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////////
//  private function in mySemantic.c module

void AddOneProcedure(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec);
void AddOneFunction(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);
myTypeList getTypesFromTyFields(
    myTable typeEnv, myTyFieldList funcFields);
bool isFuncParamTypesDefined(myTable typeEnv, myTyFieldList funcFields);
void addFormalsToScope(
    myTable varAndFuncEnv, myTable typeEnv, myTyFieldList formals);
myType makeRecordFromTyFields(myTable typeEnv, myTyFieldList tyFields);

/////////////////

bool isTypeDefined(myTable typeEnv, mySymbol typeName)
{
    assert (typeEnv && typeName);

    return MyEnvironment_getTypeFromName(typeEnv, typeName) != NULL;
}

myType getActualType(myTable typeEnv, mySymbol typeName)
{
    myType type = MyEnvironment_getTypeFromName(typeEnv, typeName);
    while(isTypeNamed(type))
    {
        type = type->u.typeNamed->type;
    }
    return type;
}

/////////////////////////////////////////////////////////////////////

//  FORM:
//      type type-id1 = type-id2
//  DO:
//      1.add an empty type to the type environment.
//  RETURN:
//      always return true.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_OnePass(myTable typeEnv, myTypeDec typeDec)
{
    if (typeDec == NULL)    return true;

    mySymbol newTypeName = typeDec->name;
    myType emptyType = makeType_Empty();
    MyEnvironment_addNewType(typeEnv, newTypeName, emptyType);
    return true;
}

////////////////////////////////////////

//  forwards
void AddOneProcedure_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec);

////////////////

//  FORM:
//      function id (tyfields) = exp
//  DO:
//      1.check whether types in `tyfields` have already been defined.
//      2.add a procedure to the function environment.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Func_Procedure_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec)
{
    bool isParamTypesDefined =
        isFuncParamTypesDefined(typeEnv, procedureDec->tyFieldList);
        
    if (isParamTypesDefined)
    {
        AddOneProcedure_OnePass(varAndFuncEnv, typeEnv, procedureDec);
        return true;
    }
    else
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCDEC_PROCEDURE__PARAM_TYPE_NOT_DEFINED,
            MySymbol_GetName(procedureDec->name));
        return false;
    }
}

void AddOneProcedure_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec)
{
    AddOneProcedure(varAndFuncEnv, typeEnv, procedureDec);
}

//////////////////////////////////////////////////

//  forwards
void AddOneFunction_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);
void processFunctionErrors_OnePass(
    bool isParamTypesDefined, bool isReturnTypeDefined, mySymbol functionName);

/////////////////

//  FORM:
//      function id (tyfields) : type-id = exp
//  DO:
//      1.check whether types in `tyfields` have already been defined.
//      2.check whether function return type is already defined.
//      3.add a function to the function environment.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Func_Function_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec)
{
    bool isParamTypesDefined =
        isFuncParamTypesDefined(typeEnv, functionDec->tyFieldList);
    bool isReturnTypeDefined = 
        isTypeDefined(typeEnv, functionDec->returnType);
    
    if (isParamTypesDefined && isReturnTypeDefined)
    {
        AddOneFunction_OnePass(varAndFuncEnv, typeEnv, functionDec);
        return true;
    }
    else
    {
        processFunctionErrors_OnePass(
            isParamTypesDefined, isReturnTypeDefined, functionDec->name);
        return false;
    }
}

void AddOneFunction_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec)
{
    AddOneFunction(varAndFuncEnv, typeEnv, functionDec);
}

void processFunctionErrors_OnePass(
    bool isParamTypesDefined, bool isReturnTypeDefined, mySymbol functionName)
{
    myString functionNameString = MySymbol_GetName(functionName);

    if (!isParamTypesDefined)
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCDEC_FUNCTION__PARAM_TYPES_NOT_DEFINED,
            functionNameString);
    if (!isReturnTypeDefined)
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCDEC_FUNCTION__RETURN_TYPE_NOT_DEFINED,
            functionNameString);
}

///////////////////////////////////////////////////

//  a delagate
bool MySemantic_Dec_Func_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myFuncDec funcDec)
{
    if (funcDec == NULL)        return true;

    switch (funcDec->kind)
    {
        case ProcedureDec:
            return MySemantic_Dec_Func_Procedure_OnePass(
                varAndFuncEnv, typeEnv, funcDec->u.procedureDec);
        case FunctionDec:
            return MySemantic_Dec_Func_Function_OnePass(
                varAndFuncEnv, typeEnv, funcDec->u.functionDec);
        default:
            assert (false);
    }
}

/////////////////////////////////////////////////

//  a delegate function.
bool MySemantic_Dec_FuncOrType_OnePass(myTable varAndFuncEnv, myTable typeEnv, myDec dec)
{
    switch (dec->kind)
    {
        case TypeDec:
            return MySemantic_Dec_Type_OnePass(typeEnv, dec->u.tyDec);
        case FuncDec:
            return MySemantic_Dec_Func_OnePass(varAndFuncEnv, typeEnv, dec->u.funcDec);
        default:
            assert(false);
    }
}

/////////////////////////////////////////////////////////////////

//  FORM:
//      type type-id1 = type-id2
//  DO:
//      1.check whether type `type-id2` is already defined and is not empty type.
//      2.set this empty type as the same type of `type-id2`.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      
bool MySemantic_Dec_Type_Named_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol existedTypeName)
{
    assert (newTypeName && existedTypeName);

    if (isTypeDefined(typeEnv, existedTypeName))
    {
        myType existedActualType = getActualType(typeEnv, existedTypeName);
        bool result =
            MyEnvironment_setTypeFromName(typeEnv, newTypeName, existedActualType);
        assert (result);    //  OnePassed before

        return true;
    }
    else
    {
        MyError_pushErrorCodeWithMsg(ERROR__TYPEDEC_NAMED__TYPE_NOT_DEFINED, 
            MySymbol_GetName(existedTypeName));
        return false;
    }
}

//////////////////////////////////////////////////

//  FORM:
//      type type-id = { tyfields }
//  DO:
//      1.check whether types in record fields have already been defined.
//      2.set this empty type to the new type.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_Record_TwoPass(
    myTable typeEnv, mySymbol newTypeName, myTyFieldList fields)
{
    myType recordType = makeRecordFromTyFields(typeEnv, fields);

    if (recordType == SEMANTIC_ERROR)
        return false;
    else
    {
        bool result =
            MyEnvironment_setTypeFromName(typeEnv, newTypeName, recordType);
        assert (result);

        return true;
    }
}

//////////////////////////////////////////////////

//  FORM:
//      type type-id1 = array of type-id2
//  DO:
//      1.check whether the `type-id2` type is already defined.
//      2.set this empty type to this type.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_Array_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol elementTypeName)
{
    if (isTypeDefined(typeEnv, elementTypeName))
    {
        myType elementType = MyEnvironment_getTypeFromName(typeEnv, elementTypeName);
        myType newArrayType = makeType_Array(elementType);
        MyEnvironment_setTypeFromName(typeEnv, newTypeName, newArrayType);
        return true;
    }
    else
    {
        MyError_pushErrorCodeWithMsg(ERROR__TYPEDEC_ARRAY__TYPE_NOT_DEFINED,
            MySymbol_GetName(elementTypeName));
        return false;
    }
}


bool MySemantic_Dec_Type_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myTypeDec typeDec)
{
    mySymbol newTypeName = typeDec->name;
    switch (typeDec->type->kind)
    {
        case NamedType:
            return MySemantic_Dec_Type_Named_TwoPass(
                typeEnv, newTypeName, typeDec->type->u.typeName);
        case RecordType:
            return MySemantic_Dec_Type_Record_TwoPass(typeEnv,
                newTypeName, typeDec->type->u.tyFieldList);
        case ArrayType:
            return MySemantic_Dec_Type_Array_TwoPass(typeEnv,
                newTypeName, typeDec->type->u.arrayTypeName);
        default:
            assert(false);
    }
}

#error "test one and two pass together"

//  a delegate function.
bool MySemantic_Dec_FuncOrType_TwoPass(myTable varAndFuncEnv, myTable typeEnv, myDec dec)
{
    switch (dec->kind)
    {
        case TypeDec:
            return MySemantic_Dec_Type_TwoPass(varAndFuncEnv, typeEnv, dec->u.tyDec);
        /*case FuncDec:
            return MySemantic_Dec_Func_TwoPass(varAndFuncEnv, typeEnv, dec->u.funcDec);*/
        default:
            assert(false);
    }
}

/////////////////////////////////////////////////////////////////

//  forwards
bool PassTemplate(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs,
    bool (*passFunc)(myTable, myTable, myDec));

///////////////

bool MySemantic_FuncsOrTypes_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    return PassTemplate(
        varAndFuncEnv, typeEnv, decs, MySemantic_Dec_FuncOrType_OnePass);
}

bool MySemantic_FuncsOrTypes_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    return PassTemplate(
        varAndFuncEnv, typeEnv, decs, MySemantic_Dec_FuncOrType_TwoPass);
}

bool PassTemplate(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs,
    bool (*passFunc)(myTable, myTable, myDec))
{
    while (decs)
    {
        if (!(*passFunc)(varAndFuncEnv, typeEnv, decs->dec))
            return false;
        decs = decs->next;
    }
    return true;
}

/////////////////////////////////////////////////////

bool MySemantic_Decs_Recursive(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    //  todo:
    //  devide decs into some consecutive parts and process
    //  (options)check if all types and functions are not empty
}