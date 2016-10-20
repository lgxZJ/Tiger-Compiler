#include "mySemantic.h"
#include "myEnvironment.h"
#include "myError.h"
#include "typeMaker.h"
#include "typeChecker.h"
#include "abstractSyntaxMaker.h"
#include "breakChecker.h"

#include "recursiveDecs.h"

#include <stdio.h>  //  for fprintf().
#include <assert.h> //  for assert().

//////////////////////////////////////////////////////////////////////
//
//  NOTE:   The expression nil(a reserved word) denotes a value nil
//  belonging to every type.
//
//////////////////////////////////////////////////////////////////////


//  forwards
myType getExpActualResultType(myTable varAndFuncEnv, myTable typeEnv, myExp exp);
static myType getActualType(myType type);
myType getActualTypeFromName(myTable typeEnv, mySymbol typeName);
myTranslationAndType MySemantic_LValueExp(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);

////////////////////////////////////////////////////////////////////////
//
//  Exp Checkers

//  DO:
//      analyze if this expression fits its semantic
bool isExpLegal(
    myTable varAndFuncEnv, myTable typeEnv, myExp exp)
{
    assert (varAndFuncEnv && typeEnv && exp);

    myTranslationAndType result = 
        MySemantic_Exp(varAndFuncEnv, typeEnv, exp);
        
    if (result == SEMANTIC_ERROR)   return false;
    else                            return true;
}

bool isExpOneTypeOrIllegal(
    myTable varAndFuncEnv, myTable typeEnv,
    myExp exp, enum TypeKind kind)
{
    assert (varAndFuncEnv && typeEnv && exp);

    myTranslationAndType result = 
        MySemantic_Exp(varAndFuncEnv, typeEnv, exp);
    if (result == SEMANTIC_ERROR)     return false;

    myType expActualType = getActualType(result->type);
    switch (kind)
    {
        case TypeInt:       return isTypeInt(expActualType);
        case TypeNoReturn:  return isTypeNoReturn(expActualType);
        case TypeArray:     return isTypeArray(expActualType);
        default:            assert(false);
    }
}

static bool isExpInt(myTable varAndFuncEnv, myTable typeEnv, myExp exp)
{
    return isExpOneTypeOrIllegal(varAndFuncEnv, typeEnv, exp, TypeInt);
}

bool isExpNoReturn(myTable varAndFuncEnv, myTable typeEnv, myExp exp)
{
    return isExpOneTypeOrIllegal(varAndFuncEnv, typeEnv, exp, TypeNoReturn);
}

//  NOTE:   array type named arrayTypeName must be valid. 
static bool isExpThisArrayElement(
    myTable varAndFuncEnv, myTable typeEnv,
    myExp exp, mySymbol arrayTypeName)
{
    assert (exp && arrayTypeName);

    myType arrayElementType = getActualType(
        getActualTypeFromName(typeEnv, arrayTypeName)
        ->u.typeArray->type);
    myType initialExpType = getActualType(
        MySemantic_Exp(varAndFuncEnv, typeEnv, exp)->type); 
    return isTypeEqual(arrayElementType, initialExpType);
}

bool isLValueExpLegal(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp)
{
    myTranslationAndType result =
        MySemantic_LValueExp(varAndFuncEnv, typeEnv, lValueExp);
    return result != SEMANTIC_ERROR;
}

//  NOTE:   expression must ba valid.
myType getExpActualResultType(myTable varAndFuncEnv, myTable typeEnv, myExp exp)
{
    assert (varAndFuncEnv && typeEnv && exp);

    myTranslationAndType result =
        MySemantic_Exp(varAndFuncEnv, typeEnv, exp);
    assert (result != SEMANTIC_ERROR);

    return getActualType(result->type);
}

myType getLValueExpActualResultType(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp  lValueExp)
{
    assert (varAndFuncEnv && typeEnv && lValueExp);

    myTranslationAndType result =
        MySemantic_LValueExp(varAndFuncEnv, typeEnv, lValueExp);
    assert (result != SEMANTIC_ERROR);

    return getActualType(result->type);
}

////////////////////////////////////////////////////////////////////////
//
//  myTranslationAndType Maker

//  RETURN:
//      A valid(not SEMANTIC_ERROR) variable of myTranslationAndType type.
myTranslationAndType make_MyTranslationAndType(
    myTranslation translation, myType type)
{
    myTranslationAndType result = 
        makeMemoryBlock(sizeof(*result), MEMORY_TYPE_NONE);
    assert(result);

    result->translation = translation;
    result->type = type;

    return result;
}

/////////////////////////////////////////////////////////////////////////
//
//  private functions

//  NOTE:
//      must be called after isVariableDeclared or isFuncDefined.
static myVarAndFuncEntry getVarOrFuncFromName(myTable varAndFuncEnv, mySymbol symbol)
{
    assert (varAndFuncEnv && symbol);
    return MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, symbol);
}

//  NOTE:
//      must be called after isVariableDeclared.
static myType getVariableType(myTable varAndFuncEnv, mySymbol variableName)
{
    assert (varAndFuncEnv && variableName);

    return MyEnvironment_getVarType(
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, variableName));
}

//  NOTE:
//      must be called after isFuncDefined.
static myType getFunctionReturnType(myTable varAndFuncEnv, mySymbol functionName)
{
    assert (varAndFuncEnv && functionName);

    return MyEnvironment_getFuncReturnType(
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, functionName));
}

//  NOTE:
//      must be called after isFuncDefined.
static myTypeList getFunctionFormalTypes(
    myTable varAndFuncEnv, mySymbol functionName)
{
    assert (varAndFuncEnv && functionName);

    return MyEnvironment_getFuncFormalTypes(
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, functionName));
}

///////////////////////////////////////////////////////////////////

static bool isVariableDeclared(myTable varAndFuncEnv, mySymbol variableName)
{
    assert (varAndFuncEnv && variableName);

    myVarAndFuncEntry entry = 
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, variableName);

    if (entry == NULL || !myEnvironment_isVarEntry(entry))
        return false;
    else
        return true;
}

static bool isFuncDefined(myTable varAndFuncEnv, mySymbol funcSymbol)
{
    assert (varAndFuncEnv && funcSymbol);

    myVarAndFuncEntry entry = 
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, funcSymbol);

    if (entry == NULL || !myEnvironment_isFuncEntry(entry))
        return false;
    else
        return true;
}

bool isTypeDefined(myTable typeEnv, mySymbol typeName)
{
    assert (typeEnv && typeName);

    return MyEnvironment_getTypeFromName(typeEnv, typeName) != NULL;
}

static myType getActualType(myType type)
{
    while(isTypeNamed(type))
    {
        type = type->u.typeNamed->type;
    }
    return type;
}

//  NOTE:   type must already be defined.
myType getActualTypeFromName(myTable typeEnv, mySymbol typeName)
{
    myType type = MyEnvironment_getTypeFromName(typeEnv, typeName);
    return getActualType(type);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//                           Semantic Globals                             //
////////////////////////////////////////////////////////////////////////////

myTable g_varAndFuncEnv = NULL;
myTable g_typeEnv = NULL;

////////////////////////////////////////////////////////////////////////////
//                    Semantic Getters and Setters                        //
////////////////////////////////////////////////////////////////////////////


void MySemantic_setVarAndFuncEnvironment(myTable varAndFuncEnv)
{
    g_varAndFuncEnv = varAndFuncEnv;
}

void MySemantic_setTypeEnvironment(myTable typeEnv)
{
    g_typeEnv = typeEnv;
}

myTable MySemantic_getVarAndFuncEnvironment(void)
{
    return g_varAndFuncEnv;
}

myTable MySemantic_getTypeEnvironment(void)
{
    return g_typeEnv;
}

////////////////////////////////////////////////////////////////////////////
//                           Semantic Checkers                            //
////////////////////////////////////////////////////////////////////////////


myTranslationAndType MySemantic_LValueExp_SimpleVar(
    myTable varAndFuncEnv, myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_RecordField(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_ArraySubscript(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);

//  FORM:
//    ->  id
//    ->  lvalue.id
//    ->  lvalue[exp]
/**
 *  @brief      Do semantic analysis on LValueExp.
 *
 *  @param[in]  varAndFuncEnv   A valid varaible and function symbol table of
 *              tiger source files.
 *  @param[in]  lValueExp       A valid left-value-expression to check.
 *
 *  @return     If it succeed, it returns a myTranslationAndType containing
 *                  intermediate codes and type of the given expression;
 *              otherwise, it returns SEMANTIC_ERROR; 
 *  @remark     The semantic analysis work of this function is to do semantic
 *              analysis on SimpleVar, RecordField and ArraySubscript.
 *
 *  @see        MySemantic_LValueExp_ArraySubscript,
 *              MySemantic_LValueExp_RecordField,
 *              MySemantic_LValueExp_SimpleVar.
 */
myTranslationAndType MySemantic_LValueExp(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp)
{
    switch (lValueExp->kind)
    {
        case SimpleVar:
            return MySemantic_LValueExp_SimpleVar(varAndFuncEnv, lValueExp);
            break;
        case RecordField:
            return MySemantic_LValueExp_RecordField(varAndFuncEnv, typeEnv, lValueExp);
            break;
        case ArraySubscript:
            return MySemantic_LValueExp_ArraySubscript(varAndFuncEnv, typeEnv, lValueExp);
            break;
    }
}

//  FORM:
//      id
/**
 *  @brief  Do semantic analysis on SimpleVar, one kind of LValueExp.
 *
 *          DO:
 *              1.check whether the variable is already declared.
 *              2.return the variable's type.
 *
 *  @param[in]  varAndFuncEnv   A valid varaible and function symbol table of
 *              tiger source files.
 *  @param[in]  lValueExp       A valid LValueExp consists of SimpleVar
 *              to check.
 *
 *  @return     If it succeed, it returns a `myTranslationAndType` containing
 *              intermediate codes and type of the given expression; Otherwise,
 *              it returns SEMANTIC_ERROR;
 *
 *  @note       Tested.
 */
myTranslationAndType MySemantic_LValueExp_SimpleVar(
    myTable varAndFuncEnv, myLValueExp lValueExp)
{
    if (!isVariableDeclared(varAndFuncEnv, lValueExp->id))
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__LVALUE_SIMPLE_VAR__VAR_NOT_DECLARED,
            MySymbol_GetName(lValueExp->id));
        return SEMANTIC_ERROR;
    }
    
    return make_MyTranslationAndType(NULL, 
        getVariableType(varAndFuncEnv, lValueExp->id));
}

///////////////////////////////////////////////

//  forward declarations
bool isVariableRecordType(myTable typeEnv, mySymbol variableName);
myType typeContainsLValueAux(
    myTable varAndFuncEnv, myTable typeEnv, myType type, myLValueAux aux);
static myType arrayContainsLValueAux(
    myTable varAndFuncEnv, myTable typeEnv, myType type, myLValueAux aux);
static myType analyzeRecordFields(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);
static myType recordContainsLValueAux(
    myTable varAndFuncEnv, myTable typeEnv, myType type, myLValueAux aux);
void processRecordFieldErrors(
    bool isVarDeclared, bool isVarAnRecord, mySymbol recordVarName);

//  FORM:
//      lvalue.id
/**
 *  @brief  Do semantic analysis on RecordField, one kind of LValueExp.
 *
 *              1.check whether the variable is already declared.
 *              2.check whether the variable is of record type.
 *              3.check whether the fields can be found in record type.
 *              4.return the last field's type.
 *
 *  @param[in]  varAndFuncEnv   A valid varaible and function symbol table of
 *              tiger source files.
 *  @param[in]  lValueExp       A valid LValueExp consists of RecordField
 *              to check.
 *
 *  @return     if succeed, it returns a myTranslationAndType containing
 *                  intermediate codes and result type of the given expression;
 *              if failed, it returns SEMANTIC_ERROR.
 *
 *  @note       Tested.
 */
myTranslationAndType MySemantic_LValueExp_RecordField(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp)
{
    mySymbol recordVarName = lValueExp->id;

    bool isVarDeclared = isVariableDeclared(varAndFuncEnv, recordVarName);
    bool isVarAnRecord = false;
    
    if (isVarDeclared)
        isVarAnRecord = isVariableRecordType(varAndFuncEnv, recordVarName);
    if (isVarDeclared && isVarAnRecord)
    {
        myType returnType =
            analyzeRecordFields(varAndFuncEnv, typeEnv, lValueExp);
        //  analysis error!
        if (returnType == SEMANTIC_ERROR)
            return SEMANTIC_ERROR;
        return make_MyTranslationAndType(NULL, returnType);
    }
    else
    {
        processRecordFieldErrors(isVarDeclared, isVarAnRecord, recordVarName);
        return SEMANTIC_ERROR;
    }
}


bool isVariableRecordType(myTable varAndFuncEnv, mySymbol variableName)
{
     myVarAndFuncEntry entry = getVarOrFuncFromName(varAndFuncEnv, variableName);
     myType actualVarType = getActualType(MyEnvironment_getVarType(entry));
     return isTypeRecord(MyEnvironment_getVarType(entry));
}

void processRecordFieldErrors(
    bool isVarDeclared, bool isVarAnRecord, mySymbol recordVarName)
{
    if (!isVarDeclared)
        MyError_pushErrorCodeWithMsg(
            ERROR__LVALUE_RECORD_FIELD__VAR_NOT_DECLARED,
            MySymbol_GetName(recordVarName));
    if (!isVarAnRecord)
        MyError_pushErrorCodeWithMsg(
            ERROR__LVALUE_RECORD_FIELD__VAR_NOT_RECORD,
            MySymbol_GetName(recordVarName));
}

//  DO:
//      check if field names in `lValueExp` can be found in record type.
//  RETURN:
//      return NULL if analyze fails.
//      return type of the last field if succeed.
static myType analyzeRecordFields(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp)
{
    assert (lValueExp);

    myRecordFieldAux fieldAux = lValueExp->u.recordFieldAux;
    myType typeOfRecord = makeType_Record(
        getVariableType(varAndFuncEnv, lValueExp->id)
            ->u.typeRecord->fieldList); 
    myLValueAux aux = makeMyLValueAux(
        fieldAux->id, NULL, fieldAux->next);

    return typeContainsLValueAux(varAndFuncEnv, typeEnv, typeOfRecord, aux);
}

/**
 *  @brief  Check if the field inside `aux` can be found in `type`.
 *  
 *  @param[in]  varAndFuncEnv   A variable and function symbol table of tiger source files.
 *  @param[in]  typeEnv         A type symbol table of tiger source files.
 *  @param[in]  type            The type where to check.
 *  @param[in]  aux             A structure which contains the field to check.
 *
 *  @return     if analyze succeed, this function returns the type of the last field;
 *              Otherwise, it returns SEMANTIC_ERROR;
 *  @remark     The caller make sures `type` and `aux` are all valid. And type of
 *              `type` must be RecordField or ArraySubscript. Or it will call exit()
 *              immediately.
 *  
 *  @note       Tested.
 */
myType typeContainsLValueAux(
    myTable varAndFuncEnv, myTable typeEnv, myType type, myLValueAux aux)
{ 
    switch (type->kind)
    {
        case TypeArray:
            return arrayContainsLValueAux(varAndFuncEnv, typeEnv, type, aux);
        case TypeRecord:
            return recordContainsLValueAux(varAndFuncEnv, typeEnv, type, aux);
        default:    //  never goes here
            assert(false);
    }
}

//  check whether field name in `aux` can be found inside record type.
//  return NULL for checking error.
static myType recordContainsLValueAux(
    myTable varAndFuncEnv, myTable typeEnv, myType type, myLValueAux aux)
{
    assert (isTypeRecord(type));
    assert (getLValueKind(aux) == RecordField);

    myTypeFieldList fields = type->u.typeRecord->fieldList;
    myType typeReturn = NULL;

    while (fields)  //  try to find field
    {
        if (MySymbol_IsSymbolEqual(fields->head->name, aux->id))
        {
            typeReturn = fields->head->type;
            break;
        }
        fields = fields->tails;
    }

    if (typeReturn == NULL)   //  field not found
    {
        MyError_pushErrorCodeWithMsg(ERROR_RECORD_FIELD_NOT_FOUND,
            MySymbol_GetName(aux->id));
        return SEMANTIC_ERROR;
     }
    else    //  field found
    {
        if (aux->next)  //  have next symbol or exp, do check
            return typeContainsLValueAux(varAndFuncEnv, typeEnv, 
                typeReturn, aux->next);
        else
            return typeReturn;
    }
}

//  check whether the type of array subscript is Integer.
//  return NULL for checking error.
static myType arrayContainsLValueAux(
    myTable varAndFuncEnv, myTable typeEnv, myType type, myLValueAux aux)
{
    assert (isTypeArray(type));
    assert (getLValueKind(aux) == ArraySubscript);

    //  check subscript's expression is of Integer type
    myExp subscriptExp = aux->exp;
    if (!isExpInt(varAndFuncEnv, typeEnv, subscriptExp))
    {
        MyError_pushErrorCode(ERROR_SUBSCRIPT_NOT_INT);
        return NULL;
    }

    if (aux->next)  //  have next symbol or exp, do checking
        return typeContainsLValueAux(varAndFuncEnv, typeEnv, 
                type->u.typeArray->type, aux->next);
    else
        return type->u.typeArray->type;
}

////////////////////////////////////////////////

//  forward declarations
bool isVariableArrayType(myTable varAndFuncEnv, mySymbol variableName);
myTranslationAndType analyzeRecursiveArraySubscripts(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp);
void processArraySubscriptErrors(
    bool isArrayVariableDeclared, bool isVariableArrayType,
    bool isSubscriptExpInt, myLValueExp lValueExp);

//  FORM:
//      lvalue[exp]
/**
 *  @brief  Do semantic analysis on ArraySubscript, one kind of LValueExp.
 *
 *          1.check whether the array variable is declared.
 *          2.check whether the declared variable is an array.
 *          3.check whether the subscript expression is of Integer type.
 *          4.return the last array element's type.
 *
 *  @param[in]  varAndFuncEnv   A valid varaible and function symbol table of
 *              tiger source files.
 *  @param[in]  lValueExp       A valid LValueExp consists of array-subscript
 *              to check.
 *
 *  @return     If succeed, it returns a myTranslationAndType containing intermediate
 *              codes and type of the given expression; otherwise, it returns
 *              SEMANTIC_ERROR.
 *
 *  @note       Tested.
 */
myTranslationAndType MySemantic_LValueExp_ArraySubscript(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp)
{
    mySymbol arrayVariableName = lValueExp->id;

    bool isArrayVariableDeclared =
        isVariableDeclared(varAndFuncEnv, arrayVariableName);
    bool isVariableAnArray = false;
    if (isArrayVariableDeclared) 
        isVariableAnArray = isVariableArrayType(varAndFuncEnv, arrayVariableName);

    myExp subscriptExp = lValueExp->u.arraySubscriptAux->exp;
    bool isSubscriptInt = isExpInt(varAndFuncEnv, typeEnv, subscriptExp);

    myTranslationAndType result;
    if (isArrayVariableDeclared && isVariableAnArray && isSubscriptInt)
        return analyzeRecursiveArraySubscripts(varAndFuncEnv, typeEnv, lValueExp);
    else    
    {
        processArraySubscriptErrors(isArrayVariableDeclared,
            isVariableAnArray, isSubscriptInt, lValueExp);
        return SEMANTIC_ERROR;
    }
}

bool isVariableArrayType(myTable varAndFuncEnv, mySymbol variableName)
{
    assert (varAndFuncEnv && variableName);

    myVarAndFuncEntry entry = getVarOrFuncFromName(varAndFuncEnv, variableName);
    if (myEnvironment_isVarEntry(entry))
        return isTypeArray(MyEnvironment_getVarType(entry));
    
    return false;
}

void processArraySubscriptErrors(
    bool isArrayVariableDeclared, bool isVariableArrayType,
    bool isSubscriptInt, myLValueExp lValueExp)
{
    assert (lValueExp);

    mySymbol arrayVariableName = lValueExp->id;

    if (!isArrayVariableDeclared)
        MyError_pushErrorCodeWithMsg(
            ERROR__LVALUE_ARRAY_SUBSCRIPT__VAR_NOT_DECLARED,
            MySymbol_GetName(arrayVariableName));
    if (!isVariableArrayType)
        MyError_pushErrorCodeWithMsg(
            ERROR__LVALUE_ARRAY_SUBSCRIPT__VAR_NOT_ARRAY_TYPE,
            MySymbol_GetName(arrayVariableName));
    if (!isSubscriptInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__LVALUE_ARRAY_SUBSCRIPT__SUBSCRIPT_NOT_INT,
            MySymbol_GetName(arrayVariableName));
}

//  DO:
//      check if the subscript expression inside `lValueExp` is of Integer type.
//  RETURN:
//      SEMANTIC_ERROR for checking error;
//      A myTranslationAndType variable for success.
myTranslationAndType analyzeRecursiveArraySubscripts(
    myTable varAndFuncEnv, myTable typeEnv, myLValueExp lValueExp)
{
    assert (lValueExp);

    myTranslationAndType result =
        make_MyTranslationAndType(NULL, NULL);
    
    //  caller make sures its a variable entry!
    myVarAndFuncEntry arrayEntry = getVarOrFuncFromName(varAndFuncEnv, lValueExp->id);
    assert (arrayEntry != NULL);

    if (lValueExp->u.arraySubscriptAux->next)
    {
        myType elementType = MyEnvironment_getVarType(arrayEntry)
            ->u.typeArray->type;

        //  recursive checking
        myType typeReturn = typeContainsLValueAux(
            varAndFuncEnv, typeEnv, elementType,
            lValueExp->u.arraySubscriptAux->next);

        if (typeReturn == NULL)
        {
            MyError_pushErrorCode(ERROR_SUBSCRIPT_ERROR);
            return SEMANTIC_ERROR;
        }
        else                        result->type = typeReturn;
    }
    else
    {
        //  return this array type
        result->type = MyEnvironment_getVarType(arrayEntry)
            ->u.typeArray->type;
    }
    return result;
}




////////////////////////////////////////////////////////////////////////////////

//  forward declarations
myTranslationAndType analyzeParamsOfFunction(
    myTable varAndFuncEnv, myTable typeEnv,
    myExpList formalVariables, myTypeList formalTypes,
    mySymbol functioName);
myTranslationAndType MySemantic_FunctionCallExp_Param(
    myTable varAndFuncEnv, myTable typeEnv,
    myParamFunctionCallExp paramFunctionCallExp);
myTranslationAndType MySemantic_FunctionCallExp_NoParam(
    myTable varAndFuncEnv, myTable typeEnv,
    myNoParamFunctionCallExp noParamFunctionCallExp);

/**
 *  @brief  Do semantic analysis on FunctionCallExp.
 *
 *  @remark The semantic analysis work of this function is to check:
 *              1.whether the function is already declared.
 *              2.if any, whether the formal parameters' type matches its signature.
 */
myTranslationAndType MySemantic_FunctionCallExp(
    myTable varAndFuncEnv, myTable typeEnv, myFunctionCallExp functionCallExp)
{
    switch (functionCallExp->kind)
    {
        case NoParamFunctionCallExp:
            return MySemantic_FunctionCallExp_NoParam(
                varAndFuncEnv, typeEnv, functionCallExp->u.noParamFunctionCallExp);
        case ParamFunctionCallExp:
            return MySemantic_FunctionCallExp_Param(
                varAndFuncEnv, typeEnv, functionCallExp->u.paramFunctionCallExp);
    }

    assert (false);
}

//  FORM:
//      id()
//  DO:
//      1.check whether the function has been defined.
//      2.return this function's return type.
//  RETURN:
//      If succeed, return a valid myTranslationAndType variable containing
//      function return type; otherwise, return SEMANTIC_ERROR;
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_FunctionCallExp_NoParam(
    myTable varAndFuncEnv, myTable typeEnv,
    myNoParamFunctionCallExp noParamFunctionCallExp)
{
    mySymbol functionName = noParamFunctionCallExp->name;
    if (!isFuncDefined(varAndFuncEnv, functionName))
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCTION_CALL__FUNC_NOT_DEFINED,
            MySymbol_GetName(functionName));
        return SEMANTIC_ERROR;
    }

    return make_MyTranslationAndType(
        NULL, getFunctionReturnType(varAndFuncEnv, functionName));
}


//  FORM:
//      id(exp[,exp])
//  DO:
//      1.check whether the function has been declared.
//      2.check whether paramter types matches(or nil of Record).
//      3.return this function's return type.
//  RETURN:
//      If succeed, return a valid myTranslationAndType variable containing
//      function return type; otherwise, return SEMANTIC_ERROR;
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_FunctionCallExp_Param(
    myTable varAndFuncEnv, myTable typeEnv,
    myParamFunctionCallExp paramFunctionCallExp)
{
    mySymbol functionName = paramFunctionCallExp->name;
    if (!isFuncDefined(varAndFuncEnv, functionName))
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCTION_CALL__FUNC_NOT_DEFINED,
            MySymbol_GetName(functionName));
        return SEMANTIC_ERROR;
    }

    myExpList formalVariables = paramFunctionCallExp->expList;
    myTypeList formalTypes = getFunctionFormalTypes(varAndFuncEnv, functionName);

    return analyzeParamsOfFunction(
        varAndFuncEnv, typeEnv, 
        formalVariables, formalTypes, 
        functionName);
}

//  forwards
bool isFormalTypeMatchesRealTypeOrNil(
    myType formalType, myType valueType);

myTranslationAndType analyzeParamsOfFunction(
    myTable varAndFuncEnv, myTable typeEnv,
    myExpList formalVariables, myTypeList formalTypes,
    mySymbol functioName)
{
    assert (varAndFuncEnv && typeEnv && functioName);

    while (formalVariables && formalTypes)
    {
        myTranslationAndType realParamResult = 
            MySemantic_Exp(varAndFuncEnv, typeEnv, formalVariables->exp);
        if (realParamResult == SEMANTIC_ERROR)   return SEMANTIC_ERROR;

        //  check if types matches
        if (!isFormalTypeMatchesRealTypeOrNil(
                formalTypes->head, realParamResult->type))
        {
            break;
        }

        formalVariables = formalVariables->next;
        formalTypes = formalTypes->tails;
    }

    //  parameter number not match!
    if (formalVariables || formalTypes)
    {
        MyError_pushErrorCodeWithMsg(
                ERROR__FUNCTION_CALL__PARAM_TYPES_NOT_MATCH,
                MySymbol_GetName(functioName));
        return SEMANTIC_ERROR;
    }
    else
    {
        return make_MyTranslationAndType(
            NULL, getFunctionReturnType(varAndFuncEnv, functioName));
    }
}

bool isNilValueOfRecordType(myType formalType, myType valueType)
{
    return isTypeRecord(formalType) && isTypeNil(valueType);
}

bool isFormalTypeMatchesRealTypeOrNil(myType formalType, myType realType)
{
    return isTypeEqual(formalType, realType) ||
           isNilValueOfRecordType(formalType, realType);
}

/////////////////////////////////////////////////////////////////////////////////

//  FORM:
//      nil
/**
 *  @brief  Semantic analysis of NilExp.
 *
 *          1.return myTypeNil Type.
 *
 *  @return A \b valid(not SEMANTIC_ERROR) myTranslationAndType variable.
 *  @remark Since NilExp consists nothing, there is no more semantic
 *          checking inside this method and the return type is myTypeNil.
 *
 *  @note   Tested
 */
myTranslationAndType MySemantic_NilExp(
    myTable varAndFuncEnv, myTable typeEnv, myNilExp nilExp)
{
    //  no checking needed here
    return make_MyTranslationAndType(NULL, makeType_Nil());  
}

////////////////////////////////////////////////////////////////////////////////////

//  FORM:
//      integer literals
/**
 *  @brief  Semantic analysis of IntegerLiteralExp.
 *
 *          1.return myTypeInt type.
 *
 *  @return A \b valid(not SEMANTIC_ERROR) myTranslationAndType variable.
 *  @remark Since IntegerLiteralExp only consists of integer literal,
 *          there is no more semantic checking inside this method and
 *          the return type is myTypeInt.
 *
 *  @note   Tested
 */
myTranslationAndType MySemantic_IntegerLiteralExp(
    myTable varAndFuncEnv, myTable typeEnv, myIntegerLiteralExp integerLiteralExp)
{
    //  no checking needed here
    return make_MyTranslationAndType(NULL, makeType_Int());
}

///////////////////////////////////////////////////////////////////////////////////

//  FORM:
//      string literals
/**
 *  @brief  Semantic analysis of StringLiteralExp.
 *
 *          1.return myTypeString type.
 *
 *  @return A \b valid(not SEMANTIC_ERROR) myTranslationAndType variable.
 *  @remark Since StringLiteralExp only consists of string literal,
 *          there is no more semantic checking inside this method
 *          and the return type is myTypeString.
 *
 *  @note   Tested
 */
myTranslationAndType MySemantic_StringLiteralExp(
    myTable varAndFuncEnv, myTable typeEnv, myStringLiteralExp stringLiteralExp)
{
    //  no checking needed here
    return make_MyTranslationAndType(NULL, makeType_String());  
}

/////////////////////////////////////////////////////////////////////////////////

//  forward declarations
bool isTypeDefinedAsArray(
    myTable typeEnv, mySymbol arrayTypeName);
bool isValueTypeMatchesArrayElementOrNil(
    myTable varAndFuncEnv, myTable typeEnv,
    myExp initialValue, mySymbol arrayTypeName);
void processArrayCreationErrors(
    bool isArrayTypeDefiend, bool isSubscriptAnIntExp,
    bool isInitialExpThisArrayType, mySymbol arrayTypeName);

//  FORM:
//      array-type-id [subscript-exp] of initial-exp
//  DO:
//      1.array type must already be declared. 
//      2.subscript expression must be of Integer type( and have valid range).
//      3.value expression must be of the array elements' type(or nil of Record).
//      4.return this array type.
//  RETURN:
//      return SEMANTIC_ERROR if it fails;
//      return a myTranslationAndType variable containing this array type.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_ArrayCreationExp(
    myTable varAndFuncEnv, myTable typeEnv, myArrayCreationExp arrayCreationExp)
{
    mySymbol arrayTypeName = arrayCreationExp->typeName;

    bool isArrayTypeDefiend = isTypeDefinedAsArray(typeEnv, arrayTypeName);
    bool isSubscriptAnIntExp = isExpInt(varAndFuncEnv, typeEnv, arrayCreationExp->length);
    bool isInitialExpThisArrayElementType = false;

    if (isArrayTypeDefiend)
        isInitialExpThisArrayElementType = isValueTypeMatchesArrayElementOrNil(
            varAndFuncEnv, typeEnv, arrayCreationExp->initial, arrayTypeName);

    if (isArrayTypeDefiend && isSubscriptAnIntExp && isInitialExpThisArrayElementType)
        return make_MyTranslationAndType(NULL, getActualTypeFromName(typeEnv, arrayTypeName));
    else
    {
        processArrayCreationErrors(isArrayTypeDefiend, isSubscriptAnIntExp,
            isInitialExpThisArrayElementType, arrayTypeName);
        return SEMANTIC_ERROR;
    }
}

bool isValueTypeMatchesArrayElementOrNil(
    myTable varAndFuncEnv, myTable typeEnv,
    myExp initialValue, mySymbol arrayTypeName)
{
    myType arrayElementType = getActualType(
        getActualTypeFromName(typeEnv, arrayTypeName)
            ->u.typeArray->type);
    myType valueType = getActualType(
        getExpActualResultType(varAndFuncEnv, typeEnv, initialValue));

    return isExpThisArrayElement(
                varAndFuncEnv, typeEnv, initialValue, arrayTypeName) ||
           isNilValueOfRecordType(arrayElementType, valueType);
}

bool isTypeDefinedAsArray(myTable typeEnv, mySymbol arrayTypeName)
{
    if (isTypeDefined(typeEnv, arrayTypeName))
        return isTypeArray(getActualTypeFromName(typeEnv, arrayTypeName));
    return false;
}

void processArrayCreationErrors(
    bool isArrayTypeDefiend, bool isSubscriptAnIntExp,
    bool isInitialExpThisArrayElementType, mySymbol arrayTypeName)
{
    myString arrayTypeString = MySymbol_GetName(arrayTypeName);
    if (!isArrayTypeDefiend)
        MyError_pushErrorCodeWithMsg(
            ERROR__ARRAY_CREATION__TYPE_NOT_ARRAY, arrayTypeString);
    if (!isSubscriptAnIntExp)
        MyError_pushErrorCodeWithMsg(
            ERROR__ARRAY_CREATION__SUBSCRIPT_NOT_INT, arrayTypeString);
    if (!isInitialExpThisArrayElementType)
        MyError_pushErrorCodeWithMsg(
            ERROR__ARRAY_CREATION__INITIAL_TYPE_NOT_ELEMENT, arrayTypeString);
}




/////////////////////////////////////////////////////////////////////////////////

//  forward declarations

bool isTypeDefinedAsRecord(myTable typeEnv, mySymbol recordTypeName);
myTranslationAndType MySemantic_RecordCreationExp_NoField(
    myTable varAndFuncEnv, myTable typeEnv,
    myNoFieldRecordCreationExp noFieldRecordCreationExp);
myTranslationAndType MySemantic_RecordCreationExp_Field(
    myTable varAndFuncEnv, myTable typeEnv,
    myFieldRecordCreationExp fieldRecordCreationExp);

//  FORM:
//      record-type-id {id=exp{,id=exp}} or
//      record-type-id {}
//  DO:
//      1.check whether the named-type is already declared as a record.
//      2.if any, check whether the field values matches the types(or nil of Record).
//      3.return this record type.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing
//          this record type;
//      if fails, it returns SEMANTIC_ERROR.
myTranslationAndType MySemantic_RecordCreationExp(
    myTable varAndFuncEnv, myTable typeEnv, myRecordCreationExp recordCreationExp)
{
    switch (recordCreationExp->kind)
    {
        case NoFieldRecordCreationExp:
            return MySemantic_RecordCreationExp_NoField(
                varAndFuncEnv, typeEnv, recordCreationExp->u.noFieldRecordCreationExp);
        case FieldRecordCreationExp:
            return MySemantic_RecordCreationExp_Field(
                varAndFuncEnv, typeEnv, recordCreationExp->u.fieldRecordCreationExp);
    }

    assert(false);
}

//  FORM:
//      record-type-id {}
//  DO:
//      1.check whether the named-type is already declared as a record.
//      2.return this record type.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing
//          this record type;
//      if fails, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.      
myTranslationAndType MySemantic_RecordCreationExp_NoField(
    myTable varAndFuncEnv, myTable typeEnv,
    myNoFieldRecordCreationExp noFieldRecordCreationExp)
{
    mySymbol recordTypeName = noFieldRecordCreationExp->typeName;

    if (isTypeDefinedAsRecord(typeEnv, recordTypeName))
        return make_MyTranslationAndType(NULL,
            getActualTypeFromName(typeEnv, recordTypeName));
    else
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__RECORD_CREATION__TYPE_NOT_DEFINED_AS_RECORD,
            MySymbol_GetName(recordTypeName));
        return SEMANTIC_ERROR;
    }
}


//  forward declarations
bool ifFieldNamesAndTypesMatches(
    myTable varAndFuncEnv, myTable typeEnv,
    myFieldRecordCreationExp fieldRecordCreationExp);
void processFieldRecordCreationErrors(
    bool isRecordTypeDefined,
    bool isCreationFieldsMatchThisRecordType,
    mySymbol recordTypeName);

//  FORM:
//      record-type-id {id=exp{,id=exp}}
//  DO:
//      1.check whether the named-type is already declared as a record.
//      2.if any, check whether the field values matches the types(or nil of Record).
//      3.return this record type.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing
//          this record type;
//      if fails, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_RecordCreationExp_Field(
    myTable varAndFuncEnv, myTable typeEnv,
    myFieldRecordCreationExp fieldRecordCreationExp)
{
    mySymbol recordTypeName = fieldRecordCreationExp->typeName;

    bool isRecordTypeDefined =
        isTypeDefinedAsRecord(typeEnv, recordTypeName);
    bool isCreationFieldsMatchThisRecordType = isRecordTypeDefined ?
            ifFieldNamesAndTypesMatches(
                varAndFuncEnv, typeEnv, fieldRecordCreationExp) :
            false;
    
    if (isRecordTypeDefined && isCreationFieldsMatchThisRecordType)
        return make_MyTranslationAndType(NULL,
            getActualTypeFromName(typeEnv, recordTypeName));
    else
    {
        processFieldRecordCreationErrors(isRecordTypeDefined,
            isCreationFieldsMatchThisRecordType, recordTypeName);
        return SEMANTIC_ERROR;
    }
}

void processFieldRecordCreationErrors(
    bool isRecordTypeDefined,
    bool isCreationFieldsMatchThisRecordType,
    mySymbol recordTypeName)
{
    myString recordTypeString = MySymbol_GetName(recordTypeName);
    if (!isRecordTypeDefined)
        MyError_pushErrorCodeWithMsg(
            ERROR__RECORD_CREATION__RECORD_TYPE_NOT_DEFINED, recordTypeString);
    if (!isCreationFieldsMatchThisRecordType)
        MyError_pushErrorCodeWithMsg(
            ERROR__RECORD_CREATION__FIELDS_NOT_MATCH, recordTypeString);
}

bool isTypeDefinedAsRecord(myTable typeEnv, mySymbol recordTypeName)
{
    if (isTypeDefined(typeEnv, recordTypeName))
        return isTypeRecord(getActualTypeFromName(typeEnv, recordTypeName));
    return false;
}

//  forwards
bool isTypeAndNameMatchesOrNil(
    myType fieldType, myType valueType,
    mySymbol givenName, mySymbol fieldName);
bool isOneFieldMatches(
    myTable varAndFuncEnv, myTable typeEnv,
    myTypeFieldList typeFields, myRecordFieldList givenRecordFields);

//  NOTE:   must be called after isTypeNotRecordAndSetError.
bool ifFieldNamesAndTypesMatches(
    myTable varAndFuncEnv, myTable typeEnv,
    myFieldRecordCreationExp fieldRecordCreationExp)
{
    mySymbol recordTypeName = fieldRecordCreationExp->typeName;
    myRecordFieldList givenRecordFields = fieldRecordCreationExp->fieldList;
    myTypeFieldList typeFields =
        getActualTypeFromName(typeEnv, recordTypeName)->u.typeRecord->fieldList;

    while (givenRecordFields && typeFields)
    {
        if (!isOneFieldMatches(
                varAndFuncEnv, typeEnv, typeFields, givenRecordFields))
            return false;

        givenRecordFields = givenRecordFields->next;
        typeFields = typeFields->tails;
    }
    //  field number not match!
    if (givenRecordFields || typeFields)    return false;
    else                                    return true;
}

bool isOneFieldMatches(
    myTable varAndFuncEnv, myTable typeEnv,
    myTypeFieldList typeFields, myRecordFieldList givenRecordFields)
{
    myType fieldType = typeFields->head->type;
    myType valueType = 
        MySemantic_Exp(varAndFuncEnv, typeEnv, givenRecordFields->field->varValue)
            ->type;
    mySymbol fieldName = typeFields->head->name;
    mySymbol givenName = givenRecordFields->field->varName;

    if (!isTypeAndNameMatchesOrNil(fieldType, valueType, 
         fieldName, givenName))
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__RECORD_CREATION__FIELDS_NOT_MATCH,
                MySymbol_GetName(givenName));
        return false;
    }
    return true;
}

bool isTypeAndNameMatchesOrNil(
    myType fieldType, myType valueType,
    mySymbol fieldName, mySymbol givenName)
{
    return MySymbol_IsSymbolEqual(givenName, fieldName) &&
        isFormalTypeMatchesRealTypeOrNil(fieldType, valueType);
}

//////////////////////////////////////////////////////////////////////////////////////

//  forward declarations
void processArithmeticErrors(bool isLeftExpInt, bool isRightExpInt);

//  FORM:
//      exp op exp
//  DO:
//      1.check whether the operands are all of Integer type.
//      2.return myTypeInt type.
//  RETURN:
//      if it succeed, it return a myTranslationAndType variable containing
//          result type and intermediate code;
//      if it fails, it reurn SEMANTIC_ERROR;
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_ArithmeticExp(
    myTable varAndFuncEnv, myTable typeEnv, myArithmeticExp arithmeticExp)
{
    bool isLeftExpInt = isExpInt(varAndFuncEnv, typeEnv, arithmeticExp->left);
    bool isRightExpInt = isExpInt(varAndFuncEnv, typeEnv, arithmeticExp->right);

    if (isLeftExpInt && isRightExpInt)
        return make_MyTranslationAndType(NULL, makeType_Int());
    else
    {
        processArithmeticErrors(isLeftExpInt, isRightExpInt);
        return SEMANTIC_ERROR;
    }
}


void processArithmeticErrors(bool isLeftExpInt, bool isRightExpInt)
{
    if (!isLeftExpInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__ARITHMETIC__EXPRESSION_TYPE_NOT_INT, "left operand");
    if (!isRightExpInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__ARITHMETIC__EXPRESSION_TYPE_NOT_INT, "right operand");
}

/////////////////////////////////////////////////////////////////////////////////

//  FORM:
//      (exp)
//  DO:
//      nothing(parentheses can appear around any expressions)
//      1.return inner expression's type.
//  RETURN:
//      if t succeed, it returns a myTranslationAndType variable containing the
//          type of inner expression;
//      if it fails, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_ParenthesesExp(
    myTable varAndFuncEnv, myTable typeEnv, myParenthesesExp parenthesesExp)
{
    return MySemantic_Exp(varAndFuncEnv, typeEnv, parenthesesExp->exp);
}

///////////////////////////////////////////////////////////////////////////////

//  FORM:
//      ()
/**
 *  @brief  Semantic analysis of NoValueExp.
 *
 *          1.return myTypeNoReturn type.
 *
 *  @return A \b valid(not SEMANTIC_ERROR) myTranslationAndType variable.
 *  @remark Since NoValueExp only consists of parentheses, there is no more
 *          semantic checking inside this method and the return type is
 *          myTypeNoReturn.
 *
 *  @note   Tested
 */
myTranslationAndType MySemantic_NoValueExp(
    myTable varAndFuncEnv, myTable typeEnv, myNoValueExp noValueExp)
{
    //  no checking needed here
    return make_MyTranslationAndType(NULL, makeType_NoReturn());
}

////////////////////////////////////////////////////////////////////////////
//  todo: break
//  FORM:
//      (exp;exp;...exp)
//  DO:
//      analyze the last expression.
//  RETURN:
//       if it succeed, it returns v myTranslationAndType variable containing
//          the type of the last expression;
//       if it fails, it returns SEMANTIC_ERROR;
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_SequencingExp(
    myTable varAndFuncEnv, myTable typeEnv, mySequencingExp sequencingExp)
{
    //  expressions inside sequence-expression must be two or more than two
    assert (sequencingExp->exp1 && sequencingExp->exp2);

    //  analyze and return the result of the last expression
    if (sequencingExp->nextList == NULL)
    {
        myTranslationAndType resultOne =
            MySemantic_Exp(varAndFuncEnv, typeEnv, sequencingExp->exp1); 
        myTranslationAndType resultTwo =
            MySemantic_Exp(varAndFuncEnv, typeEnv, sequencingExp->exp2);

        if (resultOne != SEMANTIC_ERROR && resultTwo != SEMANTIC_ERROR)
            return resultTwo;
        else
            return SEMANTIC_ERROR;
    }
    else
    {
        myExpList rests = sequencingExp->nextList;
        while (rests->next)
            rests = rests->next;
        return MySemantic_Exp(varAndFuncEnv, typeEnv, rests->exp);
    }
}

//////////////////////////////////////////////////////////////////////////////

//  forward declarations
void addLoopVarToScope(myTable varAndFuncEnv, mySymbol varName);
void processForErrors(bool isLowRangeInt, bool isHighRangeInt, bool isBodyNoReturn);

//  todo: break
//  FORM:
//      for id := exp1 to exp2 do exp3
//  DO:
//      1.check whether `exp1` and `e`xp2 is of Integer type
//      2.check whether `exp3` has no return.
//  RETURN:
//      if it succeed, it returns a myTranslationAndType variable containing
//          the type of the for-expression result;
//      if it fails, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_ForExp(
    myTable varAndFuncEnv, myTable typeEnv, myForExp forExp)
{
    MySymbol_BeginScope(varAndFuncEnv);

    bool isLowRangeInt = isExpInt(varAndFuncEnv, typeEnv, forExp->varRangeLow);
    bool isHighRangeInt = isExpInt(varAndFuncEnv, typeEnv, forExp->varRangeHigh);
    if (isLowRangeInt && isHighRangeInt)
        addLoopVarToScope(varAndFuncEnv, forExp->varName);

    enterLoop();
    bool isBodyNoReturn = isExpNoReturn(varAndFuncEnv, typeEnv, forExp->bodyExp);  
    leaveLoop();

    MySymbol_EndScope(varAndFuncEnv);

    if (isLowRangeInt && isHighRangeInt && isBodyNoReturn)
        return make_MyTranslationAndType(NULL, makeType_NoReturn());
    else
    {
        processForErrors(isLowRangeInt, isHighRangeInt, isBodyNoReturn);
        return SEMANTIC_ERROR;
    }
}

void addLoopVarToScope(myTable varAndFuncEnv, mySymbol varName)
{
    myType loopVarType = makeType_Int();
    myVarAndFuncEntry varEntry = myEnvironment_makeVarEntry(loopVarType);
    MyEnvironment_addNewVarOrFunc(varAndFuncEnv, varName, varEntry);
}

void processForErrors(bool isLowRangeInt, bool isHighRangeInt, bool isBodyNoReturn)
{
    if (!isLowRangeInt)
        MyError_pushErrorCodeWithMsg(ERROR__FOR__EXPRESSION_NOT_TYPE_INT,
            "low range of for-exp");
    if (!isHighRangeInt)
        MyError_pushErrorCodeWithMsg(ERROR__FOR__EXPRESSION_NOT_TYPE_INT,
            "high range of for-exp");
    if (!isBodyNoReturn)
        MyError_pushErrorCodeWithMsg(ERROR__FOR__EXPRESSION_NOT_TYPE_NORETURN,
            "body of for-exp");
}

///////////////////////////////////////////////////////////////////////////////

//  forward declarations
myType getThenElseResultType(
    myTable varAndFuncEnv, myTable typeEnv, myIfThenElseExp ifThenElseExp,
    bool isThenClauseLegal, bool isElseClauseLegal);
void processIfThenElseErrors(
    bool isConditionExpInt, bool isThenClauseLegal,
    bool isElseClauseLegal, bool isThenAndElseClauseSameType);

//  todo:   break
//  FORM:
//      if exp1 then exp2 else exp3
//  DO:
//      1.check whether `exp1` is of integer type.
//      2.check whether `exp2` and `exp3` has the same expression type(or
//      both have no return).
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing
//      this expression's type and its intermediate code;
//      if failed, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_IfThenElseExp(
    myTable varAndFuncEnv, myTable typeEnv, myIfThenElseExp ifThenElseExp)
{
    bool isConditionExpInt = isExpInt(varAndFuncEnv, typeEnv, ifThenElseExp->exp1);
    bool isThenClauseLegal = isExpLegal(varAndFuncEnv, typeEnv, ifThenElseExp->exp2);
    bool isElseClauseLegal = isExpLegal(varAndFuncEnv, typeEnv, ifThenElseExp->exp3);
    
    myType resultType = getThenElseResultType(
        varAndFuncEnv, typeEnv, ifThenElseExp,
        isThenClauseLegal, isElseClauseLegal);
    bool isThenAndElseClauseSameType = (resultType != NULL);

    if (isConditionExpInt && isThenAndElseClauseSameType)
            return make_MyTranslationAndType(NULL, resultType);
    else
    {
        processIfThenElseErrors(isConditionExpInt, isThenClauseLegal, 
            isElseClauseLegal, isThenAndElseClauseSameType);
        return SEMANTIC_ERROR;
    }
}

//  forwards
bool isTwoClausesTypeMatchesOrNil(
    myType thenClauseType, myType elseClauseType);

//  if the then-clause and else-clause are both legal, it returns the result
//      type of `IfThenElseExp`;
//  otherwise, it returns NULL; 
myType getThenElseResultType(
    myTable varAndFuncEnv, myTable typeEnv, myIfThenElseExp ifThenElseExp,
    bool isThenClauseLegal, bool isElseClauseLegal)
{
    if (isThenClauseLegal && isElseClauseLegal)
    {
        myType thenClauseType = MySemantic_Exp(varAndFuncEnv, typeEnv, ifThenElseExp->exp2)
            ->type;
        myType elseClauseType = MySemantic_Exp(varAndFuncEnv, typeEnv, ifThenElseExp->exp3)
            ->type;
        if (isTwoClausesTypeMatchesOrNil(thenClauseType, elseClauseType))
            return thenClauseType;
    }

    return NULL;
}

//  forwards
bool isOneRecordTypeAnotherNil(myType oneType, myType anotherType);

bool isTwoClausesTypeMatchesOrNil(myType thenClauseType, myType elseClauseType)
{
    return isTypeEqual(thenClauseType, elseClauseType) ||
           isOneRecordTypeAnotherNil(thenClauseType, elseClauseType);
}

bool isOneRecordTypeAnotherNil(myType oneType, myType anotherType)
{
    return isNilValueOfRecordType(oneType, anotherType) ||
           isNilValueOfRecordType(anotherType, oneType);
}

void processIfThenElseErrors(
    bool isConditionExpInt, bool isThenClauseLegal,
    bool isElseClauseLegal, bool isThenAndElseClauseSameType)
{
    if (!isConditionExpInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__IF_THEN_ELSE__IF_CONDITION_NOT_TYPE_INT, "if-condition");

    if (!isThenClauseLegal || !isElseClauseLegal)
        MyError_pushErrorCodeWithMsg(
            ERROR__IF_THEN_ELSE__CLAUSE_NOT_LEGAL, "if-then or if-else");
    if (!isThenAndElseClauseSameType)
        MyError_pushErrorCodeWithMsg(
            ERROR__IF_THEN_ELSE__CLAUSE_TYPE_NOT_MATCH, "if-then-else");
}

/////////////////////////////////////////////////////////////////////////////////

//  forward declarations
void processIfThenErrors(bool isIfConditionInt, bool isThenClauseNoReturn);

//  todo:   break
//  FORM:
//      if exp1 then exp2
//  DO:
//      1.check whether if-condition(exp1) is of Integer type.
//      2.check whether then-clause(exp2) is of NoReturn type.
//      3.return NoReturn type.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing
//          the intermediate code and this expression's result type;
//      if failed, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_IfThenExp(
    myTable varAndFuncEnv, myTable typeEnv, myIfThenExp ifThenExp)
{
    bool isIfConditionInt = isExpInt(varAndFuncEnv, typeEnv, ifThenExp->exp1);
    bool isThenClauseNoReturn = isExpNoReturn(varAndFuncEnv, typeEnv, ifThenExp->exp2);

    if (isIfConditionInt && isThenClauseNoReturn)
        return make_MyTranslationAndType(NULL, makeType_NoReturn());
    else
    {
        processIfThenErrors(isIfConditionInt, isThenClauseNoReturn);
        return SEMANTIC_ERROR;
    }
}

void processIfThenErrors(bool isIfConditionInt, bool isThenClauseNoReturn)
{
    if (!isIfConditionInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__IF_THEN__IF_CONDITION_NOT_TYPE_INT, "if-condition");
    if (!isThenClauseNoReturn)
        MyError_pushErrorCodeWithMsg(
            ERROR__IF_THEN__THEN_CLAUSE_TYPE_NOT_NORETURN, "then-clause");
}

///////////////////////////////////////////////////////////////////////////////

//  forward declarations
bool isTwoComparisonOperandsTypeEqualsOrNil(
    myTable varAndFuncEnv, myTable typeEnv, myComparisonExp comparisonExp);
void processComparisonErrors(
    bool isLeftOperandLegal, bool isRightOperandLegal, bool isOperandsTypeMatches);

//  FORM:
//      exp1 op exp2
//  DO:
//      1.check whether both operands are of same type(or nil of Record).
//      2.check whether their types are not NoReturn.
//      3.return Integer 1 for true, 0 for false.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing
//          the intermediate code and this expression's result type;
//      if failed, it returns SEMANTIC_ERROR.  
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_ComparisonExp(
    myTable varAndFuncEnv, myTable typeEnv, myComparisonExp comparisonExp)
{
    bool isLeftOperandLegal = isExpLegal(
        varAndFuncEnv, typeEnv, comparisonExp->left);
    bool isRightOperandLegal = isExpLegal(
        varAndFuncEnv, typeEnv, comparisonExp->right);

    bool isOperandsTypeMatches = false;
    if (isLeftOperandLegal && isRightOperandLegal)
    {
        isOperandsTypeMatches = isTwoComparisonOperandsTypeEqualsOrNil(
            varAndFuncEnv, typeEnv, comparisonExp);
    }

    if (isLeftOperandLegal && isRightOperandLegal && isOperandsTypeMatches)
        return make_MyTranslationAndType(NULL, makeType_Int());
    else
    {
        processComparisonErrors(
            isLeftOperandLegal, isRightOperandLegal, isOperandsTypeMatches);
        return SEMANTIC_ERROR;
    }
}

bool isTwoComparisonOperandsTypeEqualsOrNil(
    myTable varAndFuncEnv, myTable typeEnv, myComparisonExp comparisonExp)
{
    myType leftType = getExpActualResultType(
        varAndFuncEnv, typeEnv, comparisonExp->left);
    myType rightType = getExpActualResultType(
        varAndFuncEnv, typeEnv, comparisonExp->right);

    if (isTypeNoReturn(leftType) || isTypeNoReturn(rightType))
        return false;
    else
        return  isTypeEqual(leftType, rightType) ||
                isOneRecordTypeAnotherNil(leftType, rightType);
}

void processComparisonErrors(
    bool isLeftOperandLegal, bool isRightOperandLegal,
    bool isOperandsTypeMatches)
{
    if (!isLeftOperandLegal)
        MyError_pushErrorCodeWithMsg(ERROR__COMPARISON__OPERAND_EXPRESSION_NOT_LEGAL,
            "left");
    if (!isRightOperandLegal)
        MyError_pushErrorCodeWithMsg(ERROR__COMPARISON__OPERAND_EXPRESSION_NOT_LEGAL,
            "right");
    if (!isOperandsTypeMatches)
        MyError_pushErrorCode(ERROR__COMPARISON__OPERANDS_TYPE_NOT_MATCH);
}

/////////////////////////////////////////////////////////////////////////////////

//  forward declarations
void processBooleanOperateErrors(bool isLeftOperandInt, bool isRightOperandInt);

//  FORM:
//      exp1 op exp2
//  DO:
//      1.check whether two operands are of Integer type.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing
//          the intermediate code and result type of this expression;
//      if failed, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_BooleanOperateExp(
    myTable varAndFuncEnv, myTable typeEnv, myBooleanOperateExp booleanOperateExp)
{
    bool isLeftOperandInt =
        isExpInt(varAndFuncEnv, typeEnv, booleanOperateExp->left);
    bool isRightOperandInt = 
        isExpInt(varAndFuncEnv, typeEnv, booleanOperateExp->right);

    if (isLeftOperandInt && isRightOperandInt)
        return make_MyTranslationAndType(NULL, makeType_Int());
    else
    {
        processBooleanOperateErrors(isLeftOperandInt, isRightOperandInt);
        return SEMANTIC_ERROR;
    }    
}

void processBooleanOperateErrors(bool isLeftOperandInt, bool isRightOperandInt)
{
    if (!isLeftOperandInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__BOOLEAN_OPERATE__OPERAND_NOT_INT, "left");
    if (!isRightOperandInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__BOOLEAN_OPERATE__OPERAND_NOT_INT, "right");
}

////////////////////////////////////////////////////////////////////////////////

//  forwards
bool isAssignmentOperandsTypeMatchOrNil(
    myTable varAndFuncEnv, myTable typeEnv, myAssignmentExp assignmentExp);
void processAssignmentErrors(
    bool isLeftOperandLegal, bool isRightOperandLegal, bool isOperandsTypeMatches);

//  FORM:
//      lvalue := exp
//  DO:
//      1.check whether two operands are legal expression.
//      2.check whether two operands are of the same type(or left Record right nil).
//  RETURN:
//      if it succeed, it returns a myTranslationAndType variable containing
//          the intermediate code and myTypeNoReturn type of this expression;
//      if it failed, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_AssignmentExp(
    myTable varAndFuncEnv, myTable typeEnv, myAssignmentExp assignmentExp)
{
    bool isLeftOperandLegal =
        isLValueExpLegal(varAndFuncEnv, typeEnv, assignmentExp->lValueExp);
    bool isRightOperandLegal =
        isExpLegal(varAndFuncEnv, typeEnv, assignmentExp->exp);

    bool isOperandsTypeMatches = false;
    if (isLeftOperandLegal && isRightOperandLegal)
    {
        isOperandsTypeMatches = isAssignmentOperandsTypeMatchOrNil(
            varAndFuncEnv, typeEnv, assignmentExp);
    }

    if (isLeftOperandLegal && isRightOperandLegal && isOperandsTypeMatches)
        return make_MyTranslationAndType(NULL, makeType_NoReturn());
    else
    {
        processAssignmentErrors(
            isLeftOperandLegal, isRightOperandLegal, isOperandsTypeMatches);
        return SEMANTIC_ERROR;
    }   
}

bool isAssignmentOperandsTypeMatchOrNil(
    myTable varAndFuncEnv, myTable typeEnv, myAssignmentExp assignmentExp)
{
    myType leftOperandType =
        getLValueExpActualResultType(varAndFuncEnv, typeEnv, assignmentExp->lValueExp);
    myType rightOperandType =
        getExpActualResultType(varAndFuncEnv, typeEnv, assignmentExp->exp);

    if (isTypeNoReturn(leftOperandType) || isTypeNoReturn(rightOperandType))
        return false;
    else
        return  isTypeEqual(leftOperandType, rightOperandType) ||
                (isTypeRecord(leftOperandType) && isTypeNil(rightOperandType));
}

void processAssignmentErrors(
    bool isLeftOperandLegal, bool isRightOperandLegal, bool isOperandsTypeMatches)
{
    if (!isLeftOperandLegal)
        MyError_pushErrorCodeWithMsg(ERROR__ASSIGNMENT__OPERAND_NOT_LEGAL,
            "assignment-left-operand");
    if (!isRightOperandLegal)
        MyError_pushErrorCodeWithMsg(ERROR__ASSIGNMENT__OPERAND_NOT_LEGAL,
            "assignment-right-operand");
    if (!isOperandsTypeMatches)
        MyError_pushErrorCode(ERROR__ASSIGNMENT__OPERANDS_TYPE_NOT_MATCH);
}

///////////////////////////////////////////////////////////////////////////////

//  FORM:
//      var id := exp
//  DO:
//      1.check whether the `exp` expression is legal.
//      2.check whether the `exp` expression type is not myTypeNil and myTypeNoReturn.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Var_ShortForm(
    myTable varAndFuncEnv, myTable typeEnv, myShortFormVar shortFormVar)
{
    bool isValueExpLegal = isExpLegal(varAndFuncEnv, typeEnv, shortFormVar->exp);

    if (isValueExpLegal)
    {
        myType valueType =
            getExpActualResultType(varAndFuncEnv, typeEnv, shortFormVar->exp);
        if (!isTypeNil(valueType) && !isTypeNoReturn(valueType))
        {
            myVarAndFuncEntry varEntry = myEnvironment_makeVarEntry(valueType);
            MyEnvironment_addNewVarOrFunc(varAndFuncEnv, shortFormVar->name, varEntry);
            return true;
        }
        else
        {
            MyError_pushErrorCode(
                ERROR__VARDEC_SHORT__TYPE_CAN_NOT_BE_NIL_NORETURN);
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

//  forwards
bool isVarDecValueTypeMatchesGivenType(
    myTable varAndFuncEnv, myTable typeEnv, myLongFormVar longFormVar);
void processLongFormVarDecErrors(
    bool isGivenTypeDefined, bool isValueExpLegal,
    bool isTypeMatches, mySymbol typeName);

//  FORM:
//      var id:type-id := exp
//  DO:
//      1.check whether type `type-id` is already defined.
//      2.check whether `exp` type matches type `type-id`.
//      3.`exp` can be nil when `type-id` is of record type(or nil
//          when `type-id`is of Record Type).
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Var_LongForm(
    myTable varAndFuncEnv, myTable typeEnv, myLongFormVar longFormVar)
{
    bool isVariableTypeDefined = isTypeDefined(typeEnv, longFormVar->type);
    bool isValueExpLegal = isExpLegal(varAndFuncEnv, typeEnv, longFormVar->exp);

    bool isTypeMatches = false;
    if (isVariableTypeDefined && isValueExpLegal)
    {
        isTypeMatches = isVarDecValueTypeMatchesGivenType(
            varAndFuncEnv, typeEnv, longFormVar);
    }

    //    hidden checking for the other two booleans
    if (isTypeMatches)
    {
        myVarAndFuncEntry varEntry =
            myEnvironment_makeVarEntry(getActualTypeFromName(typeEnv, longFormVar->type));
        MyEnvironment_addNewVarOrFunc(varAndFuncEnv, longFormVar->name,varEntry);
        return true;
    }
    else
    {
        processLongFormVarDecErrors(
            isVariableTypeDefined, isValueExpLegal, isTypeMatches, longFormVar->type);
        return false;
    }
}

bool isVarDecValueTypeMatchesGivenType(
    myTable varAndFuncEnv, myTable typeEnv, myLongFormVar longFormVar)
{
    myType valueType = 
        getExpActualResultType(varAndFuncEnv, typeEnv, longFormVar->exp);
    myType givenType = 
        getActualTypeFromName(typeEnv, longFormVar->type);

    return isTypeEqual(valueType, givenType) ||
            isNilValueOfRecordType(givenType, valueType);
}

void processLongFormVarDecErrors(
    bool isVariableTypeDefined, bool isValueExpLegal,
    bool isTypeMatches, mySymbol typeName)
{
    if (!isVariableTypeDefined)
        MyError_pushErrorCodeWithMsg(
            ERROR__VARDEC_LONG__TYPE_NOT_DEFINED,
            MySymbol_GetName(typeName));
    if (!isValueExpLegal)
        MyError_pushErrorCode(
            ERROR__VARDEC_LONG__VALUE_EXPRESSION_NOT_LEGAL);
    if (!isTypeMatches)
        MyError_pushErrorCodeWithMsg(
            ERROR__VARDEC_LONG__OPERANDS_TYPE_NOT_MATCH,
            MySymbol_GetName(typeName));
}

////////////////////////////////////////////////////////

//  FORM:
//      var id := exp
//      var id : type-id := exp
//  DO:
//      delegate
bool MySemantic_Dec_Var(myTable varAndFuncEnv, myTable typeEnv, myVarDec varDec)
{
    if (varDec == NULL)     return true;

    switch (varDec->kind)
    {
        case ShortFormVar:
            return MySemantic_Dec_Var_ShortForm(
                varAndFuncEnv, typeEnv, varDec->u.shortFormVar);
        case LongFormVar:
            return MySemantic_Dec_Var_LongForm(
                varAndFuncEnv, typeEnv, varDec->u.longFormVar);
        default:
            assert (false);
    }
}

//////////////////////////////////////////////////////////////////////

//  FORM:
//      decs -> {dec}
//  DO:
//      1.do semantic analyzing on all these declarations.
//  RETURN:
//      if all declarations succeed, it returns true;
//      otherwise, it returns false.
//  STATUS:
//      Tested.
bool MySemantic_Decs(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs)
{
    return MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, decs);
}

///////////////////////////////////////////////////////////////////////////////

//  forwards
bool areExpressionsLegal(
    myTable varAndFuncEnv, myTable typeEnv, myExpList exps);
myType getLastExpResultType(
    myTable varAndFuncEnv, myTable typeEnv, myExpList exps);
void processLetErrors(
    bool areDeclarationsLegal, bool isBodyLegal);

//  todo:   break
//  FORM:
//      let decs in expseq end
//  DO:
//      1.evalute the declarations `decs`, binding variables, funcs whose scope
//          extends over the `expseq` expression which consists of zero or more
//          expressions.
//      2.check whether all expression are legal.
//  REUTURN:
//      if succeed, it returns a myTranslationAndType variable containing the 
//          intermedaite code and the result type(if any) of this expression;
//      if failed, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_LetExp(
    myTable varAndFuncEnv, myTable typeEnv, myLetExp letExp)
{
    MySymbol_BeginScope(varAndFuncEnv);
    MySymbol_BeginScope(typeEnv);

    bool areDeclarationsLegal =
        MySemantic_Decs_Recursive(varAndFuncEnv, typeEnv, letExp->decList);
    bool isBodyLegal =
        areExpressionsLegal(varAndFuncEnv, typeEnv, letExp->expList);

    
    myTranslationAndType result;
    if (areDeclarationsLegal && isBodyLegal)
        result = make_MyTranslationAndType(NULL,
            getLastExpResultType(varAndFuncEnv, typeEnv, letExp->expList));
    else
    {
        processLetErrors(areDeclarationsLegal, isBodyLegal);
        result = SEMANTIC_ERROR;
    }

    MySymbol_EndScope(varAndFuncEnv);
    MySymbol_EndScope(typeEnv);
    return result;
}

bool areExpressionsLegal(
    myTable varAndFuncEnv, myTable typeEnv, myExpList exps)
{
    while (exps)
    {
        if (MySemantic_Exp(varAndFuncEnv, typeEnv, exps->exp)
            == SEMANTIC_ERROR)
            return false;
        exps = exps->next;
    }
    return true;
}

myType getLastExpResultType(
    myTable varAndFuncEnv, myTable typeEnv, myExpList exps)
{
    if (exps == NULL)   return makeType_NoReturn();

    while (exps->next)
        exps = exps->next;
    return getExpActualResultType(varAndFuncEnv, typeEnv, exps->exp);
}

void processLetErrors(bool areDeclarationsLegal, bool isBodyLegal)
{
    if (!areDeclarationsLegal)
        MyError_pushErrorCode(ERROR__LET__ECLARATIONS_ILLEGAL);
    if (!isBodyLegal)
        MyError_pushErrorCode(ERROR__LET__BODY_ILLEGAL);
}

//////////////////////////////////////////////////////////////////////////////////

//  forwards
void processWhileErrors(bool isConditionInt, bool isBodyNoReturn);

//  todo:   break
//  FORM:
//      while exp1 do exp2
//  DO:
//      1.check whether while-condition expression is legal expression
//           and of Integer type.
//      2.check whether exp2 is of myTypeNoReturn type.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing the 
//          intemediate code and result myTypeNoReturn type of this expression;
//      if failed, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_WhileExp(
    myTable varAndFuncEnv, myTable typeEnv, myWhileExp whileExp)
{
    bool isConditionInt =
        isExpInt(varAndFuncEnv, typeEnv, whileExp->whileExp);

    enterLoop();
    bool isBodyNoReturn =
        isExpNoReturn(varAndFuncEnv, typeEnv, whileExp->bodyExp);
    leaveLoop();

    if (isConditionInt && isBodyNoReturn)
        return make_MyTranslationAndType(NULL, makeType_NoReturn());
    else
    {
        processWhileErrors(isConditionInt, isBodyNoReturn);
        return SEMANTIC_ERROR;
    }
}

void processWhileErrors(bool isConditionInt, bool isBodyNoReturn)
{
    if (!isConditionInt)
        MyError_pushErrorCode(ERROR__WHILE__CONDITION_NOT_TYPE_INT);
    if (!isBodyNoReturn)
        MyError_pushErrorCode(ERROR__WHILE__BODY_NOT_TYPE_NORETURN);
}

//////////////////////////////////////////////////////////////////////////////////

//  forwards
void processNegationErrors(bool isFollowingExpInt);

//  FORM:
//      -exp
//  DO:
//      1.check whether the following expression is of Integer type.
//  RETURN:
//      if succeed, it returns a myTranslationAndType variable containing the
//          intermediate code and the myTypeInt result type of this expression;
//      if failed, it returns SEMANTIC_ERROR.
//  STATUS:
//      Tested.
myTranslationAndType MySemantic_NegationExp(
    myTable varAndFuncEnv, myTable typeEnv, myNegationExp negationExp)
{
    bool isFollowingExpInt = isExpInt(varAndFuncEnv, typeEnv, negationExp->exp);
    if (isFollowingExpInt)
        return make_MyTranslationAndType(NULL, makeType_Int());
    else
    {
        processNegationErrors(isFollowingExpInt);
        return SEMANTIC_ERROR;
    } 
}

void processNegationErrors(bool isFollowingExpInt)
{
    if (!isFollowingExpInt)
        MyError_pushErrorCodeWithMsg(
            ERROR__NEGATIVE__EXPRESSION_NOT_TYPE_INT, "negation");
}

//////////////////////////////////////////////////////////////////////////////////

/**
 *  @brief  Semantic analysis of BreakExp.
 *
 *  @return A \b valid(not SEMANTIC_ERROR) myTranslationAndType variable.
 *  @remark Since BreakExp consists of nothing, there is no more semantic
 *          checking inside this method and the return type is myTypeNoReturn.
 *
 *  @note   Tested
 */
myTranslationAndType MySemantic_BreakExp(
    myTable varAndFuncEnv, myTable typeEnv, myBreakExp breakExp)
{
    if (isBreakInsideLoop())
        return make_MyTranslationAndType(NULL, makeType_NoReturn());
    else
    {
        MyError_pushErrorCode(ERROR__BREAK__NOT_INSIDE_LOOP);
        return SEMANTIC_ERROR;
    }
}

//////////////////////////////////////////////////////////////////////////
//
//  public function

myTranslationAndType MySemantic_Exp(
    myTable varAndFuncEnv, myTable typeEnv, myExp exp)
{
    switch (exp->kind)
    {
        case LValueExp:         
            return MySemantic_LValueExp(varAndFuncEnv, typeEnv, exp->u.lValueExp);
        case FunctionCallExp:
            return MySemantic_FunctionCallExp(varAndFuncEnv, typeEnv, exp->u.functionCallExp);
        case NilExp:
            return MySemantic_NilExp(varAndFuncEnv, typeEnv, exp->u.nilExp);
        case IntegerLiteralExp:
            return MySemantic_IntegerLiteralExp(varAndFuncEnv, typeEnv, exp->u.integerLiteralExp);
        case StringLiteralExp:
            return MySemantic_StringLiteralExp(varAndFuncEnv, typeEnv, exp->u.stringLiteralExp);
        case ArrayCreationExp:
            return MySemantic_ArrayCreationExp(varAndFuncEnv, typeEnv, exp->u.arrayCreationExp);
        case RecordCreationExp:
            return MySemantic_RecordCreationExp(varAndFuncEnv, typeEnv, exp->u.recordCreationExp);
        case ArithmeticExp:
            return MySemantic_ArithmeticExp(varAndFuncEnv, typeEnv, exp->u.arithmeticExp);
        case ParenthesesExp:
            return MySemantic_ParenthesesExp(varAndFuncEnv, typeEnv, exp->u.parenthesesExp);
        case NoValueExp:
            return MySemantic_NoValueExp(varAndFuncEnv, typeEnv, exp->u.noValueExp);
        case SequencingExp:
            return MySemantic_SequencingExp(varAndFuncEnv, typeEnv, exp->u.sequencingExp);
        case ForExp:
            return MySemantic_ForExp(varAndFuncEnv, typeEnv, exp->u.forExp);
        case IfThenElseExp:
            return MySemantic_IfThenElseExp(varAndFuncEnv, typeEnv, exp->u.ifThenElseExp);
        case IfThenExp:
            return MySemantic_IfThenExp(varAndFuncEnv, typeEnv, exp->u.ifThenExp);
        case ComparisonExp:
            return MySemantic_ComparisonExp(varAndFuncEnv, typeEnv, exp->u.comparisonExp);
        case BooleanOperateExp:
            return MySemantic_BooleanOperateExp(varAndFuncEnv, typeEnv, exp->u.booleanOperateExp);
        case AssignmentExp:
            return MySemantic_AssignmentExp(varAndFuncEnv, typeEnv, exp->u.assignmentExp);
        case LetExp:
            return MySemantic_LetExp(varAndFuncEnv, typeEnv, exp->u.letExp);
        case WhileExp:
            return MySemantic_WhileExp(varAndFuncEnv, typeEnv, exp->u.whileExp);
        case NegationExp:
            return MySemantic_NegationExp(varAndFuncEnv, typeEnv, exp->u.negationExp);
        case BreakExp:
            return MySemantic_BreakExp(varAndFuncEnv, typeEnv, exp->u.breakExp);

        default:            assert(false);
    }
}


/////////////////////////////////////////////////////////////////////////////////////