%{

#include "y.tab.h"
#include "myReport.h"

%}

%option noyywrap
%x C_COMMENT

none-print-chars	[ \\t\\f]*
sting-chars		\\n|\\t|[ ]|[a-zA-Z0-9]*|\\\^[A-Z]|\\[0-9]{3}|\\\"|\\\\

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
\"(\\n|\\t|[ ]|[a-zA-Z0-9!"#$%&'()*+,.\/:;<=>?@\[\] ^_`{|}~-]*|\\\^[A-Z]|\\[0-9]{3}|\\\"|\\\\|\\[ \t\n]*\r\n[ \t\f]*\\)*\"	{ recordTokenPos(false, yyleng);        return STRING_LITERAL; }


"/*"                   	{ BEGIN(C_COMMENT); }
<C_COMMENT>"*/"		{ BEGIN(INITIAL); }
<C_COMMENT>\n           { recordTokenPos(true, yyleng); } 
<C_COMMENT>.            { recordTokenPos(false, yyleng); }

[ \t\f]			{ recordTokenPos(false, yyleng); }
\r\n	       		{ recordTokenPos(true, yyleng); }
.			{ recordTokenPos(false, yyleng); errorReport(); }
