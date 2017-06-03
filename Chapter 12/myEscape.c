#include "myEscape.h"
#include "myEnvironment.h"
#include "mySemantic.h"
#include "typeChecker.h"

#include <assert.h> //  for assert()
#include <stdlib.h> //  for NULL

/////////////////////////////////////////////////////////////////////
//  NOTE:
//      This module assumes should take into account when abstract
//      syntaxes are illegal, because escape-finding must be made 
//      before semantic analysis begins, every variable have to be
//      `found` everywhere to decide whether it escapes. 
/////////////////////////////////////////////////////////////////////


//
//  Escape conditions:
//      1.passed by reference(record type, array type).
//      2.too big to fit in a register(record type).
//      3.the variable is an array.
//      4.accessed by nested functions.
//  Namely:
//      variables of Record and Array type are escaped, and nested-used
//      variables are escaped also.
//

////////////////////////////////////////////////////////////////////////
//                          structures

typedef struct EscapeEntry_
{
    //  there maybe redefined vars, so depth needed
    int         depth;
    /*bool    used;*/
    bool*       escapePtr;
}*  EscapeEntry;

////////////////////////////////////////////////////////////////////////
//                          global variables

static myTable g_escapeTable = 0;

////////////////////////////////////////////////////////////////////////
//                          private functions

void Escape_addVarEntry(mySymbol varSymbol, EscapeEntry entry)
{
    if (g_escapeTable == NULL) 
        g_escapeTable = MyTable_MakeEmptyTable_();

    assert (!MySymbol_InvalidSymbol(varSymbol));
    assert (entry);
    MySymbol_Enter(g_escapeTable, varSymbol, entry);
}

static EscapeEntry Escape_getVarEntry(mySymbol varSymbol)
{
    assert (!MySymbol_InvalidSymbol(varSymbol));
    EscapeEntry entry = MySymbol_Look(g_escapeTable, varSymbol);

    assert (entry);
    return entry;
}

EscapeEntry makeEscapeEntry(int depth, bool* escapePtr)
{
    EscapeEntry entry = makeMemoryBlock(sizeof(*entry), MEMORY_TYPE_NONE);
    assert (entry);

    entry->depth = depth;
    /*entry->used = false;*/
    entry->escapePtr = escapePtr;
    *escapePtr = false;
    return entry;
}

static bool isVarNestedUsed(int depth, mySymbol varSymbol)
{
    EscapeEntry entry = Escape_getVarEntry(varSymbol);
    assert (depth >= entry->depth);

    if (entry->depth == depth)
        return false;
    else
        return true;
}

static void setVarEscape(mySymbol varSymbol)
{
    (*Escape_getVarEntry(varSymbol)->escapePtr) = true;
}

static bool getVarEscape(mySymbol varSymbol)
{
    return *Escape_getVarEntry(varSymbol)->escapePtr;
}

///////////////////////
/*
static myType getActualType(myType type)
{
    //  encountered a semantic error!
    if (type == NULL)   return NULL;

    while(isTypeNamed(type))
    {
        type = type->u.typeNamed->type;
    }
    return type;
}

static myType getActualVarTypeFromName(mySymbol varName)
{
    myVarAndFuncEntry entry = MyEnvironment_getVarOrFuncFromName(
                MySemantic_getVarAndFuncEnvironment(), varName);
    if (entry == NULL)
        return NULL;
    else
        return getActualType(MyEnvironment_getVarType(entry));
} 
*/
////////////////////////////////////////////////////////////////////////
//                          forwards
void Escape_findEscape_Exp(int depth, myExp exp);

///////////////////////////////////////////////////////////////////////
//                          functions

//  DO:
//      check if variables used are in inner scopes, which escapes.
void Escape_findEscape_LValueExp(int depth, myLValueExp lValueExp)
{
    switch (lValueExp->kind)
    {
        case SimpleVar:
        {
            if (isVarNestedUsed(depth, lValueExp->id))
                setVarEscape(lValueExp->id);
            break;
        }
        case RecordField:
            //  record is always referenced by pointer
            //  down through
        case ArraySubscript:
            //  array is always referenced by pointer
            setVarEscape(lValueExp->id);
            break;
        default:    assert (false);
    }
}

///////////////////////////////////////////////////////////////////////

//  REMARK:
//      function parameters are all passed by value, so just find escapes
//      in side the formal expressions.
void Escape_findEscape_FunctionCallExp(
    int depth, myFunctionCallExp functionCallExp)
{
    switch (functionCallExp->kind)
    {
        case ParamFunctionCallExp:
        {
            myExpList exps =
                functionCallExp->u.paramFunctionCallExp->expList;
            while (exps)
                Escape_findEscape_Exp(depth, exps->exp),
                exps = exps->next;
            break;
        }
        case NoParamFunctionCallExp:
            break;  //  do nothing
        default:        assert (false);
    }
}

////////////////////////////////////////////////////////////////////////

void Escape_findEscape_NilExp(int depth, myNilExp nilExp)
{
    //  do nothing
}

////////////////////////////////////////////////////////////////////////

void Escape_findEscape_IntegerLiteralExp(
    int depth, myIntegerLiteralExp integerLiteralExp)
{
    //  actually numeric constants, do nothing
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_StringLiteralExp(
    int depth, myStringLiteralExp stringLiteralExp)
{
    //  actually string constants, do nothing
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ArrayCreationExp(
    int depth, myArrayCreationExp arrayCreationExp)
{
    Escape_findEscape_Exp(depth, arrayCreationExp->initial);
    Escape_findEscape_Exp(depth, arrayCreationExp->length);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_RecordCreationExp(
    int depth, myRecordCreationExp recordCreationExp)
{
    switch (recordCreationExp->kind)
    {
        case FieldRecordCreationExp:
        {
            myRecordFieldList fields =
                recordCreationExp->u.fieldRecordCreationExp->fieldList;
            while (fields)
                Escape_findEscape_Exp(depth, fields->field->varValue),
                fields = fields->next;
            break;
        }
        case NoFieldRecordCreationExp:
            break;  //  do nothing
        default:        assert (false);
    }
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ArithmeticExp(
    int depth, myArithmeticExp arithmeticExp)
{
    Escape_findEscape_Exp(depth, arithmeticExp->left);
    Escape_findEscape_Exp(depth, arithmeticExp->right);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ParenthesesExp(
    int depth, myParenthesesExp parenthesesExp)
{
    Escape_findEscape_Exp(depth, parenthesesExp->exp);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_NoValueExp(int depth, myNoValueExp noValueExp)
{
    //  do nothing
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_SequencingExp(int depth, mySequencingExp sequencingExp)
{
    Escape_findEscape_Exp(depth, sequencingExp->exp1);
    Escape_findEscape_Exp(depth, sequencingExp->exp2);

    myExpList exps = sequencingExp->nextList;
    while (exps)
        Escape_findEscape_Exp(depth, exps->exp),
        exps = exps->next;
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ForExp(int depth, myForExp forExp)
{
    //  set escape flags for loop-var
    Escape_addVarEntry(forExp->varName,
        makeEscapeEntry(depth, &forExp->varEscape));

    Escape_findEscape_Exp(depth, forExp->varRangeLow);
    Escape_findEscape_Exp(depth, forExp->varRangeHigh);
    Escape_findEscape_Exp(depth, forExp->bodyExp);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_IfThenElseExp(int depth, myIfThenElseExp ifThenElseExp)
{
    Escape_findEscape_Exp(depth, ifThenElseExp->exp1);
    Escape_findEscape_Exp(depth, ifThenElseExp->exp2);
    Escape_findEscape_Exp(depth, ifThenElseExp->exp3);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_IfThenExp(int depth, myIfThenExp ifThenExp)
{
    Escape_findEscape_Exp(depth, ifThenExp->exp1);
    Escape_findEscape_Exp(depth, ifThenExp->exp2);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ComparisonExp(int depth, myComparisonExp comparisonExp)
{
    Escape_findEscape_Exp(depth, comparisonExp->left);
    Escape_findEscape_Exp(depth, comparisonExp->right);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_BooleanOperateExp(
    int depth, myBooleanOperateExp booleanOperateExp)
{
    Escape_findEscape_Exp(depth, booleanOperateExp->left);
    Escape_findEscape_Exp(depth, booleanOperateExp->right);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_AssignmentExp(int depth, myAssignmentExp assignmentExp)
{
    //  the let operand of this expression may be nested, so check it also.
    Escape_findEscape_Exp(depth, assignmentExp->exp);
    Escape_findEscape_LValueExp(depth, assignmentExp->lValueExp);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

static void getVarNameAndBody(
    mySymbol* varNamePtr, myExp* varExpPtr, myVarDec varDec)
{
    switch (varDec->kind)
    {
        case ShortFormVar:
            *varNamePtr = varDec->u.shortFormVar->name;
            *varExpPtr = varDec->u.shortFormVar->exp;
            break;
        case LongFormVar:
            *varNamePtr = varDec->u.longFormVar->name;
            *varExpPtr = varDec->u.longFormVar->exp;
            break;
        default:        assert (false);
    }
}

//  STATUS:
//      Tested.
void Escape_findEscape_VarDec(int depth, myVarDec varDec)
{
    if (varDec != NULL)
    {
        mySymbol varName;
        myExp varExp;
        getVarNameAndBody(&varName, &varExp, varDec);

        bool* escapePtr = varDec->kind == LongFormVar ?
            &(varDec->u.longFormVar->escape) :
            &(varDec->u.shortFormVar->escape);
        Escape_addVarEntry(varName, makeEscapeEntry(depth, escapePtr));

        //  escape finding should not check variable types, because it
        //  is called before semantic analysis begins.
        Escape_findEscape_Exp(depth, varExp);
    }
}

/////////////////////////

//  STATUS:
//      Tested.
void Escape_findEscape_TypeDec(int depth, myTypeDec typeDec)
{
    //  no expressions used here, do nothing
}

/////////////////////////

void treatFormalsAsNotEscapedVars(int depth, myTyFieldList fields)
{ 
    while (fields)
    {
        mySymbol varName = fields->field->varName;
        Escape_addVarEntry(varName,
            makeEscapeEntry(depth, &fields->field->varEscape));

        fields = fields->next;
    }
}

//  NOTE:
void Escape_findEscape_FuncDec(int depth, myFuncDec funcDec)
{
    myTyFieldList formalFields;
    myExp body;

    switch (funcDec->kind)
    {
        case FunctionDec:
        {
            //  assume all formals escapes
            formalFields = funcDec->u.functionDec->tyFieldList;
            body = funcDec->u.functionDec->exp;
            break;
        }
        case ProcedureDec:
        {
            //  assume all formals escapes
            formalFields = funcDec->u.procedureDec->tyFieldList;
            body = funcDec->u.procedureDec->exp;
            break;
        }
        default:        assert (false);
    }

    treatFormalsAsNotEscapedVars(depth, formalFields);
    Escape_findEscape_Exp(depth, body);
}

/////////////////////////

void Escape_findEscape_Dec(int depth, myDec dec)
{
    switch (dec->kind)
    {
        case VarDec:
            return Escape_findEscape_VarDec(depth, dec->u.varDec);
        case TypeDec:
            return Escape_findEscape_TypeDec(depth, dec->u.tyDec);
        case FuncDec:
            MySymbol_BeginScope(Escape_getEscapeEnvironment());
            Escape_findEscape_FuncDec(depth + 1, dec->u.funcDec);
            MySymbol_EndScope(Escape_getEscapeEnvironment());
            return;
        default:        assert (false);
    }
}

static void Escape_findEscape_Decs(int depth, myDecList decs)
{
    while (decs)
    {
        Escape_findEscape_Dec(depth, decs->dec);   
        decs = decs->next;
    }
}

static void Escape_findEscape_Exps(int depth, myExpList exps)
{
    while (exps)
    {
        Escape_findEscape_Exp(depth, exps->exp);
        exps = exps->next;
    }
}

void Escape_findEscape_LetExp(int depth, myLetExp letExp)
{
   Escape_findEscape_Decs(depth, letExp->decList);
   Escape_findEscape_Exps(depth, letExp->expList);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void Escape_findEscape_WhileExp(int depth, myWhileExp whileExp)
{
    Escape_findEscape_Exp(depth, whileExp->whileExp);
    Escape_findEscape_Exp(depth, whileExp->bodyExp);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_NegationExp(int depth, myNegationExp negationExp)
{
    Escape_findEscape_Exp(depth, negationExp->exp);
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_BreakExp(int depth, myBreakExp breakExp)
{
    //  do nothing
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_Exp(int depth, myExp exp)
{
    switch (exp->kind)
    {
        case LValueExp:
            return Escape_findEscape_LValueExp(depth, exp->u.lValueExp);
        case FunctionCallExp:
            //  parameters and return values is copied by value.  
            return Escape_findEscape_FunctionCallExp(depth, exp->u.functionCallExp);
        case NilExp:
            return Escape_findEscape_NilExp(depth, exp->u.nilExp);
        case IntegerLiteralExp:
            return Escape_findEscape_IntegerLiteralExp(depth, exp->u.integerLiteralExp);
        case StringLiteralExp:
            return Escape_findEscape_StringLiteralExp(depth, exp->u.stringLiteralExp);
        case ArrayCreationExp:
            return Escape_findEscape_ArrayCreationExp(depth, exp->u.arrayCreationExp);
        case RecordCreationExp:
            return Escape_findEscape_RecordCreationExp(depth, exp->u.recordCreationExp);
        case ArithmeticExp:
            return Escape_findEscape_ArithmeticExp(depth, exp->u.arithmeticExp);
        case ParenthesesExp:
            return Escape_findEscape_ParenthesesExp(depth, exp->u.parenthesesExp);
        case NoValueExp:
            return Escape_findEscape_NoValueExp(depth, exp->u.noValueExp);
        case SequencingExp:
            return Escape_findEscape_SequencingExp(depth, exp->u.sequencingExp);
        case ForExp:
            MySymbol_BeginScope(Escape_getEscapeEnvironment());
            Escape_findEscape_ForExp(depth, exp->u.forExp);
            MySymbol_EndScope(Escape_getEscapeEnvironment());
            return;
        case IfThenElseExp:
            return Escape_findEscape_IfThenElseExp(depth, exp->u.ifThenElseExp);
        case IfThenExp:
            return Escape_findEscape_IfThenExp(depth, exp->u.ifThenExp);
        case ComparisonExp:
            return Escape_findEscape_ComparisonExp(depth, exp->u.comparisonExp);
        case BooleanOperateExp:
            return Escape_findEscape_BooleanOperateExp(depth, exp->u.booleanOperateExp);
        case AssignmentExp:
            return Escape_findEscape_AssignmentExp(depth, exp->u.assignmentExp);
        case LetExp:
            MySymbol_BeginScope(Escape_getEscapeEnvironment());
            Escape_findEscape_LetExp(depth, exp->u.letExp);
            MySymbol_EndScope(Escape_getEscapeEnvironment());
            return;
        case WhileExp:
            return Escape_findEscape_WhileExp(depth, exp->u.whileExp);
        case NegationExp:
            return Escape_findEscape_NegationExp(depth, exp->u.negationExp);
        case BreakExp:
            return Escape_findEscape_BreakExp(depth, exp->u.breakExp);

        default:    assert (false);
    }
} 

///////////////////////////////////////////////////////////////////////////

void Escape_findEscape(myExp exp)
{
    Escape_findEscape_Exp(1, exp);
}

bool Escape_isVarEscaped(mySymbol varName)
{
    assert (!MySymbol_InvalidSymbol(varName));
    return getVarEscape(varName);
}

myTable Escape_getEscapeEnvironment(void)
{
    if (g_escapeTable == NULL)
        g_escapeTable = MySymbol_MakeNewTable();
    return g_escapeTable;
}

void Escape_setEscapeEnvironment(myTable escapeEnv)
{
    assert (escapeEnv);
    g_escapeTable = escapeEnv;
}

/*
bool Escape_isVarUsed(mySymbol varName)
{
    assert (!MySymbol_InvalidSymbol(varName));
    return Escape_getVarEntry(varName)->used;
}*/