#ifndef MY_TRANSLATE_H
#define MY_TRANSLATE_H

#include "myFrame.h"

////////////////////////////////////////////////////////////////////
//                      typedefs

typedef myFrame                     Trans_myLevel;
typedef struct Trans_myAccess_*     Trans_myAccess;


typedef struct Trans_myAccessList_
{
    Trans_myAccess              head;
    struct Trans_myAccessList_* tail;
}* Trans_myAccessList;

Trans_myAccessList Trans_makeAccessList(
    Trans_myAccess head, Trans_myAccessList tail);

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
Trans_myLevel Trans_outermostLevel  (void);

//  DO:
//      make a new level.
//  PARAMS:
//      parent:     the enclosing level of this level.
//      name:       the label of this level.
//      formals:    a list of formal escape flags.
//  RETURN:
//      a new level.
Trans_myLevel Trans_newLevel(
    Trans_myLevel parent, myLabel name, myBoolList formals);

Trans_myAccess      Trans_allocateLocal (Trans_myLevel level, bool escapeFlag);
Trans_myAccessList  Trans_formals       (Trans_myLevel level);

#endif