#ifndef MY_TRANSLATE_H
#define MY_TRANSLATE_H

#include "myFrame.h"
#include "myAbstractSyntax.h"
#include "myIRTree.h"
#include "lValueTreater.h"

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


//  DO:
//      get the frame field of the given level.
//  PARAMS:
//      level   the given level
//  RETURN:
//      the frame field inside the given level.
myFrame Trans_getFrame(Trans_myLevel level);

//  todo:
void Trans_procEntryExit(
    Trans_myLevel level, Trans_myExp body, Trans_myAccessList formals);

////////////////////////////////////////////////////////////////////////////////
//

//  DO:
//      Save one routine fragment.
//  PARAMS:
//      frag    The fragment to be saved.. 
void Trans_addOneProcFrag(Frame_myFrag frag);

//  DO:
//      Get all the saved routine fragments.
//  RETURN:
//      A fragment list. 
Frame_myFragList Trans_getProcFrags(void);

//  DO:
//      Reset the routine fragment keeper.
void Trans_resetProcFrags(void);


//  DO:
//      Save one string fragment.
//  PARAMS:
//      frag    The fragment to be saved.
void Trans_addOneStringFrag(Frame_myFrag frag);

//  DO:
//      Get all the saved string fragments.
//  RETURN:
//      A fragment list. 
Frame_myFragList Trans_getStringFrags(void);

//  DO:
//      Reset the string fragment keeper.
void Trans_resetStringFrags(void);

/////////////////////////////////////////////////////////////////////////////////
//                              translaters

//
//  NOTE:   There are some functions not occured, such as Trans_sequencing(), 
//  Trans_parentheses(), and so on. Note these omitting is not a fault, they
//  have to be treated specially. For more details, see wiki.
//

IR_myExp Trans_decAssignment(Trans_myAccess varAccess, IR_myExp varBodyResult);
void Trans_proccedureDec    (IR_myExp bodyResult, mySymbol procName);
void Trans_functionDec      (IR_myExp bodyResult, mySymbol funcName);

IR_myExp Trans_LValueExp_GetRecordPtr   (myLValueExp lValueExp);
IR_myExp Trans_LValueExp_GetArrayPtr    (myLValueExp lValueExp);
IR_myExp Trans_LValueExp_simpleVar      (myLValueExp lValueExp);

IR_myExp Trans_noParamfunctionCall      (mySymbol funcName);
IR_myExp Trans_nil                      (void);
IR_myExp Trans_integerLiteralExp        (myIntegerLiteralExp integerLiteralExp);
IR_myExp Trans_stringLiteralExp         (myString str);
IR_myExp Trans_arrayCreation            (IR_myExp subscriptResult, IR_myExp initValueResult);
IR_myExp Trans_noFieldRecordCreation    (void);
IR_myExp Trans_arithmetic               (IR_myExp leftTran, IR_myExp rightTran, IR_BinOperator op);
IR_myExp Trans_noValue                  (void);

IR_myExp Trans_for      (
    myLabel endLabel,
    IR_myExp lowRangeResult,        IR_myExp highRangeResult,
    Trans_myAccess loopVarAccess,   IR_myExp loopBodyResult);
IR_myExp Trans_ifThenElse(
    IR_myExp conditionTrans, IR_myExp thenTrans,
    IR_myExp elseTrans, bool hasReturn);
IR_myExp Trans_ifThen   (
    IR_myExp conditionTrans, IR_myExp thenTrans);
IR_myExp Trans_comparison(
    IR_myExp leftTrans, IR_myExp rightTrans, IR_RelOperator op, bool isString);
IR_myExp Trans_booleanOperate(
    IR_myExp leftTrans, IR_myExp rightTrans, IR_BinOperator op);
IR_myExp Trans_assignment(
    IR_myExp leftTrans, IR_myExp rightTrans);
IR_myExp Trans_let      (
    IR_myStatement decsTrans, IR_myExp expsTrans);
IR_myExp Trans_while    (
    myLabel endLabel, IR_myExp condiTrans, IR_myExp expTrans);
IR_myExp Trans_negative (
    IR_myExp expTrans);
IR_myExp Trans_break    (
    myLabel targetLabel);

//////////////////////////////////////

//  used in mySemantic.c
void getAddressContent(IR_myExp* addressExpPtr);


#endif