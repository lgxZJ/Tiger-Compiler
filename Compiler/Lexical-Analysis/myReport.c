#include "myReport.h"
#include "myTokens.h"
#include "stdio.h"

extern char* yytext;

//	initial to zero
PosInfo	g_posInfo = { 1, 0 };

void resetPos()
{
  g_posInfo.line = 1;
  g_posInfo.column = 0;
}

void recordTokenPos (bool newLine, int textLength)
{
  if (newLine)	
    g_posInfo.line += 1,
    g_posInfo.column = 0;

  g_posInfo.column += textLength;
}

PosInfo getCurrentPosInfo ()
{
  return g_posInfo;
}

//	@param	optionalText	a pointer from yytext()
//	if nullptr, ignore
//	caller make sures pointer parameter is valid
int outputReport(int tokenNum, int optionalValue, char* optionalText)
{
  if (tokenNum <= 0 || tokenNum > 47)	return -1;

  //	a little bit fat -_-
  char*		tokenStrings = NULL;
  switch (tokenNum)
  {
  case INTEGER_LITERAL:		tokenStrings = "INTEGER_LITERAL";	break;
  case ID:			tokenStrings = "ID";			break;
  case STRING_LITERAL:		tokenStrings = "STRING_LITERAL";	break;
  case INT:			tokenStrings = "int";			break;
  case STRING:			tokenStrings = "string";		break;
  case TYPE:			tokenStrings = "type";			break;
  case ARRAY:			tokenStrings = "array";			break;
  case OF:			tokenStrings = "of";			break;
  case VAR:			tokenStrings = "var";			break;
  case FUNCTION:		tokenStrings = "function";		break;
  case NIL:			tokenStrings = "nil";			break;
  case LET:			tokenStrings = "let";			break;
  case IN:			tokenStrings = "in";			break;
  case END:			tokenStrings = "end";			break;
  case IF:			tokenStrings = "if";			break;
  case THEN:			tokenStrings = "then";			break;
  case ELSE:			tokenStrings = "else";			break;
  case WHILE:			tokenStrings = "while";			break;
  case DO:			tokenStrings = "do";			break;
  case FOR:			tokenStrings = "for";			break;
  case TO:			tokenStrings = "to";			break;
  case BREAK:			tokenStrings = "break";			break;
  case LEFT_CURLY_BRACE:	tokenStrings = "{";			break;
  case RIGHT_CURLY_BRACE:	tokenStrings = "}";			break;
  case COLON:			tokenStrings = ":";			break;
  case COMMA:			tokenStrings = ",";			break;
  case LEFT_PARENTHESIS:	tokenStrings = "(";			break;
  case RIGHT_PARENTHESIS:	tokenStrings = ")";			break;
  case DOT:			tokenStrings = ".";			break;
  case SEMICOLON:		tokenStrings = ";";			break;
  case LEFT_BRACKET:		tokenStrings = "[";			break;
  case RIGHT_BRACKET:		tokenStrings = "]";			break;
  case ASSIGNMENT_SIGN:		tokenStrings = ":=";			break;
  case LOGICAL_AND:		tokenStrings = "&";			break;
  case LOGICAL_OR:		tokenStrings = "|";			break;
  case EQUAL_SIGN:		tokenStrings = "=";			break;
  case NOT_EQUAL_SIGN:		tokenStrings = "<>";			break;
  case GREATER_SIGN:		tokenStrings = ">";			break;
  case GREATER_EQUAL_SIGN:	tokenStrings = ">=";			break;
  case LESS_SIGN:		tokenStrings = "<";			break;
  case LESS_EQUAL_SIGN:		tokenStrings = "<=";			break;
  case ADD_SIGN:		tokenStrings = "+";			break;
  case MINUS_SIGN:		tokenStrings = "-";			break;
  case DIVIDE_SIGN:		tokenStrings = "/";			break;
  case MULTIPLY_SIGN:		tokenStrings = "*";			break;
  }

  int	number = 0, ret = 0;

  number += fprintf (stdout, "Token:\t%s\n", tokenStrings);
  if (tokenNum == INTEGER_LITERAL)
  {
    ret = fprintf (stdout, "\tValue:\t%i\n", optionalValue);
    if (ret < 0)	return -1;
    else		number += ret;
  }
  if (optionalText)
  {
    ret =  fprintf (stdout, "\tText:\t%s\n", optionalText);
    if (ret < 0)	return -1;
    else		number += ret;
  }
  
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
