#ifndef MY_FRAME_H
#define MY_FRAME_H

/////////////////////////////////////////////////////////////////
//		     an interface of Frame		                       //
//	each specific target machine should implement its own      //
//	version.					                               //
/////////////////////////////////////////////////////////////////

#include "myTemp.h"
#include "myIRTree.h"


extern const int Frame_wordSize;

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


typedef struct Frame_myFrag_
{
    enum { Frame_StringFrag, Frame_ProcFrag } kind;
    union
    {
        struct { myLabel label; myString str; } strFrag;
        struct { IR_myStatement body; myFrame frame; mySymbol funcName; } procFrag;
    }u;
}*  Frame_myFrag;

typedef struct Frame_myFragList_
{
    Frame_myFrag                head;
    struct Frame_myFragList_*   tail;
}*  Frame_myFragList;

///////////////////     functions       //////////////////

//  DO:
//      check if the given `access` is in `Frame`.
//  PARAMS:
//      access:     the given access.
//  RETURN:
//      a bool variable.
bool Frame_isAccessInFrame(myAccess access);

//  DO:
//      check if the given `access` is in `Reg`.
//  PARAMS:
//      access:     the given access.
//  RETURN:
//      a bool variable.
bool Frame_isAccessInReg(myAccess access);

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
//      Tested
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
//      make a new string fragement.
//  PARAMS:
//      label   the label of the string.
//      str     a string.
//  RETURN:
//      a valid string Fragement.
Frame_myFrag Frame_makeStringFrag(myLabel label, myString str);

//  DO:
//      make a new procedure fragement.
//  PARAMS:
//      bodyStatement   the statements of the procedure body.
//      frame           the frame inside which this pocedure is defined.
//  RETURN:
//      a valid procedure Fragement.
Frame_myFrag Frame_makeProcFrag(IR_myStatement bodyStatement, myFrame frame, mySymbol funcName);

//  DO:
//      make a fragement list;
//  PARAMS:
//      head        the head node.
//      tail        the tail of list.
//  RETURN:
//      a new fragement list.
Frame_myFragList Frame_makeFragList(Frame_myFrag head, Frame_myFragList tail);

//  DO:
//      make a new frame with some formals inside it.
//  PARAMS:
//      frameLabel:     actually a function label.
//      formals:        flags indicates whether each parameter is escaped.
//  RETURN:
//      a new frame.
//  REMARK:
//      frame related variables should be hold in stack, so there must be
//      some machine-depedent instructions to make it work.
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
//      compare if two frame are the same.
//  PARAMS:
//      lhs:    left operand.
//      rhs:    right operand.
//  RETURN:
//      a bool variable.
//  REMARK:
//      Same means the two frame are equal and their internal states
//      are also equal.
bool Frame_isFrameSame(myFrame lhs, myFrame rhs);

//  DO:
//      allocate a new local variable.
//  PARAMS:
//      frame:      the frame in which to allocate a new variable.
//      escapeFlag: a flag indicates whether this vaiable escapes.
//  RETURN:
//      a new myAccess variable.
myAccess Frame_allocateLocal(myFrame frame, bool escapeFlag);

//  DO:
//      get local variable(include formal local vars) count allocated so far.
//  PARAMS:
//      frame:  inside which to look local variables
//  RETURN:
//      the count of local variables
//  REMARK:
//      the return value can never be negative.
int Frame_getLocalCount(myFrame frame);

//  DO:
//      get variable count of formal local allocated so far.
//  PARAMS:
//      frame:  inside which to look local variables
//  RETURN:
//      the count of formal local variables
//  REMARK:
//      the return value can never be negative.
int Frame_getformalLocalCount(myFrame frame);

//  DO:
//      get variable offset.
//  PARAMS:
//      access  an access variable.
//  RETURN:
//      the offset in within the declared level.
int Frame_getAccessOffset(myAccess access);

//  DO:
//      get the `reg` field of myAccess.
//  PARAMS:
//      access  an myAccess variable.
//  RETURN:
//      the `reg` field.
myTemp Frame_getAccessReg(myAccess access);

//  DO:
//      get all formals from one frame.
//  PARAMS:
//      frame:  inside which the formals is in.
//  RETURN:
//      a myAccessList variable containing formals' information.
//  REMARK:
//      in this list, arguments appears in reverse order.
myAccessList Frame_getFormals(myFrame frame);

//  DO:
//      get the appropriate registers.
//  RETURN:
//      a temporary which holds the stack pointer.
myTemp Frame_EAX(void);
myTemp Frame_EBX(void);
myTemp Frame_ECX(void);
myTemp Frame_EDX(void);
myTemp Frame_ESI(void);
myTemp Frame_EDI(void);
myTemp Frame_EBP(void);

//  DO:
//      get the stack pointer of this target machine.
//  RETURN:
//      a temporary which holds the stack pointer.
myTemp Frame_ESP(void);

//  DO:
//      get the frame pointer of this target machine.
//  RETURN:
//      a temporary which holds the frame pointer.
myTemp Frame_FP(void);

//  DO:
//      get register of return value in this target machine.
//  RETURN:
//      a temporary which holds the return value register. 
myTemp Frame_RV(void);

//  DO:
//      get the caller-save registers in X86 machines.
//  RETURN:
//      return caller-save registers:
//          EAX, ECX, EDX
myTempList Frame_callerSaveRegs(void);

//  DO:
//      get the callee-save registers in X86 machines.
//  RETURN:
//      return caller-save registers:
//          EBX, EBP, ESI, EDI, ESP
myTempList Frame_calleeSaveRegs(void);

//  DO:
//      convert Tiger calling convention into C call convention. 
//  PARAMS:
//      str     the name of the external function.
//      args    the arguments to be passed.
IR_myExp Frame_externalCall(myString str, IR_myExpList args);

//  todo:   implemented in later phases.
IR_myStatement Frame_procEntryExit1(myFrame frame, IR_myStatement statement);

#endif
