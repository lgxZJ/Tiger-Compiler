#include "recursiveDecs.h"
#include "typeMaker.h"
#include "typeChecker.h"
#include "myEnvironment.h"
#include "mySemantic.h"
#include "myError.h"
#include "abstractSyntaxMaker.h"

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
bool MySemantic_Dec_Var(myTable varAndFuncEnv, myTable typeEnv, myVarDec varDec);
myType getActualTypeFromName(myTable typeEnv, mySymbol typeName);
bool isExpOneTypeOrIllegal(
    myTable varAndFuncEnv, myTable typeEnv,
    myExp exp, enum TypeKind kind);
bool isExpNoReturn(myTable varAndFuncEnv, myTable typeEnv, myExp exp);
bool isTypeDefined(myTable typeEnv, mySymbol typeName);
bool isExpLegal(
    myTable varAndFuncEnv, myTable typeEnv, myExp exp);

/////////////////////////////////////////////////////////////////////

//  FORM:
//      type type-id1 = type-id2
//  DO:
//      1.add a type to the type environment without checking.
//  RETURN:
//      always return true.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_OnePass(myTable typeEnv, myTypeDec typeDec)
{
    if (typeDec == NULL)    return true;

    mySymbol newTypeName = typeDec->name;

    myType type;
    switch (typeDec->type->kind)
    {
        case NamedType:
            type = makeType_Named(typeDec->type->u.typeName, NULL);
            break;
        case RecordType:
            type = makeType_Record(NULL);
            break;
        case ArrayType:
            type = makeType_Array(NULL);
            break;
    }

    MyEnvironment_addNewType(typeEnv, newTypeName, type);
    return true;
}

//  forwards
void updateType_Named(
    myTable typeEnv, mySymbol newTypeName, mySymbol existedTypeName);
void processNamedDecErrors(
    bool isNamedTypeDefined, bool isRecursive, mySymbol existedTypeName);

//  FORM:
//      type type-id1 = type-id2
//  DO:
//      1.check whether type `type-id2` is already defined(which means can be found).
//      2.update this type content if `type-id2` exists.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_Named_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol existedTypeName)
{
    assert (newTypeName && existedTypeName);

    bool isNamedTypeDefined = isTypeDefined(typeEnv, existedTypeName);
    bool isRecursive = MySymbol_IsSymbolEqual(newTypeName, existedTypeName); 
    if (isNamedTypeDefined && !isRecursive)
    {
        updateType_Named(typeEnv, newTypeName, existedTypeName);
        return true;
    }
    else
    {
        processNamedDecErrors(isNamedTypeDefined, isRecursive, existedTypeName);
        return false;
    }
}

void updateType_Named(
    myTable typeEnv, mySymbol newTypeName, mySymbol existedTypeName)
{
    myType existedType =
        MyEnvironment_getTypeFromName(typeEnv, existedTypeName);
    myType newType = 
        MyEnvironment_getTypeFromName(typeEnv, newTypeName);
    newType->u.typeNamed->type = existedType;
}

void processNamedDecErrors(
    bool isNamedTypeDefined, bool isRecursive, mySymbol existedTypeName)
{
    if (!isNamedTypeDefined)
        MyError_pushErrorCodeWithMsg(ERROR__TYPEDEC_NAMED__TYPE_NOT_DEFINED, 
            MySymbol_GetName(existedTypeName));
    if (isRecursive)
        MyError_pushErrorCodeWithMsg(ERROR__RECURSIVE__ILLEGAL,
            MySymbol_GetName(existedTypeName));
}

//////////////////////////////////////////////////

//  forwards
myTypeFieldList makeTypeFieldsFromTyFields_TwoPass(
    myTable typeEnv, myTyFieldList tyFields);
void updateType_Record(
    myTable typeEnv, mySymbol newTypeName, myTypeFieldList typeFields);

//  FORM:
//      type type-id = { tyfields }
//  DO:
//      1.check whether types in record fields have already been defined.
//      2.update this empty type to the new type.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_Record_TwoPass(
    myTable typeEnv, mySymbol newTypeName, myTyFieldList fields)
{
    //  also checks whether field types defined
    myTypeFieldList typeFields =
        makeTypeFieldsFromTyFields_TwoPass(typeEnv, fields);

    if (typeFields != SEMANTIC_ERROR)            
    {
        updateType_Record(typeEnv, newTypeName, typeFields);
        return true;
    }
    else
        return false;
}

void updateType_Record(
    myTable typeEnv, mySymbol newTypeName, myTypeFieldList typeFields)
{
    myType newType = MyEnvironment_getTypeFromName(typeEnv, newTypeName);
    newType->u.typeRecord->fieldList = typeFields;
}

//  forwards
myTypeFieldList makeTypeFieldsFromTyFields_TwoPass(myTable typeEnv, myTyFieldList tyFields)
{
    //  end condition for recursive
    if (tyFields == NULL)   return NULL; 

    myTyField oneTyField = tyFields->field;

    if (isTypeDefined(typeEnv, oneTyField->typeName))
    {
        myTypeField oneTypeField = makeType_TypeField(
            oneTyField->varName,
            MyEnvironment_getTypeFromName(typeEnv, oneTyField->typeName));

        myTypeFieldList rests = makeTypeFieldsFromTyFields_TwoPass(
            typeEnv, tyFields->next);
        if (rests != SEMANTIC_ERROR)
            return makeType_TypeFieldList(oneTypeField, rests);
        else
            return SEMANTIC_ERROR;
    }
    else
    {
        MyError_pushErrorCodeWithMsg(ERROR__TYPEDEC_RECORD__TYPE_NOT_DEFINED,
            MySymbol_GetName(oneTyField->typeName));
        return SEMANTIC_ERROR;
    }
}

//////////////////////////////////////////////////

//  forwards
void updateType_Array(
    myTable typeEnv, mySymbol arrayTypeName, mySymbol elementTypeName);
void processArrayDecErrors(
    bool isElementTypeDefined, bool isArrayRecursive, mySymbol elementTypeName);

//  FORM:
//      type type-id1 = array of type-id2
//  DO:
//      1.check whether the `type-id2` type is already defined.
//      2.update this empty type to this type.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_Array_TwoPass(
    myTable typeEnv, mySymbol newTypeName, mySymbol elementTypeName)
{
    bool isElementTypeDefined = isTypeDefined(typeEnv, elementTypeName);
    bool isArrayRecursive = MySymbol_IsSymbolEqual(newTypeName, elementTypeName);

    if (isElementTypeDefined && !isArrayRecursive)
    {
        updateType_Array(typeEnv, newTypeName, elementTypeName);
        return true;
    }
    else
    {
        processArrayDecErrors(
            isElementTypeDefined, isArrayRecursive, elementTypeName);
        return false;
    }
}

void updateType_Array(
    myTable typeEnv, mySymbol arrayTypeName, mySymbol elementTypeName)
{
    myType elementType = MyEnvironment_getTypeFromName(typeEnv, elementTypeName);
    myType arrayType = MyEnvironment_getTypeFromName(typeEnv, arrayTypeName);
    arrayType->u.typeArray->type = elementType;
}

void processArrayDecErrors(
    bool isElementTypeDefined, bool isArrayRecursive, mySymbol elementTypeName)
{
    if (!isElementTypeDefined)
        MyError_pushErrorCodeWithMsg(ERROR__TYPEDEC_ARRAY__TYPE_NOT_DEFINED,
            MySymbol_GetName(elementTypeName));
    if (isArrayRecursive)
        MyError_pushErrorCodeWithMsg(ERROR__RECURSIVE__ILLEGAL,
            MySymbol_GetName(elementTypeName));
}

//////////////////////////////

bool MySemantic_Dec_Type_TwoPass(myTable typeEnv, myTypeDec typeDec)
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

////////////////////////////////////////
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

//  FORM:
//      function id (tyfields) = exp
//  DO:
//      1.check whether `exp` type is of NoReturn.
//      2.update this function in the function environment.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Func_Procedure_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myProcedureDec procedureDec)
{
    MySymbol_BeginScope(varAndFuncEnv);
    addFormalsToScope(varAndFuncEnv, typeEnv, procedureDec->tyFieldList);

    bool isBodyNoReturn = false;
    isBodyNoReturn = isExpNoReturn(varAndFuncEnv, typeEnv, procedureDec->exp);
    
    MySymbol_EndScope(varAndFuncEnv);

    if (isBodyNoReturn)
        return true;
    else
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCDEC_PROCEDURE__BODY_TYPE_NOT_NORETURN,
            MySymbol_GetName(procedureDec->name));
        return false;
    }
}

//////////////////////////////////////////////////

//  forwrads(mySemantic.c)
bool isFunctionReturnTypeMatchesOrNil(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec);

////////////

//  FORM:
//      function id (tyfields) : type-id = exp
//  DO:
//      1.check whether the return type matches type `type-id`(or Nil of Record).
//      2.add a function to the function environment.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  TODO:
//      there is a bug in this function, try to fix it.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Func_Function_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionDec functionDec)
{
    MySymbol_BeginScope(varAndFuncEnv);
    addFormalsToScope(varAndFuncEnv, typeEnv, functionDec->tyFieldList);

    bool isBodyLegal =
        isExpLegal(varAndFuncEnv, typeEnv, functionDec->exp);

    bool isReturnTypeMatches = false;
    if (isBodyLegal)
        isReturnTypeMatches = isFunctionReturnTypeMatchesOrNil(
            varAndFuncEnv, typeEnv, functionDec);

    MySymbol_EndScope(varAndFuncEnv);

    if (isReturnTypeMatches)
        return true;
    else
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCDEC_FUNCTION__RETURN_TYPE_NOT_MATCH,
            MySymbol_GetName(functionDec->name));
        return false;
    }
}

bool MySemantic_Dec_Func_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myFuncDec funcDec)
{
    if (funcDec == NULL)        return true;

    switch (funcDec->kind)
    {
        case ProcedureDec:
            return MySemantic_Dec_Func_Procedure_TwoPass(
                varAndFuncEnv, typeEnv, funcDec->u.procedureDec);
        case FunctionDec:
            return MySemantic_Dec_Func_Function_TwoPass(
                varAndFuncEnv, typeEnv, funcDec->u.functionDec);
        default:
            assert (false);
    }
}

/////////////////////////////////////////////////
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

//  a delegate function.
bool MySemantic_Dec_FuncOrType_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myDec dec)
{
    switch (dec->kind)
    {
        case TypeDec:
            return MySemantic_Dec_Type_TwoPass(
                typeEnv, dec->u.tyDec);
        case FuncDec:
            return MySemantic_Dec_Func_TwoPass(
                varAndFuncEnv, typeEnv, dec->u.funcDec);
        default:
            assert(false);
    }
}

bool MySemantic_Dec_Var_Pass(
    myTable varAndFuncEnv, myTable typeEnv, myDec varDec)
{
    return MySemantic_Dec_Var(
        varAndFuncEnv, typeEnv, varDec->u.varDec);
}

////////////////////////////////////////////////////////////////

//  forwards
bool PassTemplate(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs,
    bool (*passFunc)(myTable, myTable, myDec));

///////////////

bool MySemantic_Decs_FuncsOrTypes_OnePass(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    return PassTemplate(
        varAndFuncEnv, typeEnv, decs, MySemantic_Dec_FuncOrType_OnePass);
}

bool MySemantic_Decs_FuncsOrTypes_TwoPass(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    return PassTemplate(
        varAndFuncEnv, typeEnv, decs, MySemantic_Dec_FuncOrType_TwoPass);
}

bool MySemantic_Decs_Vars_Pass(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    return PassTemplate(
        varAndFuncEnv, typeEnv, decs, MySemantic_Dec_Var_Pass);
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

///////////////////////////////////////////////////////

//  forwards
myDecList copyDecs(myDecList decs);
bool detectIllegalRecursive_SameTypeOrFuncDecs(myDecList decs);
bool isAllDecsNamed(myDecList decs);
bool detectIllegalRecursive_NamedTypes(myDecList decs);
myDecList getNextConsecutivePart(myDecList decs, myDecList* pNext);

bool MySemantic_Decs_Recursive(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    if (decs == NULL)       return true;
    if (detectIllegalRecursive_SameTypeOrFuncDecs(decs))
        return false;

    //  every cycle Decs will be divide into some parts, so, we make a 
    //  copy here to make the origin one no-changed.
    myDecList next = copyDecs(decs);
    myDecList thisConsecutivePart;
    while ((thisConsecutivePart = getNextConsecutivePart(next, &next)))
    {
        if (thisConsecutivePart->dec->kind == VarDec)
        {
            if (!MySemantic_Decs_Vars_Pass(
                varAndFuncEnv, typeEnv, thisConsecutivePart))
                return false;
        }
        else
        {
            if (isAllDecsNamed(thisConsecutivePart) &&
                detectIllegalRecursive_NamedTypes(thisConsecutivePart))
                return false;

            if (!MySemantic_Decs_FuncsOrTypes_OnePass(
                    varAndFuncEnv, typeEnv, thisConsecutivePart) ||
                !MySemantic_Decs_FuncsOrTypes_TwoPass(
                    varAndFuncEnv, typeEnv, thisConsecutivePart))
                return false;
        }
    }
    return true;
}

myDecList copyDecs(myDecList decs)
{
    if (decs == NULL)   return NULL;

    myDecList newDecs = makeMyDecList(NULL, NULL);

    myDecList ptr = newDecs;
    while (decs)
    {
        ptr->dec = decs->dec;
        decs->next == NULL ? 
            (ptr->next = NULL) :
            (ptr->next = makeMyDecList(NULL, NULL), ptr = ptr->next);

        decs = decs->next;  
    }
    return newDecs;
}

bool isAllDecsNamed(myDecList decs)
{
    while (decs)
    {
        if (decs->dec->kind != TypeDec ||
            decs->dec->u.tyDec->type->kind != NamedType)
            return false;
        decs = decs->next;
    }
    return true;
}

myDecList getNextConsecutivePart(myDecList decs, myDecList* pNext)
{
    if (decs == NULL)   return NULL;

    myDecList begin = decs;
    while (decs->next &&
           decs->dec->kind == decs->next->dec->kind)
            decs = decs->next;

    if (decs->next == NULL)  *pNext = NULL;
    else                     *pNext = decs->next, decs->next = NULL;

    return begin;
}

//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////

//  forwards
bool detectConsecutivelySameFuncOrTypeDec(
    myDec dec, enum myDecKind* formerDecKind, mySymbol* formerDecName);
bool detectIllegalRecursive_SameTypeOrFuncDec(myDec dec);
void resetDetectState(void);

/////////

bool detectIllegalRecursive_SameTypeOrFuncDecs(myDecList decs)
{
    while (decs)
    {
        if (detectIllegalRecursive_SameTypeOrFuncDec(decs->dec))
        {
            resetDetectState();
            return true;
        }
        decs = decs->next;
    }
    resetDetectState();
    return false;
}


static enum myDecKind g_formerDecKind = None;
static mySymbol       g_formerDecName = NULL;

bool detectIllegalRecursive_SameTypeOrFuncDec(myDec dec)
{
    if (dec->kind == VarDec)
    {
        g_formerDecKind = None;
        g_formerDecName = NULL;
        return false;
    }

    return detectConsecutivelySameFuncOrTypeDec(
        dec, &g_formerDecKind, &g_formerDecName);
}

void resetDetectState(void)
{
    g_formerDecKind = None;
    g_formerDecName = NULL;
}

//////////////////////////////////////////////////////////////////////////

//  forwards
mySymbol getNameFromDec(myDec dec);
mySymbol getNameFromFuncDec(myFuncDec dec);

/////////

bool detectConsecutivelySameFuncOrTypeDec(
    myDec dec, enum myDecKind* formerDecKind, mySymbol* formerDecName)
{
    enum myDecKind thisDecKind = dec->kind;
    mySymbol       thisDecName = getNameFromDec(dec);

    if (*formerDecKind == thisDecKind &&
        MySymbol_IsSymbolEqual(*formerDecName, thisDecName))
    {
        MyError_pushErrorCodeWithMsg(ERROR__RECURSIVE__ILLEGAL,
            MySymbol_GetName(thisDecName));
        return true;
    }
    else
    {
        *formerDecKind = thisDecKind;
        *formerDecName = thisDecName;
        return false;
    }
}

mySymbol getNameFromFuncDec(myFuncDec dec)
{
    switch (dec->kind)
    {
        case FunctionDec:   return dec->u.functionDec->name;
        case ProcedureDec:  return dec->u.procedureDec->name;
        default:            assert (false);
    }
}

mySymbol getNameFromDec(myDec dec)
{
    switch (dec->kind)
    {
        case TypeDec:
            return dec->u.tyDec->name;
        case FuncDec:
            return getNameFromFuncDec(dec->u.funcDec);
        default:    assert (false);
    }
}


//////////////////////////////////////////////////////////////////////////

//  forwards
void addRecord(myTable recordTable, mySymbol typeName);
bool isSymbolIntOrString(mySymbol symbol);
bool findRecordAndSetError(myTable recordTable, mySymbol typeName);

bool detectIllegalRecursive_NamedTypes(myDecList decs)
{
    myTable recordTable = MySymbol_MakeNewTable();
    bool firstOne = true;

    while (decs)
    {
        myTypeDec typeDec = decs->dec->u.tyDec;
        if (findRecordAndSetError(recordTable, typeDec->type->u.typeName))
            return true;

        if (!isSymbolIntOrString(typeDec->type->u.typeName))
            addRecord(recordTable, typeDec->type->u.typeName);
        if (firstOne)
            addRecord(recordTable, typeDec->name),
            firstOne = false;

        decs = decs->next;
    }
    return false;
}

bool findRecordAndSetError(myTable recordTable, mySymbol typeName)
{
    if (MyEnvironment_getTypeFromName(recordTable, typeName))
    {
        MyError_pushErrorCodeWithMsg(ERROR__RECURSIVE__ILLEGAL,
            MySymbol_GetName(typeName));
        return true;
    }
    return false;
}

bool isSymbolIntOrString(mySymbol symbol)
{
    return MySymbol_IsSymbolEqual(symbol, MySymbol_MakeSymbol("int")) ||
            MySymbol_IsSymbolEqual(symbol, MySymbol_MakeSymbol("string"));
}

void addRecord(myTable recordTable, mySymbol typeName)
{
    void* fakeValue = (void*)12;
    MyEnvironment_addNewType(recordTable, typeName, fakeValue);
}

//////////////////////////////////////////////////////////////////////////