#include "mySemantic.h"
#include "myEnvironment.h"
#include "myError.h"
#include "abstractSyntaxMaker.h"
#include "typeMaker.h"

#include "typeChecker.h"
#include "breakChecker.h"
#include "forChecker.h"

#include "myEscape.h"

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
//  in myTranslate.c
Trans_myAccess getVarAccessFromName(mySymbol varName);

myTranslationAndType MySemantic_Exp_(myExp exp);
myType getExpActualResultType(myExp exp);
static myType getActualType(myType type);
myType getActualTypeFromName(mySymbol typeName);
myTranslationAndType MySemantic_LValueExp(myLValueExp lValueExp);

////////////////////////////////////////////////////////////////////////
//
//  Exp Checkers

bool isExpOneTypeOrIllegal(
    myExp exp, enum TypeKind kind, myTranslationAndType* resultPtr)
{
    assert (exp);
    myTranslationAndType result = MySemantic_Exp_(exp);
    
    if (result == SEMANTIC_ERROR)
        return false;
    else 
        *resultPtr = result;

    myType expActualType = getActualType(result->type);
    switch (kind)
    {
        case TypeInt:       return isTypeInt(expActualType);
        case TypeNoReturn:  return isTypeNoReturn(expActualType);
        case TypeArray:     return isTypeArray(expActualType);
        default:            assert(false);
    }
}

bool isExpLegalWithResult(myExp exp, myTranslationAndType* resultPtr)
{
    *resultPtr = MySemantic_Exp_(exp);
        
    if (*resultPtr == SEMANTIC_ERROR)   return false;
    else                                return true;
}

bool isExpIntWithResult(myExp exp, myTranslationAndType* resultPtr)
{
    return isExpOneTypeOrIllegal(exp, TypeInt, resultPtr);
}

bool isExpNoReturnWithTrans(myExp exp, IR_myExp* expResultPtr)
{
    myTranslationAndType temp;
    bool ret = isExpOneTypeOrIllegal(exp, TypeNoReturn, &temp);

    *expResultPtr = temp->translation;
    return ret;
}

bool isExpIntWithTrans(myExp exp, IR_myExp* expResult)
{
    myTranslationAndType temp;
    bool ret = isExpOneTypeOrIllegal(exp, TypeInt, &temp);

    *expResult = temp->translation;
    return ret;
}

bool isExpLegalWithTrans(myExp exp, IR_myExp* expResult)
{
    assert (exp);

    myTranslationAndType result = MySemantic_Exp_(exp);
        
    if (result == SEMANTIC_ERROR)
        return false;
    else
    {
        *expResult = result->translation;
        return true;
    }
}

static bool isExpThisArrayElementWithTrans(
    myExp exp, mySymbol arrayTypeName, IR_myExp* result)
{
    assert (exp && arrayTypeName);
    myTranslationAndType ret = MySemantic_Exp_(exp);

    myType arrayElementType = getActualType(
        getActualTypeFromName(arrayTypeName)
        ->u.typeArray->type);
    myType initialExpType = getActualType(ret->type); 
    if (isTypeEqual(arrayElementType, initialExpType))
    {
        *result = ret->translation;
        return true;
    }
    else
        return false;
}

myType getExpActualResultTypeWithTrans(myExp exp, IR_myExp* expResult)
{
    assert (exp);

    myTranslationAndType result = MySemantic_Exp_(exp);
    assert (result != SEMANTIC_ERROR);
    *expResult = result->translation;

    return getActualType(result->type);
}

//////////////////////////////////////

//  DO:
//      analyze if this expression fits its semantic
bool isExpLegal(myExp exp)
{
    IR_myExp dummy;
    return isExpLegalWithTrans(exp, &dummy);
}

static bool isExpInt(myExp exp)
{
    IR_myExp dummy;
    return isExpIntWithTrans(exp, &dummy);
}

bool isExpNoReturn(myExp exp)
{
    IR_myExp dummy;
    return isExpNoReturnWithTrans(exp, &dummy);
}

//  NOTE:   array type named arrayTypeName must be valid. 
static bool isExpThisArrayElement(
    myExp exp, mySymbol arrayTypeName)
{
    /*
    assert (exp && arrayTypeName);

    myType arrayElementType = getActualType(
        getActualTypeFromName(arrayTypeName)
        ->u.typeArray->type);
    myType initialExpType = getActualType(
        MySemantic_Exp_(exp)->type); 
    return isTypeEqual(arrayElementType, initialExpType);*/
    IR_myExp dummy;
    return isExpThisArrayElementWithTrans(exp, arrayTypeName, &dummy);
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

    myTranslationAndType result = MySemantic_Exp_(exp);
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
    IR_myExp translation, myType type)
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

static myTable g_varAndFuncEnv = NULL;
static myTable g_typeEnv = NULL;

static myTable g_levelTable = NULL;
static void*   g_fakeKey = (void*)23;

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

////////////////////////////////////////////////

void MySemantic_enterNewLevel(Trans_myLevel newLevel)
{
    if (g_levelTable == NULL)
        g_levelTable = MySymbol_MakeNewTable();

    MyTable_Enter_(g_levelTable, g_fakeKey, newLevel);
}

void MySemantic_leaveNewLevel(void)
{
    if (g_levelTable)
    {
        MyTable_Pop_(g_levelTable);
    }
}

Trans_myLevel MySemantic_getCurrentLevel(void)
{
    assert (g_levelTable);
    Trans_myLevel level = MyTable_Look_(g_levelTable, g_fakeKey);
    assert (level); 

    return level;
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


/**
 *  @brief      Do semantic analysis on LValueExp.
 *      FORM:
 *          ->  id
 *          ->  lvalue.id
 *          ->  lvalue[exp]
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
    myTranslationAndType result;
    switch (lValueExp->kind)
    {
        case SimpleVar:
            result = MySemantic_LValueExp_SimpleVar(lValueExp);
            break;
        case RecordField:
            result = MySemantic_LValueExp_RecordField(lValueExp);
            break;
        case ArraySubscript:
            result = MySemantic_LValueExp_ArraySubscript(lValueExp);
            break;
    }

    return result;
}

/**
 *  @brief  Do semantic analysis on SimpleVar, one kind of LValueExp.
 *
 *          FORM:
 *              id
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
    
    return make_MyTranslationAndType(
        Trans_LValueExp_SimpleVar(lValueExp),
        getVariableType(lValueExp->id));
}

///////////////////////////////////////////////

//  forward declarations
bool isVariableRecordType(mySymbol variableName);
myType typeContainsLValueAux(
    myType type, myLValueAux aux, IR_myExp* addressResult);
static myType arrayContainsLValueAux(
    myType type, myLValueAux aux, IR_myExp* addressResult);
static myType analyzeRecordFields(
    myLValueExp lValueExp, IR_myExp* addressResult);
static myType recordContainsLValueAux(
    myType type, myLValueAux aux, IR_myExp* addressResult);
void processRecordFieldErrors(
    bool isVarDeclared, bool isVarAnRecord, mySymbol recordVarName);
myTranslationAndType doFieldAnalyzeAndTranlation(myLValueExp lValueExp);

/**
 *  @brief  Do semantic analysis on RecordField, one kind of LValueExp.
 *
 *          FORM:
 *              lvalue.id
 *          DO:
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
        return doFieldAnalyzeAndTranlation(lValueExp);
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

myTranslationAndType doFieldAnalyzeAndTranlation(myLValueExp lValueExp)
{
    IR_myExp addressResult = NULL;
    myType returnType = analyzeRecordFields(lValueExp, &addressResult);

    if (returnType == SEMANTIC_ERROR)
        return SEMANTIC_ERROR;
        
    IR_myStatement stateResult;
    IR_myExp valueResult;
    IR_divideExp(addressResult, &stateResult, &valueResult);

    return make_MyTranslationAndType(
        IR_makeESeq(stateResult, IR_makeMem(valueResult)),
        returnType);
}




static myLValueAux wrapExpToAux(myLValueExp lValueExp)
{
    myRecordFieldAux fieldAux = lValueExp->u.recordFieldAux;
    return makeMyLValueAux(
        fieldAux->id, NULL, fieldAux->next);
}

//  DO:
//      check if field names in `lValueExp` can be found in record type.
//  RETURN:
//      return NULL if analyze fails.
//      return type of the last field if succeed.
//  REMARK:
//      One record field can bw followed by array subscript, so wrap aux and
//      delegate it to an dispatcher.
static myType analyzeRecordFields(
    myLValueExp lValueExp, IR_myExp* addressResult)
{
    assert (lValueExp);

    myType typeOfRecord = makeType_Record(
        getVariableType(lValueExp->id)
            ->u.typeRecord->fieldList); 
    myLValueAux aux = wrapExpToAux(lValueExp);

    *addressResult = Trans_LValueExp_GetRecordPtr(lValueExp);
    return typeContainsLValueAux(typeOfRecord, aux, addressResult);
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
    myType type, myLValueAux aux, IR_myExp* addressResult)
{
    switch (type->kind)
    {
        case TypeArray:
            return arrayContainsLValueAux(type, aux, addressResult);
        case TypeRecord:
            return recordContainsLValueAux(type, aux, addressResult);
        default:    //  never got here
            assert(false);
    }
}

static void tryToFindFieldAndTranslate(
    myTypeFieldList fields, myLValueAux aux,
    myType* typeReturnPtr, IR_myExp resultRegisterExp,
    IR_myExp* addressResult)
{
    int count = 0;
    while (fields)  //  try to find field
    {
        if (MySymbol_IsSymbolEqual(fields->head->name, aux->id))
        {
            *typeReturnPtr = fields->head->type;

            //  do not change result register representations
            IR_myStatement oneState =  IR_makeExp(IR_makeBinOperation(IR_Plus,
                resultRegisterExp, IR_makeConst(-count * Frame_wordSize)));
            (*addressResult)->u.eseq.statement = IR_makeSeq(
                (*addressResult)->u.eseq.statement, oneState);
            break;
        }

        count++;
        fields = fields->tails;
    }
}

static myType processsNextFieldOrReturn(
    myLValueAux aux, IR_myExp resultRegisterExp,
    IR_myExp* addressResult, myType typeReturn)
{
    if (aux->next)
	{
		//  must be of array or record type, fetch real address
		//  from this address value.
        /*
		IR_myExp newResultRegisterExp = IR_makeTemp(Temp_newTemp()); 
		IR_myStatement oneState = IR_makeMove(
            newResultRegisterExp, IR_makeMem(resultRegisterExp));
		(*addressResult)->u.eseq.statement = IR_makeSeq(
            (*addressResult)->u.eseq.statement, oneState);
		(*addressResult)->u.eseq.exp = newResultRegisterExp;*/
        getAddressContent(addressResult);

		return typeContainsLValueAux(
            typeReturn, aux->next, addressResult);
	}
	else
	    return typeReturn;
}

//  check whether field name in `aux` can be found inside record type.
//  return NULL for checking error.
static myType recordContainsLValueAux(
    myType type, myLValueAux aux, IR_myExp* addressResult)
{
    assert (isTypeRecord(type));
    assert (getLValueKind(aux) == RecordField);

    myTypeFieldList fields = type->u.typeRecord->fieldList;
    myType typeReturn = NULL;

    IR_myExp resultRegisterExp = (*addressResult)->u.eseq.exp;
    assert (resultRegisterExp->kind == IR_Temp);

    tryToFindFieldAndTranslate(
        fields, aux, &typeReturn, resultRegisterExp, addressResult);

    if (typeReturn == NULL)   //  field not found
    {
        MyError_pushErrorCodeWithMsg(ERROR_RECORD_FIELD_NOT_FOUND,
            MySymbol_GetName(aux->id));
        return SEMANTIC_ERROR;
     }
    else    //  field found
    {
        return processsNextFieldOrReturn(
            aux, resultRegisterExp, addressResult, typeReturn);
    }
}


static void calcArraySubscriptAddr(
    IR_myExp* addressResult, IR_myExp subscriptResult)
{
    IR_myExp resultRegisterExp = (*addressResult)->u.eseq.exp;
    assert (resultRegisterExp->kind == IR_Temp);

    IR_myStatement subscriptState;
    IR_myExp subscriptValue;
    IR_divideExp(subscriptResult, &subscriptState, &subscriptValue);

    //  combine offset statement
    IR_myStatement subscriptOffsetState = IR_makeExp(
        IR_makeBinOperation(IR_Multiply,
            subscriptValue, IR_makeConst(-Frame_wordSize)));
    (*addressResult)->u.eseq.statement = IR_makeSeq(
        (*addressResult)->u.eseq.statement,
        IR_makeSeq(subscriptState, subscriptOffsetState));
    
    IR_myStatement addrState = IR_makeExp(IR_makeBinOperation(IR_Plus,
        resultRegisterExp, subscriptValue));
    (*addressResult)->u.eseq.statement = IR_makeSeq(
        (*addressResult)->u.eseq.statement, addrState);
    //  do not change result register representations.
}

//  check whether the type of array subscript is Integer.
//  return NULL for checking error.
static myType arrayContainsLValueAux(
    myType type, myLValueAux aux, IR_myExp* addressResult)
{
    assert (isTypeArray(type));
    assert (getLValueKind(aux) == ArraySubscript);

    //  check subscript's expression is of Integer type
    myExp subscriptExp = aux->exp;
    IR_myExp subscriptResult = NULL;
    if (!isExpIntWithTrans(subscriptExp, &subscriptResult))
    {
        MyError_pushErrorCode(ERROR_SUBSCRIPT_NOT_INT);
        return NULL;
    }

/*
    IR_myExp resultRegisterExp = (*addressResult)->u.eseq.exp;
    assert (resultRegisterExp->kind == IR_Temp);

    IR_myStatement subscriptState;
    IR_myExp subscriptValue;
    IR_divideExp(subscriptResult, &subscriptState, &subscriptValue);

    //  do not change result register representations. 
    (*addressResult)->u.eseq.statement = IR_makeSeq(
        (*addressResult)->u.eseq.statement, subscriptState);
    IR_myStatement addrState = IR_makeExp(IR_makeBinOperation(IR_Plus,
        resultRegisterExp, subscriptValue));
    (*addressResult)->u.eseq.statement = IR_makeSeq(
        (*addressResult)->u.eseq.statement, addrState);
*/
    calcArraySubscriptAddr(addressResult, subscriptResult);

    if (aux->next)
    {
        //  next must be record or array type, whose address is got by
        //  fetching its content
        *addressResult = IR_makeMem(*addressResult);
        return typeContainsLValueAux(
                type->u.typeArray->type, aux->next, addressResult);
    }
    else
        return type->u.typeArray->type;
}

////////////////////////////////////////////////

//  forward declarations
bool isVariableArrayType(mySymbol variableName);
myTranslationAndType analyzeRecursiveArraySubscripts(
    myLValueExp lValueExp, IR_myExp currentSubscriptResult);
void processArraySubscriptErrors(
    bool isArrayVariableDeclared, bool isVariableArrayType,
    bool isSubscriptExpInt, myLValueExp lValueExp);


/**
 *  @brief  Do semantic analysis on ArraySubscript, one kind of LValueExp.
 *
 *          FORM:
 *              lvalue[exp]
 *          DO:
 *              1.check whether the array variable is declared.
 *              2.check whether the declared variable is an array.
 *              3.check whether the subscript expression is of Integer type.
 *              4.return the last array element's type.
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

    IR_myExp subscriptResult = NULL;
    myExp subscriptExp = lValueExp->u.arraySubscriptAux->exp;
    bool isSubscriptInt = isExpIntWithTrans(subscriptExp, &subscriptResult);

    myTranslationAndType result;
    if (isArrayVariableDeclared && isVariableAnArray && isSubscriptInt)
        return analyzeRecursiveArraySubscripts(lValueExp, subscriptResult);
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


IR_myExp calculateFirstSubscriptAddress(
    IR_myExp currentSubscriptResult, myLValueExp lValueExp)
{
	IR_myStatement subscriptState;
	IR_myExp subscriptValue;
	IR_divideExp(currentSubscriptResult, &subscriptState, &subscriptValue);

	//  calculate offset
	subscriptState = IR_makeSeq(
        subscriptState,
        IR_makeExp(IR_makeBinOperation(IR_Multiply,
            subscriptValue, IR_makeConst(-Frame_wordSize))));

	IR_myExp arrayPtrExp = Trans_LValueExp_GetArrayPtr(lValueExp);
	assert (arrayPtrExp->kind == IR_ESeq);
	assert (arrayPtrExp->u.eseq.exp->kind == IR_Temp);

	IR_myStatement combinedState = IR_makeSeq(
        arrayPtrExp->u.eseq.statement, subscriptState);
	IR_myStatement addrCalc = IR_makeExp(IR_makeBinOperation(IR_Plus,
        arrayPtrExp->u.eseq.exp, subscriptValue)); 
	combinedState = IR_makeSeq(combinedState, addrCalc);
    //  do not change reuslt register representations.

	return IR_makeESeq(combinedState, arrayPtrExp->u.eseq.exp);
}

//  DO:
//      check if the subscript expression inside `lValueExp` is of Integer type.
//  RETURN:
//      SEMANTIC_ERROR for checking error;
//      A myTranslationAndType variable for success.
myTranslationAndType analyzeRecursiveArraySubscripts(
    myLValueExp lValueExp, IR_myExp currentSubscriptResult)
{
    assert (lValueExp);

    myTranslationAndType result =
        make_MyTranslationAndType(NULL, NULL);
    
    //  caller make sures its a variable entry!
    myVarAndFuncEntry arrayEntry = getVarOrFuncFromName(lValueExp->id);
    assert (arrayEntry != NULL);


    //  return this array type
    result->type = MyEnvironment_getVarType(arrayEntry)
        ->u.typeArray->type;
    result->translation =
        calculateFirstSubscriptAddress(currentSubscriptResult, lValueExp);

    if (lValueExp->u.arraySubscriptAux->next)
    {
        myType elementType = MyEnvironment_getVarType(arrayEntry)
            ->u.typeArray->type;

        //  next must be of record or array type, get its address from current
        //  address's value. 
        getAddressContent(&result->translation);
        //  recursive checking, add or rewrite result if necessary
        myType typeReturn = typeContainsLValueAux(
            elementType,
            lValueExp->u.arraySubscriptAux->next, &result->translation);

        if (typeReturn == NULL)
        {
            MyError_pushErrorCode(ERROR_SUBSCRIPT_ERROR);
            return SEMANTIC_ERROR;
        }
        else                        result->type = typeReturn;
    }
    
    getAddressContent(&result->translation);
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
    myTranslationAndType result;
    switch (functionCallExp->kind)
    {
        case NoParamFunctionCallExp:
            result = MySemantic_FunctionCallExp_NoParam(
                functionCallExp->u.noParamFunctionCallExp);
            break;
        case ParamFunctionCallExp:
            result = MySemantic_FunctionCallExp_Param(
                functionCallExp->u.paramFunctionCallExp);
            break;
    }

    return result;
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

    myLabel funcLabel = MyEnvironment_getFuncLabel(
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(),
            functionName));
    assert (funcLabel != NULL);
    //  include static link as param
    IR_myExp funcCallExp = IR_makeCall(
        IR_makeName(funcLabel),
        IR_makeExpList(IR_makeTemp(Frame_FP()), NULL)); 
    return make_MyTranslationAndType(
        funcCallExp, getFunctionReturnType(functionName));
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

    //  include static link as param
    IR_myExpList exps = IR_makeExpList(IR_makeTemp(Frame_FP()), NULL);
    IR_myExpList* expsPtr = &exps->tails;
    while (formalVariables && formalTypes)
    {
        myTranslationAndType realParamResult = 
            MySemantic_Exp_(formalVariables->exp);
        if (realParamResult == SEMANTIC_ERROR)   return SEMANTIC_ERROR;

        //  check if types matches
        if (!isFormalTypeMatchesRealTypeOrNil(
                formalTypes->head, realParamResult->type))
        {
            break;
        }

        //  translation is correct only after checking
        (*expsPtr) = IR_makeExpList(realParamResult->translation, NULL);
        expsPtr = &((*expsPtr)->tails);

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
        myLabel funcLabel = MyEnvironment_getFuncLabel(
        MyEnvironment_getVarOrFuncFromName(
            MySemantic_getVarAndFuncEnvironment(),
            functioName));
        IR_myExp funcCallExp = IR_makeCall(IR_makeName(funcLabel), exps);
        return make_MyTranslationAndType(
            funcCallExp, getFunctionReturnType(functioName));
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

/**
 *  @brief  Semantic analysis of NilExp.
 *
 *          FORM:
 *              nil
 *          DO:
 *              1.return myTypeNil Type.
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
    //  treat nil as zero. When deferencing it, a segment fault will be thrown. 
    return make_MyTranslationAndType(IR_makeConst(0), makeType_Nil());  
}

////////////////////////////////////////////////////////////////////////////////////

/**
 *  @brief  Semantic analysis of IntegerLiteralExp.
 *
 *          FORM:
 *              integer literals
 *          DO:
 *              1.return myTypeInt type.
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
    return make_MyTranslationAndType(
        Trans_IntegerLiteralExp(integerLiteralExp), makeType_Int());
}

///////////////////////////////////////////////////////////////////////////////////

/**
 *  @brief  Semantic analysis of StringLiteralExp.
 *
 *          FORM:
 *              string literals
 *          DO:
 *              1.return myTypeString type.
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
    IR_myExp strLabelExp = IR_makeName(Temp_newLabel());
    Trans_string(strLabelExp, stringLiteralExp->str);

    return make_MyTranslationAndType(strLabelExp, makeType_String());  
}

/////////////////////////////////////////////////////////////////////////////////

//  forward declarations
bool isTypeDefinedAsArray(
    mySymbol arrayTypeName);
bool isValueTypeMatchesArrayElementOrNil(
    myExp initialValue, mySymbol arrayTypeName, IR_myExp* initValueResult);
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

    IR_myExp subscriptResult = NULL;
    bool isSubscriptAnIntExp = isExpIntWithTrans(
        arrayCreationExp->length, &subscriptResult);

    IR_myExp initValueResult = NULL;
    bool isInitialExpThisArrayElementType = false;
    if (isArrayTypeDefiend)
        isInitialExpThisArrayElementType = isValueTypeMatchesArrayElementOrNil(
            arrayCreationExp->initial, arrayTypeName, &initValueResult);

    if (isArrayTypeDefiend && isSubscriptAnIntExp && isInitialExpThisArrayElementType)
    {
        IR_myExp tranResult = Trans_arrayCreation(subscriptResult, initValueResult);
        return make_MyTranslationAndType(tranResult,getActualTypeFromName(arrayTypeName));
    }
    else
    {
        processArrayCreationErrors(isArrayTypeDefiend, isSubscriptAnIntExp,
            isInitialExpThisArrayElementType, arrayTypeName);
        return SEMANTIC_ERROR;
    }
}

bool isValueTypeMatchesArrayElementOrNil(
    myExp initialValue, mySymbol arrayTypeName, IR_myExp* initValueResult)
{
    myType arrayElementType = getActualType(
        getActualTypeFromName(arrayTypeName)
            ->u.typeArray->type);
    myType valueType = getActualType(
        getExpActualResultTypeWithTrans(initialValue, initValueResult));

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
    myTranslationAndType result;
    switch (recordCreationExp->kind)
    {
        case NoFieldRecordCreationExp:
            result = MySemantic_RecordCreationExp_NoField(
                recordCreationExp->u.noFieldRecordCreationExp);
            break;
        case FieldRecordCreationExp:
            result = MySemantic_RecordCreationExp_Field(
                recordCreationExp->u.fieldRecordCreationExp);
            break;
    }

    return result;
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
        return make_MyTranslationAndType(
            Trans_noFieldRecordCreation(),
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
bool ifFieldNamesAndTypesMatchesWithTrans(
    myFieldRecordCreationExp fieldRecordCreationExp, IR_myExp* tranResult);
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

    IR_myExp tranResult = NULL;
    bool isCreationFieldsMatchThisRecordType = isRecordTypeDefined ?
            ifFieldNamesAndTypesMatchesWithTrans(
                fieldRecordCreationExp, &tranResult) :
            false;
    
    if (isRecordTypeDefined && isCreationFieldsMatchThisRecordType)
        return make_MyTranslationAndType(tranResult,
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
    myTypeFieldList typeFields, myRecordFieldList givenRecordFields,
    IR_myExp* fieldValueTrans);
int getTypeFieldCount(myTypeFieldList typeFields);

static void allocateRecordSpaceTran(myTypeFieldList typeFields,
    IR_myStatement* stateResult, IR_myExp* addressRegResult)
{
    IR_myExp result = Frame_externalCall("malloc",
        IR_makeExpList(
            IR_makeConst(getTypeFieldCount(typeFields) * 4),
            NULL));
    *stateResult = result->u.eseq.statement;
    *addressRegResult = result->u.eseq.exp;
}

static IR_myExp saveAddrToResultReg(
    IR_myStatement* stateTranResultPtr, IR_myExp memoryAddrReg)
{
    IR_myExp resultReg = IR_makeTemp(Temp_newTemp());
    (*stateTranResultPtr) = IR_makeSeq(
        (*stateTranResultPtr), IR_makeMove(resultReg, memoryAddrReg));
    return resultReg;
}

static void assignOneFieldTran(
    IR_myStatement* stateTranResultPtr,IR_myExp oneFieldValue,
    IR_myExp memoryAddrReg)
{
    //  assign one field value
    IR_myStatement fieldState;
    IR_myExp fieldValue;
    IR_divideExp(oneFieldValue, &fieldState, &fieldValue);

    (*stateTranResultPtr) = IR_makeSeq((*stateTranResultPtr), fieldState);
    (*stateTranResultPtr) = IR_makeSeq(
        (*stateTranResultPtr),
        IR_makeMove(IR_makeMem(memoryAddrReg), fieldValue));
}

static void moveToNextMemoryAddr(
    IR_myStatement* stateTranResultPtr, IR_myExp memoryAddrReg)
{
    (*stateTranResultPtr) = IR_makeSeq(
        (*stateTranResultPtr),
        IR_makeExp(IR_makeBinOperation(IR_Plus,
            memoryAddrReg, IR_makeConst(-Frame_wordSize))));
}

//  NOTE:   must be called after isTypeNotRecordAndSetError.
bool ifFieldNamesAndTypesMatchesWithTrans(
    myFieldRecordCreationExp fieldRecordCreationExp, IR_myExp* translationResult)
{
    mySymbol recordTypeName = fieldRecordCreationExp->typeName;
    myRecordFieldList givenRecordFields = fieldRecordCreationExp->fieldList;
    myTypeFieldList typeFields =
        getActualTypeFromName(recordTypeName)->u.typeRecord->fieldList;

    IR_myStatement stateTranResult;
    IR_myExp memoryAddrReg;
    allocateRecordSpaceTran(typeFields, &stateTranResult, &memoryAddrReg);
    IR_myExp resultReg = saveAddrToResultReg(&stateTranResult, memoryAddrReg);

    while (givenRecordFields && typeFields)
    {
        IR_myExp oneFieldValue = NULL;
        if (!isOneFieldMatches(typeFields, givenRecordFields, &oneFieldValue))
            return false;

        assignOneFieldTran(&stateTranResult, oneFieldValue, memoryAddrReg);
        moveToNextMemoryAddr(&stateTranResult, memoryAddrReg);

        givenRecordFields = givenRecordFields->next;
        typeFields = typeFields->tails;
    }
    *translationResult = IR_makeESeq(stateTranResult, resultReg);

    //  field number not match!
    if (givenRecordFields || typeFields)    return false;
    else                                    return true;
}

int getTypeFieldCount(myTypeFieldList typeFields)
{
    int count = 0;
    while (typeFields)
        count++, typeFields = typeFields->tails;
    return count;
}

bool isOneFieldMatches(
    myTypeFieldList typeFields, myRecordFieldList givenRecordFields,
    IR_myExp* fieldValueTrans)
{
    myTranslationAndType ret =
        MySemantic_Exp_(givenRecordFields->field->varValue);

    myType fieldType = typeFields->head->type;
    myType valueType = ret->type;
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

    *fieldValueTrans = ret->translation;
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
static IR_BinOperator convertArithmeticOperatorFromASTToIR(enum myArithmeticOp op);
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
    IR_myExp leftOperandTran = NULL;
    bool isLeftExpInt = isExpIntWithTrans(arithmeticExp->left, &leftOperandTran);

    IR_myExp rightOperandTran = NULL;
    bool isRightExpInt = isExpIntWithTrans(arithmeticExp->right, &rightOperandTran);

    if (isLeftExpInt && isRightExpInt)
    {
        IR_myExp tranResult = Trans_arithmetic(
            leftOperandTran, rightOperandTran,
            convertArithmeticOperatorFromASTToIR(arithmeticExp->op));
        return make_MyTranslationAndType(tranResult, makeType_Int());
    }
    else
    {
        processArithmeticErrors(isLeftExpInt, isRightExpInt);
        return SEMANTIC_ERROR;
    }
}

static IR_BinOperator convertArithmeticOperatorFromASTToIR(enum myArithmeticOp op)
{
    switch (op)
    {
        case opPlus:        return IR_Plus;
        case opMinus:       return IR_Minus;
        case opMultiply:    return IR_Multiply;
        case opDivide:      return IR_Divide;
        default:    assert (false);
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
    return MySemantic_Exp_(parenthesesExp->exp);
}

///////////////////////////////////////////////////////////////////////////////


/**
 *  @brief  Semantic analysis of NoValueExp.
 *
 *          FORM:
 *              ()
 *          DO:
 *              1.return myTypeNoReturn type.
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
    //  no checking and translation needed here
    return make_MyTranslationAndType(NULL, makeType_NoReturn());
}

////////////////////////////////////////////////////////////////////////////

static bool analyzeAndTranslateFirstTwoExps(
    myExp exp1, myExp exp2,
    IR_myStatement* stateResultPtr, IR_myExp* valueResultPtr,
    myType* returnTypePtr);
static void translateFirstTwoExps(
    IR_myExp firstResult, IR_myExp secondResult,
    IR_myStatement* stateResultPtr, IR_myExp* valueResultPtr);
static bool analyzeAndTranslateFolllowingExps(
    mySequencingExp sequencingExp,
    IR_myStatement* stateResultPtr, IR_myExp* valueResultPtr,
    myType* returnTypePtr);

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
    IR_myStatement stateResult = NULL;
    IR_myExp valueResult = NULL;
    myType returnType = NULL;

    if (!analyzeAndTranslateFirstTwoExps(
            sequencingExp->exp1, sequencingExp->exp2,
            &stateResult, &valueResult, &returnType))
        return SEMANTIC_ERROR;

    if (sequencingExp->nextList == NULL)
    {
        return make_MyTranslationAndType(
            IR_makeESeq(stateResult, valueResult),
            returnType);
    }

    if (!analyzeAndTranslateFolllowingExps(
        sequencingExp, &stateResult, &valueResult, &returnType))
        return SEMANTIC_ERROR;

    return make_MyTranslationAndType(
            IR_makeESeq(stateResult, valueResult),
            returnType);
}

static bool analyzeAndTranslateFirstTwoExps(
    myExp exp1, myExp exp2,
    IR_myStatement* stateResultPtr, IR_myExp* valueResultPtr,
    myType* returnTypePtr)
{
    myTranslationAndType resultOne = MySemantic_Exp_(exp1); 
    myTranslationAndType resultTwo = MySemantic_Exp_(exp2);

    if (resultOne == SEMANTIC_ERROR || resultTwo == SEMANTIC_ERROR)
        return false;

    translateFirstTwoExps(
        resultOne->translation, resultTwo->translation,
        stateResultPtr, valueResultPtr);
    *returnTypePtr = resultTwo->type;
    return true;
}

static void translateFirstTwoExps(
    IR_myExp firstResult, IR_myExp secondResult,
    IR_myStatement* stateResultPtr, IR_myExp* valueResultPtr)
{
    IR_myStatement firstState;
    IR_myExp firstValue;
    IR_divideExp(firstResult, &firstState, &firstValue);

    IR_myStatement secondState;
    IR_myExp secondValue;
    IR_divideExp(secondResult, &secondState, &secondValue);

    *stateResultPtr = IR_makeSeq(firstState, secondState);
    *valueResultPtr = secondValue;
} 

static bool analyzeAndTranslateFolllowingExps(
    mySequencingExp sequencingExp,
    IR_myStatement* stateResultPtr, IR_myExp* valueResultPtr,
    myType* returnTypePtr)
{
    myTranslationAndType oneResult = NULL;
    myExpList rests = sequencingExp->nextList;

    while (rests)
    {
        //  todo: test semantic check
        //  although isn't the result type, semantic analysis needed here
        oneResult = MySemantic_Exp_(rests->exp);
        if (oneResult == SEMANTIC_ERROR)
            return false;
        
        IR_myStatement oneState;
        IR_divideExp(oneResult->translation, &oneState, valueResultPtr);
        (*stateResultPtr) = IR_makeSeq((*stateResultPtr), oneState);

        rests = rests->next;
    }

    *returnTypePtr = oneResult->type;
    return true;
}

//////////////////////////////////////////////////////////////////////////////

//  forward declarations
void addLoopVarToScope(mySymbol varName);
void processForErrors(bool isLowRangeInt, bool isHighRangeInt, bool isBodyNoReturn);

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
myTranslationAndType MySemantic_ForExp(myForExp forExp)
{
    MySymbol_BeginScope(MySemantic_getVarAndFuncEnvironment());

    IR_myExp lowRangeResult = NULL;
    bool isLowRangeInt = 
        isExpIntWithTrans(forExp->varRangeLow, &lowRangeResult);

    IR_myExp highRangeResult = NULL;
    bool isHighRangeInt = 
        isExpIntWithTrans(forExp->varRangeHigh, &highRangeResult);

    if (isLowRangeInt && isHighRangeInt)
        addLoopVarToScope(forExp->varName);
    Trans_myAccess loopVarAccess = getVarAccessFromName(forExp->varName);

    enterLoop();    //  for break checking
    enterForLoop(forExp->varName);  //  for loop-var checking
    IR_myExp bodyResult = NULL;
    bool isBodyNoReturn =
        isExpNoReturnWithTrans(forExp->bodyExp, &bodyResult);
    leaveForLoop();  
    leaveLoop();

    MySymbol_EndScope(MySemantic_getVarAndFuncEnvironment());

    if (isLowRangeInt && isHighRangeInt && isBodyNoReturn)
    {
        IR_myExp tranResult =
            Trans_for(lowRangeResult, highRangeResult, loopVarAccess, bodyResult);
        return make_MyTranslationAndType(tranResult, makeType_NoReturn());
    }
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
    
    //  loop-var always escapes
    Trans_myAccess varAccess = Trans_allocateLocal(
        MySemantic_getCurrentLevel(), true);
    myVarAndFuncEntry varEntry = myEnvironment_makeVarEntry(
        varAccess, loopVarType);
    
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
    bool isThenClauseLegal, bool isElseClauseLegal,
    myType thenClauseType, myType elseClauseType);
void processIfThenElseErrors(
    bool isConditionExpInt, bool isThenClauseLegal,
    bool isElseClauseLegal, bool isThenAndElseClauseSameType);


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
myTranslationAndType MySemantic_IfThenElseExp(myIfThenElseExp ifThenElseExp)
{
    myTranslationAndType conditionResult = NULL;
    myTranslationAndType thenClauseResult = NULL;
    myTranslationAndType elseClauseResult = NULL;

    bool isConditionExpInt =
        isExpIntWithResult(ifThenElseExp->exp1, &conditionResult);
    bool isThenClauseLegal =
        isExpLegalWithResult(ifThenElseExp->exp2, &thenClauseResult);
    bool isElseClauseLegal =
        isExpLegalWithResult(ifThenElseExp->exp3, &elseClauseResult);
    
    myType resultType = getThenElseResultType(
        isThenClauseLegal, isElseClauseLegal,
        thenClauseResult->type, elseClauseResult->type);
    bool isThenAndElseClauseSameType = (resultType != NULL);

    if (isConditionExpInt && isThenAndElseClauseSameType)
    {
        IR_myExp ifThenElseTrans = Trans_ifThenElse(
            conditionResult->translation,
            thenClauseResult->translation,
            elseClauseResult->translation,
            !isTypeNoReturn(resultType));
        return make_MyTranslationAndType(ifThenElseTrans, resultType);
    }
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
    bool isThenClauseLegal, bool isElseClauseLegal,
    myType thenClauseType, myType elseClauseType)
{
    if (isThenClauseLegal && isElseClauseLegal)
    {
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
    IR_myExp conditionTrans;
    bool isIfConditionInt =
        isExpIntWithTrans(ifThenExp->exp1, &conditionTrans);

    IR_myExp thenTrans;
    bool isThenClauseNoReturn =
        isExpNoReturnWithTrans(ifThenExp->exp2, &thenTrans);

    if (isIfConditionInt && isThenClauseNoReturn)
    {
        IR_myExp ifThenTrans = Trans_ifThen(conditionTrans, thenTrans);
        return make_MyTranslationAndType(ifThenTrans, makeType_NoReturn());
    }
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
bool isTwoComparisonOperandsTypeEqualsOrNil(myType leftType, myType rightType);
static IR_RelOperator convertComparisonOperatorFromASTToIR(
    enum myComparisonOp op);
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
myTranslationAndType MySemantic_ComparisonExp(myComparisonExp comparisonExp)
{
    myTranslationAndType leftResult;
    bool isLeftOperandLegal =
        isExpLegalWithResult(comparisonExp->left, &leftResult);
    
    myTranslationAndType rightResult;
    bool isRightOperandLegal =
        isExpLegalWithResult(comparisonExp->right, &rightResult);

    bool isOperandsTypeMatches = false;
    if (isLeftOperandLegal && isRightOperandLegal)
    {
        isOperandsTypeMatches = isTwoComparisonOperandsTypeEqualsOrNil(
            leftResult->type, rightResult->type);
    }

    if (isLeftOperandLegal && isRightOperandLegal && isOperandsTypeMatches)
    {
        IR_myExp comparisonTran = Trans_comparison(
            leftResult->translation,
            rightResult->translation,
            convertComparisonOperatorFromASTToIR(comparisonExp->op),
            isTypeString(leftResult->type));
        return make_MyTranslationAndType(comparisonTran, makeType_Int());
    }
    else
    {
        processComparisonErrors(
            isLeftOperandLegal, isRightOperandLegal, isOperandsTypeMatches);
        return SEMANTIC_ERROR;
    }
}

bool isTwoComparisonOperandsTypeEqualsOrNil(myType leftType, myType rightType)
{// todo: inequality comparison cannot only applied to record or array 
    myType leftActualType = getActualType(leftType);
    myType rightActualType = getActualType(rightType);

    if (isTypeNoReturn(leftActualType) || isTypeNoReturn(rightActualType))
        return false;
    else
        return  isTypeEqual(leftActualType, rightActualType) ||
                isOneRecordTypeAnotherNil(leftActualType, rightActualType);
}

static IR_RelOperator convertComparisonOperatorFromASTToIR(
    enum myComparisonOp op)
{
    switch (op)
    {
        case opEqual:               return IR_Equal;
        case opNotEqual:            return IR_NotEqual;
        case opLargerThan:          return IR_GreaterThan;
        case opSmallerThan:         return IR_LessThan;
        case opLargerEqualThan:     return IR_GreaterEqual;
        case opSmallerEqualThan:    return IR_LessEqual;
        default:    assert (false);
    }
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
    bool isLeftOperandLegal, bool isRightOperandLegal,
    bool isOperandsTypeMatches, bool isLoopVarIllegalAssigned,
    mySymbol leftOperandVarName);

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
    mySymbol leftOperandVarName = assignmentExp->lValueExp->id;

    bool isLeftOperandLegal =
        isLValueExpLegal(assignmentExp->lValueExp);
    bool isRightOperandLegal =
        isExpLegal(assignmentExp->exp);

    bool isOperandsTypeMatches = false;
    bool isLoopVarIllegalAssigned = false;
    if (isLeftOperandLegal && isRightOperandLegal)
    {
        isOperandsTypeMatches = isAssignmentOperandsTypeMatchOrNil(
            assignmentExp);
        if (isInsideForLoop())
            isLoopVarIllegalAssigned = isLoopVarAssigned(leftOperandVarName); 
    }

    if (isLeftOperandLegal && isRightOperandLegal &&
        isOperandsTypeMatches && !isLoopVarIllegalAssigned)
        return make_MyTranslationAndType(NULL, makeType_NoReturn());
    else
    {
        processAssignmentErrors(
            isLeftOperandLegal, isRightOperandLegal,
            isOperandsTypeMatches, isLoopVarIllegalAssigned,
            leftOperandVarName);
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
    bool isLeftOperandLegal, bool isRightOperandLegal,
    bool isOperandsTypeMatches, bool isLoopVarIllegalAssigned,
    mySymbol leftOperandVarName)
{
    if (!isLeftOperandLegal)
        MyError_pushErrorCodeWithMsg(ERROR__ASSIGNMENT__OPERAND_NOT_LEGAL,
            "assignment-left-operand");
    if (!isRightOperandLegal)
        MyError_pushErrorCodeWithMsg(ERROR__ASSIGNMENT__OPERAND_NOT_LEGAL,
            "assignment-right-operand");
    if (!isOperandsTypeMatches)
        MyError_pushErrorCode(ERROR__ASSIGNMENT__OPERANDS_TYPE_NOT_MATCH);
    if (isLoopVarIllegalAssigned)
        MyError_pushErrorCodeWithMsg(ERROR__FOR_LOOP_VAR__CAN_NOT_BE_ASSIGNED,
            MySymbol_GetName(leftOperandVarName));
}

///////////////////////////////////////////////////////////////////////////////

//  forwards
void allocateAndAddNewVar_Short(myShortFormVar shortFormVar, myType valueType);

//  FORM:
//      var id := exp
//  DO:
//      1.check whether the `exp` expression is legal.
//      2.check whether the `exp` expression type is not myTypeNil and myTypeNoReturn.
//  RETURN:
//      a boolean variable indicates whether this function succeeds.
//  STATUS:
//      Tested.
bool MySemantic_Dec_Var_ShortForm(myShortFormVar shortFormVar, IR_myStatement* resultPtr)
{
    IR_myExp valueResult = NULL;
    bool isValueExpLegal = isExpLegalWithTrans(shortFormVar->exp, &valueResult);

    if (isValueExpLegal)
    {
        myType actualValueType =
            getExpActualResultType(shortFormVar->exp);
        if (!isTypeNil(actualValueType) && !isTypeNoReturn(actualValueType))
        {
            allocateAndAddNewVar_Short(shortFormVar, actualValueType);
            *resultPtr = doAssignmentTranslation(
                getVarAccessFromName(shortFormVar->name),valueResult)
                ->u.eseq.statement;
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

void allocateAndAddNewVar_Short(myShortFormVar shortFormVar, myType actualValueType)
{
    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();

    if (isTypeArray(actualValueType) || isTypeRecord(actualValueType) ||
        isTypeString(actualValueType))
        shortFormVar->escape = true;

    Trans_myAccess varAccess = Trans_allocateLocal(
        MySemantic_getCurrentLevel(), shortFormVar->escape);
    myVarAndFuncEntry varEntry = myEnvironment_makeVarEntry(
        varAccess, actualValueType);
    MyEnvironment_addNewVarOrFunc(varAndFuncEnv, shortFormVar->name, varEntry);   
}

/////////////////////////////////////////////////////////////////////////////////

//  forwards
bool isVarDecValueTypeMatchesGivenType(myLongFormVar longFormVar);
void allocateAndAddNewVar_Long(myLongFormVar longFormVar);
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
bool MySemantic_Dec_Var_LongForm(myLongFormVar longFormVar, IR_myStatement* resultPtr)
{
    bool isVariableTypeDefined = isTypeDefined(longFormVar->type);

    IR_myExp valueResult = NULL;
    bool isValueExpLegal = isExpLegalWithTrans(
        longFormVar->exp, &valueResult);

    bool isTypeMatches = false;
    if (isVariableTypeDefined && isValueExpLegal)
    {
        isTypeMatches = isVarDecValueTypeMatchesGivenType(
            longFormVar);
    }

    //    hidden checking for the other two booleans
    if (isTypeMatches)
    {
        allocateAndAddNewVar_Long(longFormVar);
        *resultPtr = doAssignmentTranslation(
            getVarAccessFromName(longFormVar->name), valueResult)
            ->u.eseq.statement;
        return true;
    }
    else
    {
        processLongFormVarDecErrors(
            isVariableTypeDefined, isValueExpLegal,
            isTypeMatches, longFormVar->type);
        return false;
    }
}

bool isVarDecValueTypeMatchesGivenType(myLongFormVar longFormVar)
{
    myType valueType = 
        getExpActualResultType(longFormVar->exp);
    myType givenType = 
        getActualTypeFromName(longFormVar->type);

    return isTypeEqual(valueType, givenType) ||
            isNilValueOfRecordType(givenType, valueType);
}

void allocateAndAddNewVar_Long(myLongFormVar longFormVar)
{
    myTable varAndFuncEnv = MySemantic_getVarAndFuncEnvironment();
    myType actualValueType = getActualTypeFromName(longFormVar->type);
    if (isTypeArray(actualValueType) || isTypeRecord(actualValueType) ||
        isTypeString(actualValueType))
        longFormVar->escape = true;

    Trans_myAccess varAccess = Trans_allocateLocal(
        MySemantic_getCurrentLevel(), longFormVar->escape);
    myVarAndFuncEntry varEntry =
        myEnvironment_makeVarEntry(varAccess, actualValueType);
            
    MyEnvironment_addNewVarOrFunc(varAndFuncEnv, longFormVar->name,varEntry);
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
bool MySemantic_Dec_Var(myVarDec varDec, IR_myStatement* resultPtr)
{
    if (varDec == NULL)
    {
        *resultPtr = NULL;
        return true;
    }

    switch (varDec->kind)
    {
        case ShortFormVar:
            return MySemantic_Dec_Var_ShortForm(
                varDec->u.shortFormVar, resultPtr);
        case LongFormVar:
            return MySemantic_Dec_Var_LongForm(                
                varDec->u.longFormVar, resultPtr);
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
bool MySemantic_Decs(myDecList decs, IR_myStatement* resultPtr)
{
    return MySemantic_Decs_Recursive(decs, resultPtr);
}

///////////////////////////////////////////////////////////////////////////////

//  forwards
bool areExpressionsLegal(myExpList exps, IR_myExp* expsResult);
myType getLastExpResultType(myExpList exps);
IR_myExp combineDecsAndExpsResults(IR_myStatement decsResult, IR_myExp expsResult);
void processLetErrors(bool areDeclarationsLegal, bool isBodyLegal);

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
myTranslationAndType MySemantic_LetExp(myLetExp letExp)
{
    MySymbol_BeginScope(MySemantic_getVarAndFuncEnvironment());
    MySymbol_BeginScope(MySemantic_getTypeEnvironment());

    IR_myStatement decsResult = NULL;
    bool areDeclarationsLegal =
        MySemantic_Decs_Recursive(letExp->decList, &decsResult);

    IR_myExp expsResult = NULL;
    bool isBodyLegal =
        areExpressionsLegal(letExp->expList, &expsResult);

    myTranslationAndType result;
    if (areDeclarationsLegal && isBodyLegal)
    {
        result = make_MyTranslationAndType(
            combineDecsAndExpsResults(decsResult, expsResult),
            getLastExpResultType(letExp->expList));
    }
    else
    {
        processLetErrors(areDeclarationsLegal, isBodyLegal);
        result = SEMANTIC_ERROR;
    }

    MySymbol_EndScope(MySemantic_getVarAndFuncEnvironment());
    MySymbol_EndScope(MySemantic_getTypeEnvironment());
    return result;
}

IR_myExp combineDecsAndExpsResults(IR_myStatement decsResult, IR_myExp expsResult)
{
	//  areExpressionsLegal() ensures it
	assert (expsResult->kind == IR_ESeq);

	return IR_makeESeq(
	    IR_makeSeq(decsResult, expsResult->u.eseq.statement),
	    expsResult->u.eseq.exp);
}

//////////////////////////

#define STATEMENT_INIT(stateResult)             \
    IR_myStatement* stateIter = &stateResult;   \
    IR_myStatement* nextIter; 

#define STATEMENT_ALLOCATE_AND_SET()            \
    *stateIter = IR_makeSeq(NULL, NULL);        \
    nextIter = &(*stateIter)->u.seq.right;      \
    stateIter = &(*stateIter)->u.seq.left;

#define STATEMENT_MOVE_TO_NEXT()                \
    stateIter = nextIter;

bool areExpressionsLegal(myExpList exps, IR_myExp* expResult)
{
    IR_myStatement stateResult = NULL;
    IR_myExp valueResult = NULL;

    STATEMENT_INIT(stateResult);
    myTranslationAndType oneReturn = NULL; 
    while (exps)
    {
        oneReturn = MySemantic_Exp_(exps->exp); 
        if (oneReturn == SEMANTIC_ERROR)
        {
            *expResult = NULL;
            return false;
        }
        else
        {
            STATEMENT_ALLOCATE_AND_SET();
            IR_divideExp(oneReturn->translation, stateIter, &valueResult);
            STATEMENT_MOVE_TO_NEXT();
        }

        exps = exps->next;
    }

    //  last expression has no return
    if (oneReturn == NULL || isTypeNoReturn(oneReturn->type))
        valueResult = NULL;
    *expResult = IR_makeESeq(stateResult, valueResult);
    return true;
}

#undef STATEMENT_INIT
#undef STATEMENT_ALLOCATE_AND_SET
#undef STATEMENT_MOVE_TO_NEXT

//////////////////////////

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
 *          Break expression can only be inside loops.
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

//////////////////////////////////////////////////////////

myTranslationAndType MySemantic_Exp_(myExp exp)
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

//////////////////////////////////////////////////////////////////////////
//
//  public function

//  To make fewer parameters in functions, there is a need to make this
//  function call another.
myTranslationAndType MySemantic_Exp(myExp exp)
{
    Escape_findEscape(exp);

    MySemantic_enterNewLevel(Trans_outermostLevel());
    myTranslationAndType result = MySemantic_Exp_(exp);
    MySemantic_leaveNewLevel();
    return result;
}