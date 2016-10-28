#include "myEscape.h"
#include "myEnvironment.h"

#include <assert.h> //  for assert()
#include <stdlib.h> //  for NULL

/////////////////////////////////////////////////////////////////////
//      This module assumes all phases will passed, then it can find
//      the escape conditions of variables.
/////////////////////////////////////////////////////////////////////


//
//  Escape conditions:
//      1.passed by reference(record type, array type).
//      2.accessed by nested functions.
//      3.too big to fit in a register(record type).
//      4.the variable is an array.
//

////////////////////////////////////////////////////////////////////////
//                          structures

typedef struct EscapeEntry_
{
    //  there maybe redefined vars, so depth needed
    int     depth;
    bool    used;
    bool    escape;
}*  EscapeEntry;

////////////////////////////////////////////////////////////////////////
//                          global variables

static myTable g_escapeTable = 0;

////////////////////////////////////////////////////////////////////////
//                          private functions

static void Escape_addVarEntry(mySymbol varSymbol, EscapeEntry entry)
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

static EscapeEntry makeDefaultEscapeEntry(int depth)
{
    EscapeEntry entry = makeMemoryBlock(sizeof(*entry), MEMORY_TYPE_NONE);
    assert (entry);

    entry->depth = depth;
    entry->used = false;
    entry->escape = true;
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
    Escape_getVarEntry(varSymbol)->escape = true;
}

static bool getVarEscape(mySymbol varSymbol)
{
    return Escape_getVarEntry(varSymbol)->escape;
}

////////////////////////////////////////////////////////////////////////
//                          forwards
void Escape_findEscape_Exp(int depth, myExp exp);

///////////////////////////////////////////////////////////////////////
//                          functions

//  DO:
//      check if variables used are in outer scopes, which escapes.
void Escape_findEscape_LValueExp(int depth, myLValueExp lValueExp)
{
    switch (lValueExp->kind)
    {
        case SimpleVar:
            if (isVarNestedUsed(depth, lValueExp->id))
                setVarEscape(lValueExp->id);
            break;
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

void Escape_findEscape_FunctionCallExp(
    int depth, myFunctionCallExp functionCallExp)
{
    //  todo:
}

////////////////////////////////////////////////////////////////////////

void Escape_findEscape_NilExp(int depth, myNilExp nilExp)
{
    //  todo:
}

////////////////////////////////////////////////////////////////////////

void Escape_findEscape_IntegerLiteralExp(
    int depth, myIntegerLiteralExp integerLiteralExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_StringLiteralExp(
    int depth, myStringLiteralExp stringLiteralExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ArrayCreationExp(
    int depth, myArrayCreationExp arrayCreationExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_RecordCreationExp(
    int depth, myRecordCreationExp recordCreationExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ArithmeticExp(
    int depth, myArithmeticExp arithmeticExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ParenthesesExp(
    int depth, myParenthesesExp parenthesesExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_NoValueExp(int depth, myNoValueExp noValueExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_SequencingExp(int depth, mySequencingExp sequencingExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ForExp(int depth, myForExp forExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_IfThenElseExp(int depth, myIfThenElseExp ifThenElseExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_IfThenExp(int depth, myIfThenExp ifThenExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_ComparisonExp(int depth, myComparisonExp comparisonExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_BooleanOperateExp(
    int depth, myBooleanOperateExp booleanOperateExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_AssignmentExp(int depth, myAssignmentExp assignmentExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void Escape_findEscape_VarDec(int depth, myVarDec varDec)
{
    #error "test here"
    if (varDec != NULL)
    {
        EscapeEntry entry = makeDefaultEscapeEntry(depth);
        switch (varDec->kind)
        {
            case ShortFormVar:
                Escape_addVarEntry(varDec->u.shortFormVar->name, entry);
                Escape_findEscape_Exp(depth, varDec->u.shortFormVar->exp);
                break;
            case LongFormVar:
                Escape_addVarEntry(varDec->u.longFormVar->name, entry);
                Escape_findEscape_Exp(depth, varDec->u.longFormVar->exp);
                break;
            default:        assert (false);
        }
    }
}

/////////////////////////

void Escape_findEscape_TypeDec(int depth, myTypeDec typeDec)
{
    //  todo:
}

/////////////////////////

void Escape_findEscape_FuncDec(int depth, myFuncDec funcDec)
{
    //  todo:
}

/////////////////////////

void Escape_findEscape_Dec(int depth, myDec dec)
{
    //  todo:
    /*
            case VarDec:
                Escape_addVarEntry(
                    getVarSymbol(decs->dec->u.varDec),
                    makeEscapeEntry(depth, false));
                break;
            case TypeDec:
            case FuncDec:
            default:    ;   //  do nothing*/
    switch (dec->kind)
    {
        case VarDec:
            return Escape_findEscape_VarDec(depth, dec->u.varDec);
        case TypeDec:
            return Escape_findEscape_TypeDec(depth, dec->u.tyDec);
        case FuncDec:
            return Escape_findEscape_FuncDec(depth, dec->u.funcDec);
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
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_NegationExp(int depth, myNegationExp negationExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_BreakExp(int depth, myBreakExp breakExp)
{
    //  todo:
}

///////////////////////////////////////////////////////////////////////

void Escape_findEscape_Exp(int depth, myExp exp)
{
    switch (exp->kind)
    {
        /*case LValueExp:
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
            return Escape_findEscape_ArrayCreation(depth, exp->u.arrayCreationExp);
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
            return Escape_findEscape_ForExp(depth, exp->u.forExp);
        case IfThenElseExp:
            return Escape_findEscape_IfThenElseExp(depth, exp->u.ifThenElseExp);
        case IfThenExp:
            return Escape_findEscape_IfThenExp(depth, exp->u.ifThenExp);
        case ComparisonExp:
            return Escape_findEscape_ComparisonExp(depth, exp->u.comparisonExp);
        case BooleanOperateExp:
            return Escape_findEscape_BooleanOperateExp(depth, exp->u.booleanOperateExp);
        case AssignmentExp:
            return Escape_findEscape_AssignmentExp(depth, exp->u.assignmentExp);*/
        case LetExp:
            return Escape_findEscape_LetExp(depth, exp->u.letExp);
        /*case WhileExp:
            return Escape_findEscape_WhileExp(depth, exp->u.whileExp);
        case NegationExp:
            return Escape_findEscape_NegationExp(depth, exp->u.negationExp);
        case BreakExp:
            return Escape_findEscape_BreakExp(depth, exp->u.breakExp);*/

        default:    assert (false);
    }
} 

///////////////////////////////////////////////////////////////////////////

void Escape_findEscape(myExp exp)
{
    Escape_findEscape_Exp(1, exp);
}

bool Escape_isVarEscaped(mySymbol varSymbol)
{
    assert (!MySymbol_InvalidSymbol(varSymbol));
    return getVarEscape(varSymbol);
}