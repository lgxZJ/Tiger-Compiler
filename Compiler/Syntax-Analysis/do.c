#include "do.h"

#include <assert.h>

//  forwards in myTiger.y
myExp parseOneFile(char* parseFile);


myExp doSyntax(char* filename)
{
    return parseOneFile(filename);
}