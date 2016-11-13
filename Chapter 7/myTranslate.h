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




void Trans_procEntryExit(
    Trans_myLevel level, Trans_myExp body, Trans_myAccessList formals);
Frame_myFragList Trans_gePprocFrags(void);

void Trans_putStringFrag(myLabel label, myString str);
Frame_myFragList Trans_getStrFrags(void);

//////////////////////////////////////////////////////////////////////////
//                              translaters

//  DO:
//      translate a simple var into IR representations.
//  PARAMS:
//      access      an access variable.
//      left        the level inside which this variable is used.
//  RETURN;
//      an IR repreesntation.
//  REMARK:
//      variables which are nested used are also processed.
Trans_myExp Trans_simpleVar(Trans_myAccess access, Trans_myLevel level);

Trans_myExp Trans_LValueExp_SimpleVar(myLValueExp lValueExp);

#endif