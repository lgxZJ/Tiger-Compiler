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
myType getExpActualResultType(myExp exp);
static myType getActualType(myType type);
myType getActualTypeFromName(mySymbol typeName);
myTranslationAndType MySemantic_LValueExp(myLValueExp lValueExp);

////////////////////////////////////////////////////////////////////////
//
//  Exp Checkers

//  DO:
//      analyze if this expression fits its semantic
bool isExpLegal(myExp exp)
{
    assert (exp);

    myTranslationAndType result = MySemantic_Exp(exp);
        
    if (result == SEMANTIC_ERROR)   return false;
    else                            return true;
}

bool isExpOneTypeOrIllegal(myExp exp, enum TypeKind kind)
{
    assert (exp);

    myTranslationAndType result = 
        MySemantic_Exp(exp);
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

static bool isExpInt(myExp exp)
{
    return isExpOneTypeOrIllegal(exp, TypeInt);
}

bool isExpNoReturn(myExp exp)
{
    return isExpOneTypeOrIllegal(exp, TypeNoReturn);
}

//  NOTE:   array type named arrayTypeName must be valid. 
static bool isExpThisArrayElement(
    myExp exp, mySymbol arrayTypeName)
{
    assert (exp && arrayTypeName);

    myType arrayElementType = getActualType(
        getActualTypeFromName(arrayTypeName)
        ->u.typeArray->type);
    myType initialExpType = getActualType(
        MySemantic_Exp(exp)->type); 
    return isTypeEqual(arrayElementType, initialExpType);
}

bool isLValueExpLegal(myLValueExp lValueExp)
{
    myTranslationAndType result =
        MySemantic_LValueExp(lValueExp);
    return result != SEMANTIC_ERROR;
}

//  NOTE:   expression must ba valid.
myType getExpActualResultType(myExp exp)
{
    assert (exp);

    myTranslationAndType result = MySemantic_Exp(exp);
    assert (result != SEMANTIC_ERROR);

    return getActualType(result->type);
}

myType getLValueExpActualResultType(myLValueExp  lValueExp)
{
    assert (lValueExp);

    myTranslationAndType result = MySemantic_LValueExp(lValueExp);
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
static myVarAndFuncEntry getVarOrFuncFromName(mySymbol symbol)
{
    assert (symbol);
    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    return MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, symbol);
}

//  NOTE:
//      must be called after isVariableDeclared.
static myType getVariableType(mySymbol variableName)
{
    assert (variableName);

    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    return MyEnvironment_getVarType(
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, variableName));
}

//  NOTE:
//      must be called after isFuncDefined.
static myType getFunctionReturnType(mySymbol functionName)
{
    assert (functionName);

    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    return MyEnvironment_getFuncReturnType(
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, functionName));
}

//  NOTE:
//      must be called after isFuncDefined.
static myTypeList getFunctionFormalTypes(mySymbol functionName)
{
    assert (functionName);

    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    return MyEnvironment_getFuncFormalTypes(
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, functionName));
}

///////////////////////////////////////////////////////////////////

static bool isVariableDeclared(mySymbol variableName)
{
    assert (variableName);

    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    myVarAndFuncEntry entry = 
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, variableName);

    if (entry == NULL || !myEnvironment_isVarEntry(entry))
        return false;
    else
        return true;
}

static bool isFuncDefined(mySymbol funcSymbol)
{
    assert (funcSymbol);

    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    myVarAndFuncEntry entry = 
        MyEnvironment_getVarOrFuncFromName(varAndFuncEnv, funcSymbol);

    if (entry == NULL || !myEnvironment_isFuncEntry(entry))
        return false;
    else
        return true;
}

bool isTypeDefined(mySymbol typeName)
{
    assert (typeName);

    myTable typeEnv = MySemantic_getTypeEnvironment();
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
myType getActualTypeFromName(mySymbol typeName)
{
    myTable typeEnv = MySemantic_getTypeEnvironment();
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
    assert (varAndFuncEnv);
    g_varAndFuncEnv = varAndFuncEnv;
}

void MySemantic_setTypeEnvironment(myTable typeEnv)
{
    assert (typeEnv);
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
    myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_RecordField(
    myLValueExp lValueExp);
myTranslationAndType MySemantic_LValueExp_ArraySubscript(
    myLValueExp lValueExp);

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
    myLValueExp lValueExp)
{
    switch (lValueExp->kind)
    {
        case SimpleVar:
            return MySemantic_LValueExp_SimpleVar(lValueExp);
            break;
        case RecordField:
            return MySemantic_LValueExp_RecordField(lValueExp);
            break;
        case ArraySubscript:
            return MySemantic_LValueExp_ArraySubscript(lValueExp);
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
    myLValueExp lValueExp)
{
    if (!isVariableDeclared(lValueExp->id))
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__LVALUE_SIMPLE_VAR__VAR_NOT_DECLARED,
            MySymbol_GetName(lValueExp->id));
        return SEMANTIC_ERROR;
    }
    
    return make_MyTranslationAndType(NULL, 
        getVariableType(lValueExp->id));
}

///////////////////////////////////////////////

//  forward declarations
bool isVariableRecordType(mySymbol variableName);
myType typeContainsLValueAux(
    myType type, myLValueAux aux);
static myType arrayContainsLValueAux(
    myType type, myLValueAux aux);
static myType analyzeRecordFields(
    myLValueExp lValueExp);
static myType recordContainsLValueAux(
    myType type, myLValueAux aux);
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
    myLValueExp lValueExp)
{
    mySymbol recordVarName = lValueExp->id;

    bool isVarDeclared = isVariableDeclared(recordVarName);
    bool isVarAnRecord = false;
    
    if (isVarDeclared)
        isVarAnRecord = isVariableRecordType(recordVarName);
    if (isVarDeclared && isVarAnRecord)
    {
        myType returnType =
            analyzeRecordFields(lValueExp);
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


bool isVariableRecordType(mySymbol variableName)
{
     myVarAndFuncEntry entry = getVarOrFuncFromName(variableName);
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
    myLValueExp lValueExp)
{
    assert (lValueExp);

    myRecordFieldAux fieldAux = lValueExp->u.recordFieldAux;
    myType typeOfRecord = makeType_Record(
        getVariableType(lValueExp->id)
            ->u.typeRecord->fieldList); 
    myLValueAux aux = makeMyLValueAux(
        fieldAux->id, NULL, fieldAux->next);

    return typeContainsLValueAux(typeOfRecord, aux);
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
    myType type, myLValueAux aux)
{ 
    switch (type->kind)
    {
        case TypeArray:
            return arrayContainsLValueAux(type, aux);
        case TypeRecord:
            return recordContainsLValueAux(type, aux);
        default:    //  never goes here
            assert(false);
    }
}

//  check whether field name in `aux` can be found inside record type.
//  return NULL for checking error.
static myType recordContainsLValueAux(
    myType type, myLValueAux aux)
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
            return typeContainsLValueAux(
                typeReturn, aux->next);
        else
            return typeReturn;
    }
}

//  check whether the type of array subscript is Integer.
//  return NULL for checking error.
static myType arrayContainsLValueAux(
    myType type, myLValueAux aux)
{
    assert (isTypeArray(type));
    assert (getLValueKind(aux) == ArraySubscript);

    //  check subscript's expression is of Integer type
    myExp subscriptExp = aux->exp;
    if (!isExpInt(subscriptExp))
    {
        MyError_pushErrorCode(ERROR_SUBSCRIPT_NOT_INT);
        return NULL;
    }

    if (aux->next)  //  have next symbol or exp, do checking
        return typeContainsLValueAux(
                type->u.typeArray->type, aux->next);
    else
        return type->u.typeArray->type;
}

////////////////////////////////////////////////

//  forward declarations
bool isVariableArrayType(mySymbol variableName);
myTranslationAndType analyzeRecursiveArraySubscripts(
    myLValueExp lValueExp);
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
    myLValueExp lValueExp)
{
    mySymbol arrayVariableName = lValueExp->id;

    bool isArrayVariableDeclared =
        isVariableDeclared(arrayVariableName);
    bool isVariableAnArray = false;
    if (isArrayVariableDeclared) 
        isVariableAnArray = isVariableArrayType(arrayVariableName);

    myExp subscriptExp = lValueExp->u.arraySubscriptAux->exp;
    bool isSubscriptInt = isExpInt(subscriptExp);

    myTranslationAndType result;
    if (isArrayVariableDeclared && isVariableAnArray && isSubscriptInt)
        return analyzeRecursiveArraySubscripts(lValueExp);
    else    
    {
        processArraySubscriptErrors(isArrayVariableDeclared,
            isVariableAnArray, isSubscriptInt, lValueExp);
        return SEMANTIC_ERROR;
    }
}

bool isVariableArrayType(mySymbol variableName)
{
    assert (variableName);

    myVarAndFuncEntry entry = getVarOrFuncFromName(variableName);
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
    myLValueExp lValueExp)
{
    assert (lValueExp);

    myTranslationAndType result =
        make_MyTranslationAndType(NULL, NULL);
    
    //  caller make sures its a variable entry!
    myVarAndFuncEntry arrayEntry = getVarOrFuncFromName(lValueExp->id);
    assert (arrayEntry != NULL);

    if (lValueExp->u.arraySubscriptAux->next)
    {
        myType elementType = MyEnvironment_getVarType(arrayEntry)
            ->u.typeArray->type;

        //  recursive checking
        myType typeReturn = typeContainsLValueAux(
            elementType,
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
    myExpList formalVariables, myTypeList formalTypes,
    mySymbol functioName);
myTranslationAndType MySemantic_FunctionCallExp_Param(
    myParamFunctionCallExp paramFunctionCallExp);
myTranslationAndType MySemantic_FunctionCallExp_NoParam(
    myNoParamFunctionCallExp noParamFunctionCallExp);

/**
 *  @brief  Do semantic analysis on FunctionCallExp.
 *
 *  @remark The semantic analysis work of this function is to check:
 *              1.whether the function is already declared.
 *              2.if any, whether the formal parameters' type matches its signature.
 */
myTranslationAndType MySemantic_FunctionCallExp(
    myFunctionCallExp functionCallExp)
{
    switch (functionCallExp->kind)
    {
        case NoParamFunctionCallExp:
            return MySemantic_FunctionCallExp_NoParam(
                functionCallExp->u.noParamFunctionCallExp);
        case ParamFunctionCallExp:
            return MySemantic_FunctionCallExp_Param(
                functionCallExp->u.paramFunctionCallExp);
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
    myNoParamFunctionCallExp noParamFunctionCallExp)
{
    mySymbol functionName = noParamFunctionCallExp->name;
    if (!isFuncDefined(functionName))
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCTION_CALL__FUNC_NOT_DEFINED,
            MySymbol_GetName(functionName));
        return SEMANTIC_ERROR;
    }

    return make_MyTranslationAndType(
        NULL, getFunctionReturnType(functionName));
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
    myParamFunctionCallExp paramFunctionCallExp)
{
    mySymbol functionName = paramFunctionCallExp->name;
    if (!isFuncDefined(functionName))
    {
        MyError_pushErrorCodeWithMsg(
            ERROR__FUNCTION_CALL__FUNC_NOT_DEFINED,
            MySymbol_GetName(functionName));
        return SEMANTIC_ERROR;
    }

    myExpList formalVariables = paramFunctionCallExp->expList;
    myTypeList formalTypes = getFunctionFormalTypes(functionName);

    return analyzeParamsOfFunction(
        
        formalVariables, formalTypes, 
        functionName);
}

//  forwards
bool isFormalTypeMatchesRealTypeOrNil(
    myType formalType, myType valueType);

myTranslationAndType analyzeParamsOfFunction(
    myExpList formalVariables, myTypeList formalTypes,
    mySymbol functioName)
{
    assert (functioName);

    while (formalVariables && formalTypes)
    {
        myTranslationAndType realParamResult = 
            MySemantic_Exp(formalVariables->exp);
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
            NULL, getFunctionReturnType(functioName));
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
    myNilExp nilExp)
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
    myIntegerLiteralExp integerLiteralExp)
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
    myStringLiteralExp stringLiteralExp)
{
    //  no checking needed here
    return make_MyTranslationAndType(NULL, makeType_String());  
}

/////////////////////////////////////////////////////////////////////////////////

//  forward declarations
bool isTypeDefinedAsArray(
    mySymbol arrayTypeName);
bool isValueTypeMatchesArrayElementOrNil(
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
    myArrayCreationExp arrayCreationExp)
{
    mySymbol arrayTypeName = arrayCreationExp->typeName;

    bool isArrayTypeDefiend = isTypeDefinedAsArray(arrayTypeName);
    bool isSubscriptAnIntExp = isExpInt(arrayCreationExp->length);
    bool isInitialExpThisArrayElementType = false;

    if (isArrayTypeDefiend)
        isInitialExpThisArrayElementType = isValueTypeMatchesArrayElementOrNil(
            arrayCreationExp->initial, arrayTypeName);

    if (isArrayTypeDefiend && isSubscriptAnIntExp && isInitialExpThisArrayElementType)
        return make_MyTranslationAndType(NULL, getActualTypeFromName(arrayTypeName));
    else
    {
        processArrayCreationErrors(isArrayTypeDefiend, isSubscriptAnIntExp,
            isInitialExpThisArrayElementType, arrayTypeName);
        return SEMANTIC_ERROR;
    }
}

bool isValueTypeMatchesArrayElementOrNil(
    myExp initialValue, mySymbol arrayTypeName)
{
    myType arrayElementType = getActualType(
        getActualTypeFromName(arrayTypeName)
            ->u.typeArray->type);
    myType valueType = getActualType(
        getExpActualResultType(initialValue));

    return isExpThisArrayElement(
                initialValue, arrayTypeName) ||
           isNilValueOfRecordType(arrayElementType, valueType);
}

bool isTypeDefinedAsArray(mySymbol arrayTypeName)
{
    if (isTypeDefined(arrayTypeName))
        return isTypeArray(getActualTypeFromName(arrayTypeName));
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

bool isTypeDefinedAsRecord(mySymbol recordTypeName);
myTranslationAndType MySemantic_RecordCreationExp_NoField(
    myNoFieldRecordCreationExp noFieldRecordCreationExp);
myTranslationAndType MySemantic_RecordCreationExp_Field(
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
    myRecordCreationExp recordCreationExp)
{
    switch (recordCreationExp->kind)
    {
        case NoFieldRecordCreationExp:
            return MySemantic_RecordCreationExp_NoField(
                recordCreationExp->u.noFieldRecordCreationExp);
        case FieldRecordCreationExp:
            return MySemantic_RecordCreationExp_Field(
                recordCreationExp->u.fieldRecordCreationExp);
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
    myNoFieldRecordCreationExp noFieldRecordCreationExp)
{
    mySymbol recordTypeName = noFieldRecordCreationExp->typeName;

    if (isTypeDefinedAsRecord(recordTypeName))
        return make_MyTranslationAndType(NULL,
            getActualTypeFromName(recordTypeName));
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
    myFieldRecordCreationExp fieldRecordCreationExp)
{
    mySymbol recordTypeName = fieldRecordCreationExp->typeName;

    bool isRecordTypeDefined =
        isTypeDefinedAsRecord(recordTypeName);
    bool isCreationFieldsMatchThisRecordType = isRecordTypeDefined ?
            ifFieldNamesAndTypesMatches(
                fieldRecordCreationExp) :
            false;
    
    if (isRecordTypeDefined && isCreationFieldsMatchThisRecordType)
        return make_MyTranslationAndType(NULL,
            getActualTypeFromName(recordTypeName));
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

bool isTypeDefinedAsRecord(mySymbol recordTypeName)
{
    if (isTypeDefined(recordTypeName))
        return isTypeRecord(getActualTypeFromName(recordTypeName));
    return false;
}

//  forwards
bool isTypeAndNameMatchesOrNil(
    myType fieldType, myType valueType,
    mySymbol givenName, mySymbol fieldName);
bool isOneFieldMatches(
    myTypeFieldList typeFields, myRecordFieldList givenRecordFields);

//  NOTE:   must be called after isTypeNotRecordAndSetError.
bool ifFieldNamesAndTypesMatches(
    myFieldRecordCreationExp fieldRecordCreationExp)
{
    mySymbol recordTypeName = fieldRecordCreationExp->typeName;
    myRecordFieldList givenRecordFields = fieldRecordCreationExp->fieldList;
    myTypeFieldList typeFields =
        getActualTypeFromName(recordTypeName)->u.typeRecord->fieldList;

    while (givenRecordFields && typeFields)
    {
        if (!isOneFieldMatches(
                typeFields, givenRecordFields))
            return false;

        givenRecordFields = givenRecordFields->next;
        typeFields = typeFields->tails;
    }
    //  field number not match!
    if (givenRecordFields || typeFields)    return false;
    else                                    return true;
}

bool isOneFieldMatches(
    myTypeFieldList typeFields, myRecordFieldList givenRecordFields)
{
    myType fieldType = typeFields->head->type;
    myType valueType = 
        MySemantic_Exp(givenRecordFields->field->varValue)
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
    myArithmeticExp arithmeticExp)
{
    bool isLeftExpInt = isExpInt(arithmeticExp->left);
    bool isRightExpInt = isExpInt(arithmeticExp->right);

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
    myParenthesesExp parenthesesExp)
{
    return MySemantic_Exp(parenthesesExp->exp);
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
    myNoValueExp noValueExp)
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
    mySequencingExp sequencingExp)
{
    //  expressions inside sequence-expression must be two or more than two
    assert (sequencingExp->exp1 && sequencingExp->exp2);

    //  analyze and return the result of the last expression
    if (sequencingExp->nextList == NULL)
    {
        myTranslationAndType resultOne =
            MySemantic_Exp(sequencingExp->exp1); 
        myTranslationAndType resultTwo =
            MySemantic_Exp(sequencingExp->exp2);

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
        return MySemantic_Exp(rests->exp);
    }
}

//////////////////////////////////////////////////////////////////////////////

//  forward declarations
void addLoopVarToScope(mySymbol varName);
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
    myForExp forExp)
{
    MySymbol_BeginScope(MySemantic_getVarAndFuncEnvironment());

    bool isLowRangeInt = isExpInt(forExp->varRangeLow);
    bool isHighRangeInt = isExpInt(forExp->varRangeHigh);
    if (isLowRangeInt && isHighRangeInt)
        addLoopVarToScope(forExp->varName);

    enterLoop();
    bool isBodyNoReturn = isExpNoReturn(forExp->bodyExp);  
    leaveLoop();

    MySymbol_EndScope(MySemantic_getVarAndFuncEnvironment());

    if (isLowRangeInt && isHighRangeInt && isBodyNoReturn)
        return make_MyTranslationAndType(NULL, makeType_NoReturn());
    else
    {
        processForErrors(isLowRangeInt, isHighRangeInt, isBodyNoReturn);
        return SEMANTIC_ERROR;
    }
}

void addLoopVarToScope(mySymbol varName)
{
    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment(); 
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
    myIfThenElseExp ifThenElseExp,
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
    myIfThenElseExp ifThenElseExp)
{
    bool isConditionExpInt = isExpInt(ifThenElseExp->exp1);
    bool isThenClauseLegal = isExpLegal(ifThenElseExp->exp2);
    bool isElseClauseLegal = isExpLegal(ifThenElseExp->exp3);
    
    myType resultType = getThenElseResultType(
        ifThenElseExp,
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
    myIfThenElseExp ifThenElseExp,
    bool isThenClauseLegal, bool isElseClauseLegal)
{
    if (isThenClauseLegal && isElseClauseLegal)
    {
        myType thenClauseType = MySemantic_Exp(ifThenElseExp->exp2)
            ->type;
        myType elseClauseType = MySemantic_Exp(ifThenElseExp->exp3)
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
    myIfThenExp ifThenExp)
{
    bool isIfConditionInt = isExpInt(ifThenExp->exp1);
    bool isThenClauseNoReturn = isExpNoReturn(ifThenExp->exp2);

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
    myComparisonExp comparisonExp);
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
    myComparisonExp comparisonExp)
{
    bool isLeftOperandLegal = isExpLegal(
        comparisonExp->left);
    bool isRightOperandLegal = isExpLegal(
        comparisonExp->right);

    bool isOperandsTypeMatches = false;
    if (isLeftOperandLegal && isRightOperandLegal)
    {
        isOperandsTypeMatches = isTwoComparisonOperandsTypeEqualsOrNil(
            comparisonExp);
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
    myComparisonExp comparisonExp)
{
    myType leftType = getExpActualResultType(
        comparisonExp->left);
    myType rightType = getExpActualResultType(
        comparisonExp->right);

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
    myBooleanOperateExp booleanOperateExp)
{
    bool isLeftOperandInt =
        isExpInt(booleanOperateExp->left);
    bool isRightOperandInt = 
        isExpInt(booleanOperateExp->right);

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
    myAssignmentExp assignmentExp);
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
    myAssignmentExp assignmentExp)
{
    bool isLeftOperandLegal =
        isLValueExpLegal(assignmentExp->lValueExp);
    bool isRightOperandLegal =
        isExpLegal(assignmentExp->exp);

    bool isOperandsTypeMatches = false;
    if (isLeftOperandLegal && isRightOperandLegal)
    {
        isOperandsTypeMatches = isAssignmentOperandsTypeMatchOrNil(
            assignmentExp);
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
    myAssignmentExp assignmentExp)
{
    myType leftOperandType =
        getLValueExpActualResultType(assignmentExp->lValueExp);
    myType rightOperandType =
        getExpActualResultType(assignmentExp->exp);

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
    myShortFormVar shortFormVar)
{
    bool isValueExpLegal = isExpLegal(shortFormVar->exp);

    if (isValueExpLegal)
    {
        myType valueType =
            getExpActualResultType(shortFormVar->exp);
        if (!isTypeNil(valueType) && !isTypeNoReturn(valueType))
        {
            myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
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
    myLongFormVar longFormVar);
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
    myLongFormVar longFormVar)
{
    bool isVariableTypeDefined = isTypeDefined(longFormVar->type);
    bool isValueExpLegal = isExpLegal(longFormVar->exp);

    bool isTypeMatches = false;
    if (isVariableTypeDefined && isValueExpLegal)
    {
        isTypeMatches = isVarDecValueTypeMatchesGivenType(
            longFormVar);
    }

    //    hidden checking for the other two booleans
    if (isTypeMatches)
    {
        myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
        myVarAndFuncEntry varEntry =
            myEnvironment_makeVarEntry(getActualTypeFromName(longFormVar->type));
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
    myLongFormVar longFormVar)
{
    myType valueType = 
        getExpActualResultType(longFormVar->exp);
    myType givenType = 
        getActualTypeFromName(longFormVar->type);

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
bool MySemantic_Dec_Var(myVarDec varDec)
{
    if (varDec == NULL)     return true;

    switch (varDec->kind)
    {
        case ShortFormVar:
            return MySemantic_Dec_Var_ShortForm(
                varDec->u.shortFormVar);
        case LongFormVar:
            return MySemantic_Dec_Var_LongForm(
                varDec->u.longFormVar);
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
    myDecList decs)
{
    return MySemantic_Decs_Recursive(decs);
}

///////////////////////////////////////////////////////////////////////////////

//  forwards
bool areExpressionsLegal(
    myExpList exps);
myType getLastExpResultType(
    myExpList exps);
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
    myLetExp letExp)
{
    MySymbol_BeginScope(MySemantic_getVarAndFuncEnvironment());
    MySymbol_BeginScope(MySemantic_getTypeEnvironment());

    bool areDeclarationsLegal =
        MySemantic_Decs_Recursive(letExp->decList);
    bool isBodyLegal =
        areExpressionsLegal(letExp->expList);

    
    myTranslationAndType result;
    if (areDeclarationsLegal && isBodyLegal)
        result = make_MyTranslationAndType(NULL,
            getLastExpResultType(letExp->expList));
    else
    {
        processLetErrors(areDeclarationsLegal, isBodyLegal);
        result = SEMANTIC_ERROR;
    }

    MySymbol_EndScope(MySemantic_getVarAndFuncEnvironment());
    MySymbol_EndScope(MySemantic_getTypeEnvironment());
    return result;
}

bool areExpressionsLegal(
    myExpList exps)
{
    while (exps)
    {
        if (MySemantic_Exp(exps->exp)
            == SEMANTIC_ERROR)
            return false;
        exps = exps->next;
    }
    return true;
}

myType getLastExpResultType(
    myExpList exps)
{
    if (exps == NULL)   return makeType_NoReturn();

    while (exps->next)
        exps = exps->next;
    return getExpActualResultType(exps->exp);
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
    myWhileExp whileExp)
{
    bool isConditionInt =
        isExpInt(whileExp->whileExp);

    enterLoop();
    bool isBodyNoReturn =
        isExpNoReturn(whileExp->bodyExp);
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
    myNegationExp negationExp)
{
    bool isFollowingExpInt = isExpInt(negationExp->exp);
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
    myBreakExp breakExp)
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
    myExp exp)
{
    switch (exp->kind)
    {
        case LValueExp:         
            return MySemantic_LValueExp(exp->u.lValueExp);
        case FunctionCallExp:
            return MySemantic_FunctionCallExp(exp->u.functionCallExp);
        case NilExp:
            return MySemantic_NilExp(exp->u.nilExp);
        case IntegerLiteralExp:
            return MySemantic_IntegerLiteralExp(exp->u.integerLiteralExp);
        case StringLiteralExp:
            return MySemantic_StringLiteralExp(exp->u.stringLiteralExp);
        case ArrayCreationExp:
            return MySemantic_ArrayCreationExp(exp->u.arrayCreationExp);
        case RecordCreationExp:
            return MySemantic_RecordCreationExp(exp->u.recordCreationExp);
        case ArithmeticExp:
            return MySemantic_ArithmeticExp(exp->u.arithmeticExp);
        case ParenthesesExp:
            return MySemantic_ParenthesesExp(exp->u.parenthesesExp);
        case NoValueExp:
            return MySemantic_NoValueExp(exp->u.noValueExp);
        case SequencingExp:
            return MySemantic_SequencingExp(exp->u.sequencingExp);
        case ForExp:
            return MySemantic_ForExp(exp->u.forExp);
        case IfThenElseExp:
            return MySemantic_IfThenElseExp(exp->u.ifThenElseExp);
        case IfThenExp:
            return MySemantic_IfThenExp(exp->u.ifThenExp);
        case ComparisonExp:
            return MySemantic_ComparisonExp(exp->u.comparisonExp);
        case BooleanOperateExp:
            return MySemantic_BooleanOperateExp(exp->u.booleanOperateExp);
        case AssignmentExp:
            return MySemantic_AssignmentExp(exp->u.assignmentExp);
        case LetExp:
            return MySemantic_LetExp(exp->u.letExp);
        case WhileExp:
            return MySemantic_WhileExp(exp->u.whileExp);
        case NegationExp:
            return MySemantic_NegationExp(exp->u.negationExp);
        case BreakExp:
            return MySemantic_BreakExp(exp->u.breakExp);

        default:            assert(false);
    }
}


/////////////////////////////////////////////////////////////////////////////////////