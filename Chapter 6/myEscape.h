#ifndef MY_ESCAPE_H
#define MY_ESCAPE_H

#include "myAbstractSyntax.h"

/// \brief      find and record escapes in the given expression.
/// \param[in] exp     the expression inside which to look for.
/// \return     this function has no return value.
/// \remark     after called, variable escape information is holded,
///             call Escape_isVarEscape() to get one.
void Escape_findEscape(myExp exp);

/// \brief      check if the given variable is escaped.
/// \param[in] varName   the name of a variable.
/// \return     a bool.
/// \remark     variables inside nested scopes can not be check any more!!
bool Escape_isVarEscaped(mySymbol varName);

/// \brief      get escape environment which is actually a table.
/// \param      this function has no parameters.
/// \return     the environment.
/// \remark     it's a checked runtime error when calling this before
///             any information is entered.
myTable Escape_getEscapeEnvironment(void);

/// \brief      set escape environment which is actually a table.
/// \param      escapeEnv   the environmet to set.
/// \return     this function has no return value.
/// \remark     it's a checked runtime error to pass a NULL environment.
void Escape_setEscapeEnvironment(myTable escapeEnv);

#endif