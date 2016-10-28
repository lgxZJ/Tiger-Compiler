#ifndef MY_ESCAPE_H
#define MY_ESCAPE_H

#include "myAbstractSyntax.h"

/// \brief      find and record escapes in the given expression.
/// \params[in] exp     the expression inside which to look for.
/// \return     this function has no return value.
/// \remark     after called, variable escape information is holded,
///             call Escape_isVarEscape() to get one.
void Escape_findEscape(myExp exp);

/// \brief      check if the given variable is escaped.
/// \params[in] varSymbol   the symbol of a variable.
/// \return     a bool.
bool Escape_isVarEscaped(mySymbol varSymbol);

#endif