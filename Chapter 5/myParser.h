#ifndef MY_PARSER_H
#define MY_PARSER_H

#include "myAbstractSyntax.h"

//  RETURN:
//      if succeeded, it returns a `myExp` structure.
//      if failed, it returns ABSTRACT_SYNTAX_ERROR.
//  REMARK:
//      if encountered any error while lexical analyzing, it reports
//          some information.
//      else it reports nothing.
myExp parseOneFile(char* parseFile);

#endif