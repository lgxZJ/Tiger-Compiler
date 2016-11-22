#include "recursiveDecs.h"
#include "typeMaker.h"
#include "typeChecker.h"
#include "myEnvironment.h"
#include "mySemantic.h"
#include "myError.h"
#include "abstractSyntaxMaker.h"
#include "myEscape.h"

#include <stdlib.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////////
//  private function in mySemantic.c module

extern bool    MySemantic_Dec_Var      (myVarDec varDec, IR_myStatement* resultPtr);
extern myType  getActualTypeFromName   (mySymbol typeName);
extern myType  getExpActualResultType  (myExp exp);

extern bool isExpOneTypeOrIllegal(myExp exp, enum TypeKind kind);
    
extern bool isExpNoReturn  (myExp exp);
extern bool isTypeDefined  (mySymbol typeName);
extern bool isExpLegal     (myExp exp);

extern bool isExpLegalWithResult(myExp exp, IR_myExp* expResult);
extern bool isExpNoReturnWithResult(myExp exp, IR_myExp* expResultPtr);

extern void MySemantic_enterNewLevel(Trans_myLevel newLevel);
extern void MySemantic_leaveNewLevel(void);
extern Trans_myLevel MySemantic_getCurLevel(void);

/////////////////////////////////////////////////////////////////////

//  macros used for store results:
//      it allocates space for result and set result pointer to it.
//
//  because result pointer names are all the same, so these macros
//  have no parameters.
//
#define ALLOCATE_AND_SET_RESULT_PTR() *resultPtr = IR_makeSeq(NULL, NULL);  \
        IR_myStatement* nextPtr = &(*resultPtr)->u.seq.right;               \
        resultPtr = &(*resultPtr)->u.seq.left;                              \

#define MOVE_TO_NEXT_RESULT_PTR()       resultPtr = nextPtr;

/////////////////////////////////////////////////////////////////////

//  FORM:
//      type type-id1 = type-id2
//  DO:
//      1.add a type to the type environment without checking.
//  RETURN:
//      always return true.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Type_OnePass(myTypeDec typeDec)
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

    myTable typeEnv = MySemantic_getTypeEnvironment();
    MyEnvironment_addNewType(typeEnv, newTypeName, type);
    return true;
}

//  forwards
void updateType_Named(
    mySymbol newTypeName, mySymbol existedTypeName);
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
    mySymbol newTypeName, mySymbol existedTypeName)
{
    assert (newTypeName && existedTypeName);

    bool isNamedTypeDefined = isTypeDefined(existedTypeName);
    bool isRecursive = MySymbol_IsSymbolEqual(newTypeName, existedTypeName); 
    if (isNamedTypeDefined && !isRecursive)
    {
        updateType_Named(newTypeName, existedTypeName);
        return true;
    }
    else
    {
        processNamedDecErrors(isNamedTypeDefined, isRecursive, existedTypeName);
        return false;
    }
}

void updateType_Named(mySymbol newTypeName, mySymbol existedTypeName)
{
    myTable typeEnv = MySemantic_getTypeEnvironment();
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
    myTyFieldList tyFields);
void updateType_Record(
    mySymbol newTypeName, myTypeFieldList typeFields);

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
    mySymbol newTypeName, myTyFieldList fields)
{
    //  also checks whether field types defined
    myTypeFieldList typeFields =
        makeTypeFieldsFromTyFields_TwoPass(fields);

    if (typeFields != SEMANTIC_ERROR)            
    {
        updateType_Record(newTypeName, typeFields);
        return true;
    }
    else
        return false;
}

void updateType_Record(
    mySymbol newTypeName, myTypeFieldList typeFields)
{
    myTable typeEnv = MySemantic_getTypeEnvironment();
    myType newType = MyEnvironment_getTypeFromName(typeEnv, newTypeName);
    newType->u.typeRecord->fieldList = typeFields;
}

//  forwards
myTypeFieldList makeTypeFieldsFromTyFields_TwoPass(myTyFieldList tyFields)
{
    //  end condition for recursive
    if (tyFields == NULL)   return NULL; 

    myTyField oneTyField = tyFields->field;

    if (isTypeDefined(oneTyField->typeName))
    {
        myTable typeEnv = MySemantic_getTypeEnvironment();
        myTypeField oneTypeField = makeType_TypeField(
            oneTyField->varName,
            MyEnvironment_getTypeFromName(typeEnv, oneTyField->typeName));

        myTypeFieldList rests = makeTypeFieldsFromTyFields_TwoPass(
            tyFields->next);
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
    mySymbol arrayTypeName, mySymbol elementTypeName);
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
    mySymbol newTypeName, mySymbol elementTypeName)
{
    bool isElementTypeDefined = isTypeDefined(elementTypeName);
    bool isArrayRecursive = MySymbol_IsSymbolEqual(newTypeName, elementTypeName);

    if (isElementTypeDefined && !isArrayRecursive)
    {
        updateType_Array(newTypeName, elementTypeName);
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
    mySymbol arrayTypeName, mySymbol elementTypeName)
{
    myTable typeEnv = MySemantic_getTypeEnvironment();
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

bool MySemantic_Dec_Type_TwoPass( myTypeDec typeDec)
{
    mySymbol newTypeName = typeDec->name;
    switch (typeDec->type->kind)
    {
        case NamedType:
            return MySemantic_Dec_Type_Named_TwoPass(
                newTypeName, typeDec->type->u.typeName);
        case RecordType:
            return MySemantic_Dec_Type_Record_TwoPass(
                newTypeName, typeDec->type->u.tyFieldList);
        case ArrayType:
            return MySemantic_Dec_Type_Array_TwoPass(
                newTypeName, typeDec->type->u.arrayTypeName);
        default:
            assert(false);
    }
}

////////////////////////////////////////
////////////////////////////////////////

//  forwards
void setFormalsEscapeFlags(myTyFieldList formals);
void AddOneProcedure_OnePass(myProcedureDec procedureDec);
bool isFuncParamTypesDefined(myTyFieldList funcFields);

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
    myProcedureDec procedureDec)
{
    bool isParamTypesDefined =
        isFuncParamTypesDefined(procedureDec->tyFieldList);
        
    if (isParamTypesDefined)
    {
        setFormalsEscapeFlags(procedureDec->tyFieldList);
        AddOneProcedure_OnePass(procedureDec);
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

bool isFuncParamTypesDefined(myTyFieldList funcFields)
{
    while (funcFields)
    {
        myTyField paramField = funcFields->field;
        if (!isTypeDefined(paramField->typeName))
            return false;

        funcFields = funcFields->next;
    }

    return true;
}

void setFormalsEscapeFlags(myTyFieldList formals)
{
    //  formals are in frame by default.
    while (formals)
    {
        /*myType actualType = getActualTypeFromName(formals->field->typeName);
        if (isTypeString(actualType) || isTypeArray(actualType) ||
            isTypeRecord(actualType))
            formals->field->varEscape = true;*/
        formals->field->varEscape = true;
        formals = formals->next;
    }
}

//  forwards
void AddOneProcedure(myProcedureDec procedureDec);

/////////////

void AddOneProcedure_OnePass(myProcedureDec procedureDec)
{
    AddOneProcedure(procedureDec);
}

myTypeList getTypesFromTyFields(myTyFieldList funcFields)
{
    if (funcFields == NULL) return NULL;

    myType fieldType = getActualTypeFromName(
        funcFields->field->typeName);
    return makeType_TypeList(
        fieldType,
        getTypesFromTyFields(funcFields->next));
}

void AddOneFunc(
    mySymbol funcName, myTyFieldList tyFields, myType returnType)
{
    myTypeList formalTypes =
        getTypesFromTyFields(tyFields); 

    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    //  fill these empty fields in two-pass
    myVarAndFuncEntry funcEntry = myEnvironment_makeFuncEntry(
        (Trans_myLevel)NULL, Temp_newLabel(),
        formalTypes, returnType);
    MyEnvironment_addNewVarOrFunc(
        varAndFuncEnv, funcName, funcEntry);
}

void AddOneProcedure(myProcedureDec procedureDec)
{
    AddOneFunc(procedureDec->name,
        procedureDec->tyFieldList, makeType_NoReturn());
}

//////////////////////////////////////////////////

//  forwards
void AddOneFunction_OnePass(myFunctionDec functionDec);
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
    myFunctionDec functionDec)
{
    bool isParamTypesDefined =
        isFuncParamTypesDefined(functionDec->tyFieldList);
    bool isReturnTypeDefined = 
        isTypeDefined(functionDec->returnType);
    
    if (isParamTypesDefined && isReturnTypeDefined)
    {
        setFormalsEscapeFlags(functionDec->tyFieldList);
        AddOneFunction_OnePass(functionDec);
        return true;
    }
    else
    {
        processFunctionErrors_OnePass(
            isParamTypesDefined, isReturnTypeDefined, functionDec->name);
        return false;
    }
}

//  forwards
void AddOneFunction(myFunctionDec functionDec);

///////

void AddOneFunction_OnePass(myFunctionDec functionDec)
{
    AddOneFunction(functionDec);
}
void AddOneFunction(myFunctionDec functionDec)
{
    myTable typeEnv = MySemantic_getTypeEnvironment();
    myType returnType =
        MyEnvironment_getTypeFromName(typeEnv, functionDec->returnType);
    AddOneFunc(functionDec->name, functionDec->tyFieldList, returnType);
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
    myFuncDec funcDec)
{
    if (funcDec == NULL)        return true;

    switch (funcDec->kind)
    {
        case ProcedureDec:
            return MySemantic_Dec_Func_Procedure_OnePass(
                funcDec->u.procedureDec);
        case FunctionDec:
            return MySemantic_Dec_Func_Function_OnePass(
                funcDec->u.functionDec);
        default:
            assert (false);
    }
}

/////////////////////////////////////////////////

//  forwards
void addFormalsToScope(mySymbol funcName, myTyFieldList funcFormals);
myBoolList generateFormalBools(myTyFieldList formals);
Trans_myLevel updateFunc_Procedure(myProcedureDec procedureDec);

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
    myProcedureDec procedureDec)
{
    MySymbol_BeginScope(MySemantic_getVarAndFuncEnvironment());
    Trans_myLevel newLevel = updateFunc_Procedure(procedureDec);
    MySemantic_enterNewLevel(newLevel);

    addFormalsToScope(procedureDec->name, procedureDec->tyFieldList);

    bool isBodyNoReturn = false;
    IR_myExp bodyResult = NULL;
    isBodyNoReturn = isExpNoReturnWithResult(procedureDec->exp, &bodyResult);
    

    MySemantic_leaveNewLevel();
    MySymbol_EndScope(MySemantic_getVarAndFuncEnvironment());

    if (isBodyNoReturn)
    {
        Trans_proccedureBody(bodyResult, procedureDec->name);
        return true;
    }
    else
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCDEC_PROCEDURE__BODY_TYPE_NOT_NORETURN,
            MySymbol_GetName(procedureDec->name));
        return false;
    }
}

myBoolList generateFormalBools(myTyFieldList formals)
{
    if (formals == NULL)   return NULL;
    return Frame_makeBoolList(
        generateFormalBools(formals->next),
        formals->field->varEscape);
}

void addFormalsToScope(mySymbol funcName, myTyFieldList funcFormals)
{
    //  add formals to environments
    MySymbol_BeginScope(MySemantic_getVarAndFuncEnvironment());
    MySymbol_BeginScope(MySemantic_getTypeEnvironment());

    myVarAndFuncEntry funcEntry = MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(), funcName);

    //  skip return value and static link
    Trans_myAccessList accessList = Trans_getFormals(
        MyEnvironment_getFuncLevel(funcEntry));
    myTypeList accessTypes = MyEnvironment_getFuncFormalTypes(funcEntry);
    while (accessTypes && accessList && funcFormals)
    {
        MyEnvironment_addNewVarOrFunc(
            MySemantic_getVarAndFuncEnvironment(),
            funcFormals->field->varName,
            myEnvironment_makeVarEntry(accessList->head, accessTypes->head));

        accessTypes = accessTypes->tails;
        accessList = accessList->tail;
        funcFormals = funcFormals->next;
    }

    assert (accessTypes == NULL &&
            accessList == NULL &&
            funcFormals == NULL);
}

Trans_myLevel updateFunc_Procedure(myProcedureDec procedureDec)
{
    // formals of functions are all escaped
    myLabel funcLabel = Temp_newLabel();
    Trans_myLevel newLevel = Trans_newLevel(
        MySemantic_getCurrentLevel(),
        funcLabel,
        generateFormalBools(procedureDec->tyFieldList));
    MyEnvironment_updateFuncLevel(
        MySemantic_getVarAndFuncEnvironment(),
        procedureDec->name,
        newLevel);
    return newLevel;
}

//////////////////////////////////////////////////

//  forwards
bool isFunctionReturnTypeMatchesOrNil(
    myFunctionDec functionDec);
Trans_myLevel updateFunc_Function(
    myFunctionDec functionDec);

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
    myFunctionDec functionDec)
{
    Trans_myLevel newLevel = updateFunc_Function(functionDec);
    MySemantic_enterNewLevel(newLevel);
    MySymbol_BeginScope(MySemantic_getVarAndFuncEnvironment());
    addFormalsToScope(functionDec->name, functionDec->tyFieldList);

    IR_myExp bodyResult = NULL;
    bool isBodyLegal = isExpLegalWithResult(functionDec->exp, &bodyResult);

    bool isReturnTypeMatches = false;
    if (isBodyLegal)
        isReturnTypeMatches = isFunctionReturnTypeMatchesOrNil(
            functionDec);

    MySymbol_EndScope(MySemantic_getVarAndFuncEnvironment());
    MySemantic_leaveNewLevel();

    if (isReturnTypeMatches)
    {
        Trans_functionBody(bodyResult, functionDec->name);
        return true;
    }
    else
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCDEC_FUNCTION__RETURN_TYPE_NOT_MATCH,
            MySymbol_GetName(functionDec->name));
        return false;
    }
}

bool isFunctionReturnTypeMatchesOrNil(
    myFunctionDec functionDec)
{
    myType bodyType =
            getExpActualResultType(functionDec->exp);
    myType functionReturnType =
            getActualTypeFromName(functionDec->returnType);
    return isTypeEqual(bodyType, functionReturnType) ||
            (isTypeRecord(functionReturnType) && isTypeNil(bodyType));
}

Trans_myLevel updateFunc_Function(myFunctionDec functionDec)
{
    // formals of functions are all escaped
    myLabel funcLabel = Temp_newLabel();
    Trans_myLevel newLevel = Trans_newLevel(
        MySemantic_getCurrentLevel(),
        funcLabel,
        generateFormalBools(functionDec->tyFieldList));

    MyEnvironment_updateFuncLevel(
        MySemantic_getVarAndFuncEnvironment(),
        functionDec->name,
        newLevel);
    return newLevel;
}

/////////////////////////////////////

bool MySemantic_Dec_Func_TwoPass(
    myFuncDec funcDec)
{
    if (funcDec == NULL)        return true;

    switch (funcDec->kind)
    {
        case ProcedureDec:
            return MySemantic_Dec_Func_Procedure_TwoPass(
                funcDec->u.procedureDec);
        case FunctionDec:
            return MySemantic_Dec_Func_Function_TwoPass(
                funcDec->u.functionDec);
        default:
            assert (false);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

//  a delegate function.
bool MySemantic_Dec_FuncOrType_OnePass(myDec dec, IR_myStatement* dummy)
{
    //  the second parameter is explicitly omitted.
    switch (dec->kind)
    {
        case TypeDec:
            return MySemantic_Dec_Type_OnePass(dec->u.tyDec);
        case FuncDec:
            return MySemantic_Dec_Func_OnePass(dec->u.funcDec);
        default:
            assert(false);
    }
}

//  a delegate function.
bool MySemantic_Dec_FuncOrType_TwoPass(myDec dec, IR_myStatement* dummy)
{
    //  the second parameter is explicitly omitted.
    switch (dec->kind)
    {
        case TypeDec:
            return MySemantic_Dec_Type_TwoPass(dec->u.tyDec);
        case FuncDec:
            return MySemantic_Dec_Func_TwoPass(dec->u.funcDec);
        default:
            assert(false);
    }
}

bool MySemantic_Dec_Var_Pass(myDec varDec, IR_myStatement* resultPtr)
{
    return MySemantic_Dec_Var(varDec->u.varDec, resultPtr);
}

////////////////////////////////////////////////////////////////

//  forwards
bool PassTemplate(
    myDecList decs,
    bool (*passFunc)(myDec, IR_myStatement*),
    IR_myStatement* resultPtr);

///////////////

bool MySemantic_Decs_FuncsOrTypes_OnePass(myDecList decs)
{
    IR_myStatement dummy;
    return PassTemplate(decs, MySemantic_Dec_FuncOrType_OnePass, &dummy);
}

bool MySemantic_Decs_FuncsOrTypes_TwoPass(myDecList decs)
{
    IR_myStatement dummy;
    return PassTemplate(decs, MySemantic_Dec_FuncOrType_TwoPass, &dummy);
}

bool MySemantic_Decs_Vars_Pass(myDecList decs, IR_myStatement* resultPtr)
{
    return PassTemplate(decs, MySemantic_Dec_Var_Pass, resultPtr);
}

bool PassTemplate(
    myDecList decs, bool (*passFunc)(myDec, IR_myStatement*),
    IR_myStatement* resultPtr)
{
    while (decs)
    {
        ALLOCATE_AND_SET_RESULT_PTR();

        if (!(*passFunc)(decs->dec, resultPtr))
            return false;
        decs = decs->next;

        MOVE_TO_NEXT_RESULT_PTR();
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
    myDecList decs, IR_myStatement* resultPtr)
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
        ALLOCATE_AND_SET_RESULT_PTR();

        if (thisConsecutivePart->dec->kind == VarDec)
        {
            if (!MySemantic_Decs_Vars_Pass(thisConsecutivePart, resultPtr))
                return false;
        }
        else
        {
            if (isAllDecsNamed(thisConsecutivePart) &&
                detectIllegalRecursive_NamedTypes(thisConsecutivePart))
                return false;

            if (!MySemantic_Decs_FuncsOrTypes_OnePass(thisConsecutivePart) ||
                !MySemantic_Decs_FuncsOrTypes_TwoPass(thisConsecutivePart))
                return false;
        }

        MOVE_TO_NEXT_RESULT_PTR();
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