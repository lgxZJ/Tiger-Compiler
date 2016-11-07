#ifndef RECURSIVE_DECS_H
#define RECURSIVE_DECS_H

#include "myTypes.h"
#include "myAbstractSyntax.h"

///
/// \brief      apply recursive semantic analysis on declarations.
/// \param[in]  decs    a list of declarations.
/// \return     a bool indicates whether analysis succeed.
///
bool MySemantic_Decs_Recursive(myDecList decs);

#endif