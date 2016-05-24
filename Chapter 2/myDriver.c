/**
 *	@file	myDriver.c
 *	@author	lgxZJ@outlook.com
 *	@date	5/19/2016
 *
 *	@brief	A driven file for the whole little program.
 */

#include "myTokens.h"
#include "myReport.h"
#include "stdlib.h"
#include "stdio.h"

/**	@cond	*/

int yylex(void); /* prototype for the lexing function */
extern FILE* 	yyin;
extern int	yyleng;
extern char*	yytext;

/**	@endcond	*/

//	return -1 for error, 0 for success
/**
 *	@brief	Do token analyse on one file.
 *
 *			This function uses the function \b yylex() provided by 
 *			<a href="http://flex.sourceforge.net/">flex</a> to do
 *			real work.
 *
 *	@param	filename	The name of file to analyse.
 *	@return	If the function succeeds, the return value is 0.
 *			If the function fails, the return value is -1.
 *	
 *	@note	This function do no parameter checkings, the caller make sures
 *			the parameters passed in is valid.
 */
int oneParse(char* filename)
{
    static int count = 0;
    ++count;

    printf("-----------Begin Parse----------\n");
    printf("No.%i\n", count);

    yyin = fopen(filename, "r");
    if ((yyin == NULL))
    {
	printf("file open error!\n");
	return -1;
    }

    int tokenNum;
    while (tokenNum = yylex())
    {
	if (tokenNum == INTEGER_LITERAL)
	{
	    if(outputReport(tokenNum, atoi(yytext), NULL) == -1)
		goto error;
	}
	else if (tokenNum == ID || tokenNum == COMMENT || tokenNum == STRING_LITERAL)
	{
	    if(outputReport(tokenNum, 0, yytext) == -1)
		goto error;
	}
	else
	{ 
	    if(outputReport(tokenNum, 0, NULL) == -1)
		goto error;
	}
    }

    fclose(yyin);
    printf("---------------End Parse---------------\n\n");

    return 0;
error:
    return -1;
}

/**	@cond	*/

int main (int argc, char* argv[])
{
    printf("Useage a.out filename.\nDefault use files in testcases folder.\n\n");

    if (argc == 2)
    {
	if(oneParse(argv[1]) == -1)
	{
	    errorReport();
	    return -1;
	}
    }
    else//	use default files
    {
	char	filename[256] = {'\0'};
	for (unsigned i = 1; i < 50; ++i)
	{
	    sprintf(filename, "testcases/test%i.tig", i);

	    if (oneParse(filename) == -1)
	    {
		errorReport();
		return -1;
	    }
	}
    }

    printf("====All files parsed!====\n");
    return 0;
}

/**	@endcond	*/
