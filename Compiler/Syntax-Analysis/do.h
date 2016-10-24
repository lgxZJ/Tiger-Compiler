#ifndef DO_SYNTAX_H
#define DO_SYNTAX_H

#include "myAbstractSyntax.h"

//  PARAMS:
//      filename:   the name of file to parse.
//  RETURN:
//      if function succeed, it returns an AST(abstract syntax tree);
//      otherwise, it returns ABSTRACT_SYNTAX_ERROR.
//  REMARK:
//      it's an unchecked runtime error to pass an invalid filename.
myExp doSyntax(char* filename);

#endif
