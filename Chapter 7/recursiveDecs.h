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

///
/// \brief      add formals to the current scope.
/// \param[in]  funcName    the function name.
/// \param[in]  funcFormals the formals of the function.
///
/// \remark     this function appears here only to be used in myTranslate module.
/// \todo       remove this function to a commom file.     
void addFormalsToScope(mySymbol funcName, myTyFieldList funcFormals);

#endif