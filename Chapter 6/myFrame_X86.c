#include "myFrame.h"

#include <assert.h> //  for assert()
#include <stdlib.h> //  for NULL macro

///////////////////////////////////////////////////////////////////
//                          macros
#define RETURN_COUNT()  int count = 0;          \
                                                \
                        while (list)            \
                        {                       \
                            ++count;            \
                            list = list->tail;  \
                        }                       \
                        return count;

#define BASE_SIZE       4       //  base size of each local var

///////////////////////////////////////////////////////////////////
//                          structures

struct myAccess_
{
    enum AccessKind { InFrame, InReg } kind;
    union
    {
        int     offset; //  offset comapred to frame pointer
        myTemp  reg;    //  register representation
    }u;
};

struct myFrame_
{
    myAccessList    formals;
    myString        viewShift;
    int             localCount;
    int             formalCount;
    myLabel         label;
};

/////////////////////////////////////////////////////////////////////
//                          functions

static myAccess makeInFrameAccess(int offset)
{
    //  4 is the value of BASE_SIZE
    //__asm__ __volatile__ ("subl $4, %esp\n\t");

    myAccess access = makeMemoryBlock(sizeof(*access), MEMORY_TYPE_NONE);
    assert (access);

    access->kind = InFrame;
    access->u.offset=  offset;
    return access;
}

static myAccess makeInRegAccess(myTemp reg)
{
    myAccess access = makeMemoryBlock(sizeof(*access), MEMORY_TYPE_NONE);
    assert (access);

    access->kind = InReg;
    access->u.reg = reg;
    return access;
}

/////////////////////////////////////////

bool Frame_isAccessInFrame(myAccess access)
{
    return access->kind == InFrame;
}

bool Frame_isAccessInReg(myAccess access)
{
    return access->kind == InReg;
}

/////////////////////////////////////////

myBoolList Frame_makeBoolList(myBoolList formers, bool newOne)
{
    myBoolList bools = makeMemoryBlock(sizeof(*bools), MEMORY_TYPE_NONE);
    assert(bools);

    bools->head = newOne;
    bools->tail = formers;
    return bools;
}

myAccessList Frame_makeAccessList(myAccess newOne, myAccessList formers)
{
    myAccessList accesses = makeMemoryBlock(sizeof(*accesses), MEMORY_TYPE_NONE);
    assert(accesses);

    accesses->head = newOne;
    accesses->tail = formers;
    return accesses;
}

int Frame_getBoolListCount(myBoolList list)
{
    RETURN_COUNT();
}

int Frame_getAccessListCount(myAccessList list)
{
    RETURN_COUNT();
}

/////////////////////////////////////////////

void fillFormalsFromFlags(myFrame frame, myBoolList formalFlags);
void fillViewShiftFromFlags(myFrame frame);

myFrame Frame_newFrame(myLabel frameLabel, myBoolList formalFlags)
{
    myFrame frame = makeMemoryBlock(sizeof(*frame), MEMORY_TYPE_NONE);
    assert (frame);

    frame->label = frameLabel;
    frame->localCount = 0;  //  not include formals
    fillFormalsFromFlags(frame, formalFlags);
    fillViewShiftFromFlags(frame);
    
    return frame;
}

void fillFormalsFromFlags(myFrame frame, myBoolList formalFlags)
{
    frame->formalCount = 0;
    if (formalFlags == NULL)
        frame->formals = NULL;
    else
        frame->formals = Frame_makeAccessList(NULL, NULL);

    //  use iterations to replace recursive function calls
    myAccessList accessList = frame->formals;
    while (formalFlags)
    {
        myAccess access;

        if (formalFlags->head)
            accessList->head = makeInFrameAccess(frame->formalCount++ * BASE_SIZE);
        else
            accessList->head = makeInRegAccess(Temp_newTemp());

        formalFlags->tail == NULL ?
            (0) :
            (accessList->tail = Frame_makeAccessList(NULL, NULL));
        accessList = accessList->tail;
        formalFlags = formalFlags->tail;
    }
}

void fillViewShiftFromFlags(myFrame frame)
{
    frame->viewShift = "movl %esp, %ebp\n\t";
}

/////////////////////////////////////

bool Frame_isFrameEqual(myFrame lhs, myFrame rhs)
{
    return lhs == rhs;
}

/////////////////////////////////////

myAccess Frame_allocateLocal(myFrame frame, bool escapeFlag)
{
    if (escapeFlag)
        return makeInFrameAccess(frame->localCount++ * BASE_SIZE);
    else    
        return makeInRegAccess(Temp_newTemp());
}

int Frame_getLocalCount(myFrame frame)
{
    assert (frame->localCount >= 0);
    return frame->localCount;
}

///////////////////////////////////

myAccessList Frame_getFormals(myFrame frame)
{
    return frame->formals;
}

////////////////////////////////////////////////////////////////////
//                      private functions
int Frame_getAccessOffset(myAccess access)
{
    assert (Frame_isAccessInFrame(access));
    return access->u.offset;
}