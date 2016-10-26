#include "myTranslate.h"

#include <assert.h> //  for assert()
#include <stdlib.h> //  for NULL macro

/////////////////////////////////////////////////////////////////
//                          structures

struct Trans_myAccess_
{
    Trans_myLevel   level;
    myAccess        access;
};


///////////////////////////////////////////////////////////////////
//                          functions

Trans_myAccessList Trans_makeAccessList(
    Trans_myAccess head, Trans_myAccessList tail)
{
    Trans_myAccessList accessList =
        makeMemoryBlock(sizeof(*accessList), MEMORY_TYPE_NONE);
    assert (accessList);

    accessList->head = head;
    accessList->tail = tail;
    return accessList;
}

////////////////////////////////

bool Trans_isLevelEqual(Trans_myLevel lhs, Trans_myLevel rhs)
{
    return Frame_isFrameEqual(lhs, rhs);
}

////////////////////////////////

static Trans_myLevel g_outermostLevel = NULL;

Trans_myLevel Trans_outermostLevel(void)
{
    if (g_outermostLevel == NULL)
    {
        myLabel outermostLabel = Temp_newNamedLabel("outmost");
        myBoolList noFormalFlags = NULL;
        g_outermostLevel = Frame_newFrame(outermostLabel, noFormalFlags);
    } 
    return g_outermostLevel;
}

////////////////////////////////

