#ifndef MY_TRANSLATE_H
#define MY_TRANSLATE_H

#include "myFrame.h"
#include "myAbstractSyntax.h"
#include "myIRTree.h"

///////////////////////////////////////////////////////////////////////////
//
//  This module is a high level abstration of other low level abstractions.
//
///////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//                      typedefs

//  level is just a nested frame.
typedef struct Trans_myLevel_*      Trans_myLevel;
typedef struct Trans_myAccess_*     Trans_myAccess;


typedef struct Trans_myAccessList_
{
    Trans_myAccess              head;
    struct Trans_myAccessList_* tail;
}* Trans_myAccessList;

typedef struct Trans_myExp_*    Trans_myExp;

////////////////////////////////////////////////////////////////////
//                          functions

Trans_myAccessList Trans_makeAccessList(
    Trans_myAccess head, Trans_myAccessList tail);

//  DO:
//      get the account of the given list.
//  PARAMS:
//      list:   a Trans_myAccessList list.
//  RETURN:
//      the count of that list.
int Trans_getAccessListCount(Trans_myAccessList list);

//  DO:
//      get the level within which this access lives in.
//  PARSMS:
//      access:     an access.
//  RETURN:
//      the level within which this access lives in.
Trans_myLevel Trans_getAccessLevel(Trans_myAccess access);

//  DO:
//      compare if two level equals.
//  PARAMS:
//      lhs:    left operand.
//      rhs:    right operand.
//  RETURN:
//      a bool variable.
bool Trans_isLevelEqual(Trans_myLevel lhs, Trans_myLevel rhs);

bool Trans_isLevelSame(Trans_myLevel lhs, Trans_myLevel rhs);

//  DO:
//      make a level within which one program is nested.
//  PARAMS:
//      none.
//  RETURN:
//      a new Trans_myLevel variable.
//  REMARK:
//      "main" all "Library" functions are declared at this outermost level.
//      It contains no formals and returns the same level every time it is called.
Trans_myLevel Trans_outermostLevel(void);

//  DO:
//      check if the given level is the outermost level.
//  PARAMS:
//      level:  the level to check.
//  RETURN:
//      a bool variable.
//  REMARK:
//      it's a checked runtime error to compare when no Trans_outermostLevel()
//      not called once yet or to pass a NULL level!
bool Trans_isOutermostLevel(Trans_myLevel level);

//  DO:
//      make a new nested level.
//  PARAMS:
//      parent:     the enclosing level of this level.
//      name:       the label of this level.
//      formals:    a list of formal escape flags.
//  RETURN:
//      a new level.
Trans_myLevel Trans_newLevel(
    Trans_myLevel parent, myLabel name, myBoolList formals);

//  DO:
//      allocate a local variable in the given level.
//  PARAMS:
//      level:      the level which this variable lives in.
//      escapeFlag: a bool indicates whether this variable is escaped.
//  RETURN:
//      a Trans_myAccess variable representing the new local variable.
Trans_myAccess Trans_allocateLocal(Trans_myLevel level, bool escapeFlag);

//  DO:
//      get the formals(not include static link) inside the given level.
//  PARAMS:
//      level:  a level.
//  RETURN:
//      it returns the formals insided this level(not include static link). If there is
//      no formals(include outermost level), it returns NULL.
//  REMARK:
//      once a level is created, its formal can not be changed any more!
Trans_myAccessList Trans_getFormals(Trans_myLevel level);


//  todo:
myFrame Trans_getFrame(Trans_myLevel level);

void Trans_procEntryExit(
    Trans_myLevel level, Trans_myExp body, Trans_myAccessList formals);

////////////////////////////////////////////////////////////////////////////////
//

//  todo:
void Trans_addOneProcFrag(Frame_myFrag frag);
Frame_myFragList Trans_getProcFrags(void);
void Trans_resetProcFrags(void);

void Trans_addOneStringFrag(Frame_myFrag frag);
Frame_myFragList Trans_getStringFrags(void);
void Trans_resetStringFrags(void);

/////////////////////////////////////////////////////////////////////////////////
//                              translaters

IR_myExp Trans_LValueExp_GetRecordPtr(myLValueExp lValueExp);
IR_myExp Trans_LValueExp_GetArrayPtr(myLValueExp lValueExp);
IR_myStatement Trans_VarDec(myVarDec varDec);

IR_myExp Trans_LValueExp_SimpleVar(myLValueExp lValueExp);
IR_myExp Trans_LValueExp_RecordField(myLValueExp lValueExp);

IR_myExp Trans_IntegerLiteralExp(myIntegerLiteralExp integerLiteralExp);

///
/// \brief      translate a let-exp into intermediate code
/// \params[in] letExp  a let expression.
/// \returns    an IR represetation of this let expression.
///
/// \remark     The value of the last expression is the entire value of the
///             let-exp. if empty body, empty decs or no-value, then the
///             value of this expression is 0; 
IR_myExp Trans_LetExp(myLetExp letExp);
IR_myExp Trans_Exp_(myExp exp);

//////////////////////////////////////

void Trans_proccedureBody(IR_myExp bodyResult, mySymbol procName);
void Trans_functionBody(IR_myExp bodyResult, mySymbol funcName);

#endif