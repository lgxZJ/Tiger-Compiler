#include "breakGetter.h"

#include <assert.h>

static myLabel g_labelArray[256];
static int g_curIndex = -1;

/////////////////////////////////////////////////////

void pushBreakTarget(myLabel targetLabel)
{
    assert (g_curIndex < 256);
    g_labelArray[++g_curIndex] = targetLabel;
}

void popBreakTarget(void)
{
    myLabel label = getBreakTarget();
    g_curIndex--;
}

myLabel getBreakTarget(void)
{
    assert (g_curIndex >= 0);
    return g_labelArray[g_curIndex];
}