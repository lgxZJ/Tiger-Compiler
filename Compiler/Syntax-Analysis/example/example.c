#include "../do.h"

#include <stdio.h>

int main()
{
    myExp ast = doSyntax("test.tig");
    if (ast == ABSTRACT_SYNTAX_ERROR)
        printf("Analysis error!\n");
    else
        printf("Analysis succeed!\n");
    return 0;
}
