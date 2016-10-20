#include "breakChecker.h"

//////////////////////////////////////////////////
//  global variables
static bool g_isOuterScopeLoop = false;


/////////////////////////////////////////////////
//  functions
void enterLoop(void)
{
    g_isOuterScopeLoop = true;
}

void leaveLoop(void)
{
    g_isOuterScopeLoop = false;
}

bool isBreakInsideLoop(void)
{
    return g_isOuterScopeLoop;
}