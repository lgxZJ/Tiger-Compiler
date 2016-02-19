#include "myTokens.h"
#include "myReport.h"
#include "stdlib.h"
#include "stdio.h"

int yylex(void); /* prototype for the lexing function */
extern FILE* 	yyin;
extern int	yyleng;
extern char*	yytext;

int main (int argc, char *argv[])
{
  if (argc != 2)	
    fprintf(stdout, "Usage: *.out source_file_name!\n"),
    exit(1);
  else
    yyin = fopen(argv[1], "r");

  if (yyin)
  {
    int tokenNum;
    while (tokenNum = yylex ())
    {
      if (tokenNum == INTEGER_LITERAL)
	outputReport(tokenNum, atoi(yytext), NULL);
      else if (tokenNum == ID || tokenNum == COMMENT || tokenNum == STRING_LITERAL)
	outputReport(tokenNum, NULL, yytext);
      else 
	outputReport(tokenNum, NULL, NULL);
    }
  }

  fclose(yyin);
  return 0;
}
