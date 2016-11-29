#include "forChecker.h"
#include "myEnvironment.h"

#include <stdlib.h>

///////////////////////////////////////////////////////////////////////
//                          globals
static myTable g_loopVarTable = NULL;
int    g_forLoopDepth = 0;

/////////////////////////////////////////////////////////////////////
//                          functions

void enterForLoop(mySymbol loopVarName)
{
    if (g_loopVarTable == NULL)
        g_loopVarTable = MySymbol_MakeNewTable();
    //  keep this var name
    ++g_forLoopDepth;
    MySymbol_Enter(g_loopVarTable, loopVarName, (void*)true);
}

void leaveForLoop(void)
{
    if (g_loopVarTable != NULL)
    {
        --g_forLoopDepth;
        MyTable_Pop_(g_loopVarTable);
    }
}

bool isInsideForLoop(void)
{
    return g_forLoopDepth != 0;
}

bool isLoopVarAssigned(mySymbol varNameAssigned)
{
    return MySymbol_Look(g_loopVarTable, varNameAssigned) != NULL;
}