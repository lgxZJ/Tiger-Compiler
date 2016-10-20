#include "do.h"

#include <stdio.h>  //  for FILE
#include <assert.h> //  for assert()

//-----------------------------forwards------------------------------------//

/* prototype for the lexing function(return 0 for error) */
int yylex(void);

extern FILE* 	yyin;
extern int	    yyleng;
extern char*	yytext;

extern bool     g_errorDetected;

//-----------------------------functions------------------------------------//

bool doLexical(char* filename)
{
    assert (filename);
    g_errorDetected = false;

    yyin = fopen(filename, "r");
    if (yyin == NULL)   return false;

    //  once an error occurs, it automatically reports ot.
    while (yylex())
        ;

    fclose(yyin);
    return !g_errorDetected;
}
