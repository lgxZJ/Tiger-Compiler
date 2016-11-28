#include "breakChecker.h"

//////////////////////////////////////////////////
//  global variables
static int g_outerLoopCount = 0;


/////////////////////////////////////////////////
//  functions
void enterLoop(void)
{
    g_outerLoopCount++;
}

void leaveLoop(void)
{
    g_outerLoopCount--;
}

bool isBreakInsideLoop(void)
{
    return g_outerLoopCount;
}

void resetChecker(void)
{
    g_outerLoopCount = 0;
}