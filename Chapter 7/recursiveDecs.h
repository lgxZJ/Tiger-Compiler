#ifndef RECURSIVE_DECS_H
#define RECURSIVE_DECS_H

#include "myTypes.h"
#include "myAbstractSyntax.h"
#include "myIRTree.h"

///
/// \brief      apply recursive semantic analysis on declarations.
/// \param[in]  decs        a list of declarations.
/// \param[out] resultPtr   a IR_myStatement variable pointer to receive results.
/// \return     a bool indicates whether analysis succeed.
///
bool MySemantic_Decs_Recursive(myDecList decs, IR_myStatement* resultPtr);

#endif