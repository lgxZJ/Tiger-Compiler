#include "myTokens.h"
#include "myReport.h"
#include "stdlib.h"
#include "stdio.h"

int yylex(void); /* prototype for the lexing function */
extern FILE* 	yyin;
extern int	yyleng;
extern char*	yytext;


//	return -1 for error, 0 for success
int oneParse(char* filename)
{
    static int count = 0;
    ++count;

    printf("-----------Begin Parse----------\n");
    printf("No.%i", count);

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

    printf("====All files passed!====\n");
    return 0;
}
