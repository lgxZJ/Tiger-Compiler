%{

  #include "stdio.h"
  #include "y.tab.h"
  #include "myReport.h"
  extern FILE* yyin;
  extern char* yytext;
  extern PosInfo	g_posInfo;

%}

%token	TYPE	ARRAY	OF	ID	INT	STRING	VAR	FUNCTION
%token	NIL	LET	IN	END	STRING_LITERAL	INTEGER_LITERAL
%token	IF	THEN	ELSE	WHILE	DO	FOR	TO	BREAK
%token	LEFT_CURLY_BRACE	RIGHT_CURLY_BRACE	COLON	COMMA
%token	LEFT_PARENTHESIS	RIGHT_PARENTHESIS	DOT	SEMICOLON
%token	LEFT_BRACKET		RIGHT_BRACKET


%right	ASSIGNMENT_SIGN
%left	LOGICAL_AND	LOGICAL_OR
%nonassoc	EQUAL_SIGN	NOT_EQUAL_SIGN	LESS_SIGN	GREATER_SIGN	LESS_EQUAL_SIGN	GREATER_EQUAL_SIGN
%left	ADD_SIGN	MINUS_SIGN
%left	MULTIPLY_SIGN	DIVIDE_SIGN



%start	program

%%

decs		: dec decs
		|
		;

dec		: tydec
		| vardec
		| fundec
		;

tydec		: TYPE build_in_type EQUAL_SIGN ID
		| TYPE ID EQUAL_SIGN ID
		| TYPE build_in_type EQUAL_SIGN ty
		| TYPE ID EQUAL_SIGN ty
		;

build_in_type	: INT
		| STRING
		;

/*type_id		: ID 
		| build_in_type
		;*/

ty		: build_in_type
		| LEFT_CURLY_BRACE tyfields RIGHT_CURLY_BRACE
		| ARRAY OF build_in_type
		| ARRAY OF ID
		;
tyfields	: ID COLON build_in_type tyfields_aux
		| ID COLON ID tyfields_aux
		|
		;
tyfields_aux	: COMMA ID COLON build_in_type tyfields_aux
		| COMMA ID COLON ID tyfields_aux
		|
		;

vardec		: VAR ID ASSIGNMENT_SIGN exp
		| VAR ID COLON build_in_type ASSIGNMENT_SIGN exp
		| VAR ID COLON ID ASSIGNMENT_SIGN exp
		;

fundec		: FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS EQUAL_SIGN exp
		| FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS COLON build_in_type EQUAL_SIGN exp
		| FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS COLON ID EQUAL_SIGN exp
		;

/*l_value		: ID
		| l_value DOT ID
		| l_value LEFT_BRACKET exp RIGHT_BRACKET
		;*/

l_value		: ID l_value_aux
     		;
l_value_aux	: DOT ID l_value_aux
		| LEFT_BRACKET exp RIGHT_BRACKET l_value_aux
     		|
     		;

function_call	: ID LEFT_PARENTHESIS RIGHT_PARENTHESIS
		| ID LEFT_PARENTHESIS exp function_call_aux RIGHT_PARENTHESIS
		;
function_call_aux	: COMMA exp function_call_aux
			|
			;

nil		: NIL
		;

integer_literal	: INTEGER_LITERAL
		;
string_literal	: STRING_LITERAL
		;

array_creation	: build_in_type LEFT_BRACKET exp RIGHT_BRACKET OF exp
		| ID LEFT_BRACKET exp RIGHT_BRACKET OF exp
		;

record_creation	: build_in_type LEFT_CURLY_BRACE ID EQUAL_SIGN exp record_creation_aux RIGHT_CURLY_BRACE
		| ID LEFT_CURLY_BRACE ID EQUAL_SIGN exp record_creation_aux RIGHT_CURLY_BRACE
		| build_in_type LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
		| ID LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
		;
record_creation_aux	: COMMA ID EQUAL_SIGN exp record_creation_aux
			|
			;

arithmetic	: exp ADD_SIGN exp
		| exp MINUS_SIGN exp
		| exp MULTIPLY_SIGN exp
		| exp DIVIDE_SIGN exp
		;

parentheses	: LEFT_PARENTHESIS exp RIGHT_PARENTHESIS
		;

no_value	: LEFT_PARENTHESIS  RIGHT_PARENTHESIS
		;

sequencing	: LEFT_PARENTHESIS exp SEMICOLON exp sequencing_aux RIGHT_PARENTHESIS
		;
sequencing_aux	: SEMICOLON exp sequencing_aux
		|
		;

for		: FOR ID ASSIGNMENT_SIGN exp TO exp DO exp
		;

if_then_else	: IF exp THEN exp ELSE exp
		;

if_then		: IF exp THEN exp
		;

comparison	: exp EQUAL_SIGN exp
		| exp NOT_EQUAL_SIGN exp
		| exp LESS_SIGN exp
		| exp LESS_EQUAL_SIGN exp
		| exp GREATER_SIGN exp
		| exp GREATER_EQUAL_SIGN exp
		;

boolean_operate	: exp LOGICAL_AND exp
		| exp LOGICAL_OR exp
		;

assignment	: l_value ASSIGNMENT_SIGN exp
		;

let		: LET decs IN expseq END
		;
expseq		: exp let_aux
		|
		;
let_aux		: SEMICOLON exp let_aux
		|
		;

while		: WHILE exp DO exp
		;

negation	: MINUS_SIGN exp %prec DIVIDE_SIGN
		;

break		: BREAK
		;

exp		: l_value
		| function_call
		| nil
		| integer_literal
		| string_literal
		| array_creation
		| record_creation
		| arithmetic
		| parentheses
		| no_value
		| sequencing
		| for
		| if_then_else
		| if_then
		| comparison
		| boolean_operate
		| assignment
		| let
		| while
		| negation
		| break
		;

program		: exp;

%%

int main (int argc, char* argv[])
{
  if (argc != 2)
  {
    printf("Usage a.out filename\n");
    exit(1);
  }
  else
  {
    yyin = fopen(argv[1], "r");
  }

  int result = yyparse();
  switch (result)
  {
  case 0:	printf("parse succeed!\n");				break;
  case 1:	printf("invalid input, possible wrong syntax.\n");	break;
  case 2:	printf("memory exhaustion!\n");				break;
  }

  return 0;
}

void yyerror()
{
  printf("parse error:%s\tline:%i\tcolunm:%i\n",
     yytext, g_posInfo.line, g_posInfo.column);
}
 
