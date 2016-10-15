#ifndef RECURSIVE_FILE_H
#define RECURSIVE_FILE_H

#include "myTypes.h"
#include "myAbstractSyntax.h"

bool MySemantic_Decs_Recursive(
    myTable varAndFuncEnv, myTable typeEnv, myDecList decs);

#endif