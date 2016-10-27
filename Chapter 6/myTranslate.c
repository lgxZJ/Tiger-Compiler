#include "myTranslate.h"

#include <assert.h> //  for assert()
#include <stdlib.h> //  for NULL macro

/////////////////////////////////////////////////////////////////
//                          macros
#define NONE_LEVEL                  NULL

/////////////////////////////////////////////////////////////////
//                          structures

struct Trans_myAccess_
{
    Trans_myLevel   level;
    myAccess        access;
};

struct Trans_myLevel_
{
    myFrame             frame;
    Trans_myAccessList  formals;
};


///////////////////////////////////////////////////////////////////
//                           private functions

static Trans_myAccess Trans_makeAccess(Trans_myLevel level, myAccess access)
{
    Trans_myAccess accessInLevel =
        makeMemoryBlock(sizeof(*accessInLevel), MEMORY_TYPE_NONE);
    assert (accessInLevel);

    accessInLevel->access = access;
    accessInLevel->level = level;
    return accessInLevel;
}

static Trans_myAccessList Trans_convertAccessFromFrameToLevel(
    myAccessList accessesInFrame, Trans_myLevel level)
{
    Trans_myAccessList result = Trans_makeAccessList(NULL, NULL);

    Trans_myAccessList accessesInLevel = result;
    while (accessesInFrame)
    {
        Trans_myAccess accessInLevel =
            Trans_makeAccess(level, accessesInFrame->head);

        accessesInLevel->head=  accessInLevel;
        accessesInFrame->tail == NULL ?
            (0) :
            (accessesInLevel->tail = Trans_makeAccessList(NULL, NULL));

        accessesInFrame = accessesInFrame->tail;
        accessesInLevel = accessesInLevel->tail;
    }
    return result;
}

/////////////////////////////////////////////////////////////////////
//                      public functions

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

///////////////////////////////

int Trans_getAccessListCount(Trans_myAccessList list)
{
    int count = 0;
    while (list)
    {
        ++count;
        list = list->tail;
    }
    return count;
}

//////////////////////////////

Trans_myLevel Trans_getAccessLevel(Trans_myAccess access)
{
    return access->level;
}

////////////////////////////////

bool Trans_isLevelEqual(Trans_myLevel lhs, Trans_myLevel rhs)
{
    return lhs == rhs;
}

////////////////////////////////

static Trans_myLevel g_outermostLevel = NULL;

Trans_myLevel Trans_outermostLevel(void)
{
    if (g_outermostLevel == NULL)
    {
        g_outermostLevel =
            makeMemoryBlock(sizeof(g_outermostLevel), MEMORY_TYPE_NONE);
        assert (g_outermostLevel);

        myLabel outermostLabel = Temp_newNamedLabel("outmost");
        myBoolList noFormalFlags = NULL;
        g_outermostLevel->frame = Frame_newFrame(outermostLabel, noFormalFlags);
        g_outermostLevel->formals = Trans_convertAccessFromFrameToLevel(
            Frame_getFormals(g_outermostLevel->frame), NONE_LEVEL);
    } 
    return g_outermostLevel;
}

bool Trans_isOutermostLevel(Trans_myLevel level)
{
    assert (g_outermostLevel != NULL);
    assert (level != NULL);
    return g_outermostLevel == level; 
}

////////////////////////////////

Trans_myLevel Trans_newLevel(
    Trans_myLevel parent, myLabel name, myBoolList formals)
{
    //  static link is treated as the fisrt formal which always escape
    myBoolList formalsAddStaticLink = Frame_makeBoolList(formals, true);
    myFrame frame = Frame_newFrame(name, formalsAddStaticLink);

    //  todo: assign value to the static link formal

    Trans_myLevel level = makeMemoryBlock(sizeof(*level), MEMORY_TYPE_NONE);
    assert (level);
    level->frame = frame;
    level->formals = Trans_convertAccessFromFrameToLevel(
        Frame_getFormals(frame), level);
    return level;
}

//////////////////////////////

Trans_myAccess Trans_allocateLocal(Trans_myLevel level, bool escapeFlag)
{
    myAccess frameAccess = Frame_allocateLocal(level->frame, escapeFlag);

    Trans_myAccess transAccess = makeMemoryBlock(sizeof(*transAccess), MEMORY_TYPE_NONE);
    assert (transAccess);

    transAccess->access = frameAccess;
    transAccess->level = level;
    return transAccess;
}

//////////////////////////////

Trans_myAccessList Trans_getFormals(Trans_myLevel level)
{
    if (Trans_isOutermostLevel(level))
        return NULL;
    else
        //  skip the static link formal
        return level->formals->tail;
}

