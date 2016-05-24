%{

#include "myTokens.h"
#include "myReport.h"

%}

%option noyywrap
%x C_COMMENT

valid_string_line	[\]a-zA-Z0-9~`!@#$%^&*()_+=|{}\[,./<>?;': -]|\\\\|\\\"|\\n|\\t|\t|(\\^(@|[A-Z]|\[|\\\\|\]|^|_))|\\(0[0-7]{2}|1[0-7][0-7])

%%


while		{ recordTokenPos(false, yyleng);	return WHILE; }
for		{ recordTokenPos(false, yyleng);	return FOR; }
to		{ recordTokenPos(false, yyleng);	return TO; }
break		{ recordTokenPos(false, yyleng);	return BREAK; }
let		{ recordTokenPos(false, yyleng);	return LET; }
in		{ recordTokenPos(false, yyleng);	return IN; }
end		{ recordTokenPos(false, yyleng);	return END; }
function	{ recordTokenPos(false, yyleng);        return FUNCTION; }
var		{ recordTokenPos(false, yyleng);        return VAR; }
type		{ recordTokenPos(false, yyleng);        return TYPE; }
array		{ recordTokenPos(false, yyleng);        return ARRAY; }
if		{ recordTokenPos(false, yyleng);        return IF; }
then		{ recordTokenPos(false, yyleng);        return THEN; }
else		{ recordTokenPos(false, yyleng);        return ELSE; }
do		{ recordTokenPos(false, yyleng);        return DO; }
of		{ recordTokenPos(false, yyleng);        return OF; }
nil		{ recordTokenPos(false, yyleng);        return NIL; }
 
int             { recordTokenPos(false, yyleng);        return INT; }   
string          { recordTokenPos(false, yyleng);        return STRING; }
","		{ recordTokenPos(false, yyleng);        return COMMA; }
":"		{ recordTokenPos(false, yyleng);        return COLON; }
";"		{ recordTokenPos(false, yyleng);        return SEMICOLON; }
"("		{ recordTokenPos(false, yyleng);        return LEFT_PARENTHESIS; }
")"		{ recordTokenPos(false, yyleng);        return RIGHT_PARENTHESIS; }
"["		{ recordTokenPos(false, yyleng);        return LEFT_BRACKET; }
"]"		{ recordTokenPos(false, yyleng);        return RIGHT_BRACKET; }
"{"		{ recordTokenPos(false, yyleng);        return LEFT_CURLY_BRACE; }
"}"		{ recordTokenPos(false, yyleng);        return RIGHT_CURLY_BRACE; }
"."		{ recordTokenPos(false, yyleng);        return DOT; }
"+"		{ recordTokenPos(false, yyleng);        return ADD_SIGN; }
"-"		{ recordTokenPos(false, yyleng);        return MINUS_SIGN; }
"*"		{ recordTokenPos(false, yyleng);        return MULTIPLY_SIGN; }
"/"		{ recordTokenPos(false, yyleng);        return DIVIDE_SIGN; }
"="		{ recordTokenPos(false, yyleng);        return EQUAL_SIGN; }
"<>"		{ recordTokenPos(false, yyleng);        return NOT_EQUAL_SIGN; }
"<"		{ recordTokenPos(false, yyleng);        return LESS_SIGN; }
"<="		{ recordTokenPos(false, yyleng);        return LESS_EQUAL_SIGN; }
">"		{ recordTokenPos(false, yyleng);        return GREATER_SIGN; }
">="		{ recordTokenPos(false, yyleng);        return GREATER_EQUAL_SIGN; }
"&"		{ recordTokenPos(false, yyleng);        return LOGICAL_AND; }
"|"		{ recordTokenPos(false, yyleng);        return LOGICAL_OR; }
":="		{ recordTokenPos(false, yyleng);        return ASSIGNMENT_SIGN; }


[a-zA-Z][a-zA-Z0-9_]*	{ recordTokenPos(false, yyleng);        return ID; }
[0-9]+			{ recordTokenPos(false, yyleng);        return INTEGER_LITERAL; }
 /*	under Windows, use below	*/
 /*\"({valid_string_line}|\\[\t ]*\r\n[\t ]*\\)*\"	{ recordTokenPos(false, yyleng);*/
 /*	under Linux, use below	*/
\"({valid_string_line}|\\[\t ]*\n[\t ]*\\)*\"	{ recordTokenPos(false, yyleng);        return STRING_LITERAL; }

"/*"                   	{ BEGIN(C_COMMENT); }
<C_COMMENT>"*/"		{ BEGIN(INITIAL); }
<C_COMMENT>\n           { recordTokenPos(true, yyleng); } 
<C_COMMENT>.            { recordTokenPos(false, yyleng); }

[ \t\f]			{ recordTokenPos(false, yyleng); }
 /*	under linux, use the line below to detect newline	*/
\n	       		{ recordTokenPos(true, yyleng); }
 /*	under window, use the line below to detect newline	*/
 /*\r\n	       		{ recordTokenPos(true, yyleng); }*/
.			{ recordTokenPos(false, yyleng); errorReport(); }
