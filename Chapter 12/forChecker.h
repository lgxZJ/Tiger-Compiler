#ifndef FOR_CHECKER_H
#define FOR_CHECKER_H

///////////////////////////////////////////////////////////////
//
//     Check:
//          the loop variable can not be assigned in loop body.
//
//////////////////////////////////////////////////////////////

#include "mySymbol.h"

void enterForLoop(mySymbol loopVarName);
void leaveForLoop(void);
bool isInsideForLoop(void);
bool isLoopVarAssigned(mySymbol varNameAssigned);;

#endif