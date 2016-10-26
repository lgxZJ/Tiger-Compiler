#ifndef MY_FRAME_H
#define MY_FRAME_H

/////////////////////////////////////////////////////////////////
//		     an interface of Frame		                       //
//	each specific target machine should implement its own      //
//	version.					                               //
/////////////////////////////////////////////////////////////////

#include "myTemp.h"

///////////////////     typedefs    /////////////////////

typedef struct myFrame_	    	*myFrame;
typedef struct myAccess_	    *myAccess;

typedef struct myAccessList_
{
    myAccess                head;
    struct myAccessList_*   tail;
}*myAccessList;

typedef struct myBoolList_
{
    bool                    head;
    struct myBoolList_*     tail;
}*myBoolList;

///////////////////     functions       //////////////////

//  DO:
//      add a new bool flag in the head of the formers.
//  PARAMS:
//      formers:    the former bools.
//      next:       the new bool.
//  RETURN:
//      a new bool list.
//  STATUS:
//      Tested.
myBoolList Frame_makeBoolList(myBoolList formers, bool newOne);

//  DO:
//      add a new `access` into the head of the formers.
//  PARAMS:
//      formers:    the former `access`s.
//      newOne:     the new `access`
//  RETURN:
//      a new `access` list.
//  STATUS:
//      
myAccessList Frame_makeAccessList(myAccess newOne, myAccessList formers);


//  DO:
//      get the number of bools in list.
//  PARAMS:
//      list:   a list to look in.
//  RETURN:
//      the number of bool in the given list.
//  Tested.
int Frame_getBoolListCount(myBoolList list);

//  DO:
//      get the number of `access` in list.
//  PARAMS:
//      list:   a list to look in.
//  RETURN:
//      the number of `access` in the given list.
//  Tested.
int Frame_getAccessListCount(myAccessList list);

//  DO:
//      make a new frame with some formals inside it.
//  PARAMS:
//      frameLabel:     actually a function label.
//      formals:        flags indicates whether each parameter is escaped.
//  RETURN:
//      a new frame.
myFrame Frame_newFrame(myLabel frameLabel, myBoolList formalFlags);

//  DO:
//      compare if two frame equals.
//  PARAMS:
//      lhs:    left operand.
//      rhs:    right operand.
//  RETURN:
//      a bool variable.
bool Frame_isFrameEqual(myFrame lhs, myFrame rhs);

//  DO:
//      allocate a new local variable.
//  PARAMS:
//      frame:      the frame in which to allocate a new variable.
//      escapeFlag: a flag indicates whether this vaiable escapes.
//  RETURN:
//      a new myAccess variable.
myAccess Frame_allocateLocal(myFrame frame, bool escapeFlag);

//  DO:
//      get local variable count allocated so far.
//  PARAMS:
//      frame:  inside which to look local variables
//  RETURN:
//      the count of local variables
//  REMARK:
//      the return value can never be negative.
int Frame_getLocalCount(myFrame frame);

//  DO:
//      get all formals from one frame.
//  PARAMS:
//      frame:  inside which the formals is in.
//  RETURN:
//      a myAccessList variable containing formals' information.
//  STATUS:
//      
myAccessList Frame_getFormals(myFrame frame);

#endif
