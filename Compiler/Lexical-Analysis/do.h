#ifndef DO_LEXICAL_H
#define DO_LEXICAL_H

#include <stdbool.h>

//  PARAM:
//      filename    a valid filename.
//  RETURN:
//      return true if lexical analyzing succeed; otherwise false.
//  REMARK:
//      it's an unckecked runtime error to pass a invalid filename.
bool doLexical(char* filename);

#endif
