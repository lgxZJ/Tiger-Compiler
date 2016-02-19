#include "myReport.h"
#include "y.tab.h"
#include "stdio.h"

extern char* yytext;

//	initial to zero
PosInfo	g_posInfo = { 1, 0 };

void recordTokenPos (bool newLine, int textLength)
{
  g_posInfo.column += textLength;

  if (newLine)	
    g_posInfo.line += 1,
    g_posInfo.column = 0;
}

PosInfo getCurrentPosInfo ()
{
  return g_posInfo;
}

//	@param	optionalText	a pointer from yytext()
//	if nullptr, ignore
int outputReport(int tokenNum, int optionalValue, char* optionalText)
{
  char*		tokenStrings[] = {
    "while",	"for",	"to",	"break",	"let",
    "in",	"end",	"function",	"var",	"type",
    "array",	"if",	"then",	"else",	"do",	"of",
    "nil",

    ",",	":",	";",	"(",	")",	"[",
    "]",	"{",	"}",	".",	"+",	"-",
    "*",	"/",	"=",	"<>",	"<",	"<=",
    ">",	">=",	"&",	"|",	":=",

    "new-line",

    "ID",	"COMMENT",	"INTEGER_LITERAL",	"STRING_LITERAL",

    "int",	"string"
  };

  int	number = 0;

  number += fprintf (stdout, "Token:\t%s\n", tokenStrings[tokenNum - 1]);
  if (tokenNum == INTEGER_LITERAL)
    number += fprintf (stdout, "\tValue:\t%i\n", optionalValue);
  if (optionalText)
    number +=  fprintf (stdout, "\tText:\t%s\n", optionalText);
  
  return number;
}

int errorReport ()
{
  return fprintf (stdout, 
		  "Lexcial analysis has encountered an error!\n"
		  "\tAt source----line:\t%4i\tcolumn:\t%4i\n\tAt text:\t%s\n",
		  g_posInfo.line,
		  g_posInfo.column,
		  yytext);
}
