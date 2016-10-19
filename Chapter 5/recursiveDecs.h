#ifndef RECURSIVE_DECS_H
#define RECURSIVE_DECS_H

#include "myTypes.h"
#include "myAbstractSyntax.h"

bool MySemantic_Decs_Recursive(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs);

#endif