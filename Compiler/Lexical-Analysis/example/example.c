#include "../do.h"

#include <stdio.h>

int main()
{
    bool result = doLexical("test.tig");

    if (result) printf("example succeed!\n");
    else        printf("example failed!\n");

    return 0;
}
