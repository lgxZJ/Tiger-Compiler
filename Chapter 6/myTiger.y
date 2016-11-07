%{

  #include "abstractSyntaxMaker.h"
  #include "myReport.h"
  #include "y.tab.h"
  #include <stdio.h>
  #include <stdlib.h>

  extern FILE* yyin;
  extern char* yytext;
  extern PosInfo g_posInfo;

  //used to hold global abstract syntax tree
  myExp		g_AbstractSyntaxTree;
  
  extern int yylex();//	prototype for lex scanning

%}

%union	{
	int				int_val;
	myString			myString_val;
	myExp				myExp_val;
	myDec				myDec_val;
	myDecList			myDecList_val;
	myTypeDec			myTypeDec_val;
	myVarDec			myVarDec_val;
	myFuncDec			myFuncDec_val;
	myTy				myTy_val;
	myTyFieldList			myTyFieldList_val;
	myLValueExp			myLValue_val;
	myLValueAux			myLValueAux_val;
	myFunctionCallExp		myFunctionCallExp_val;
	myExpList			myExpList_val;
	myNilExp			myNilExp_val;
	myIntegerLiteralExp		myIntegerLiteralExp_val;
	myStringLiteralExp		myStringLiteralExp_val;
	myArrayCreationExp		myArrayCreationExp_val;
	myRecordCreationExp		myRecordCreationExp_val;
	myRecordFieldList		myRecordFieldList_val;
	myArithmeticExp			myArithmeticExp_val;
	myParenthesesExp		myParenthesesExp_val;
	myNoValueExp			myNoValueExp_val;
	mySequencingExp			mySequencingExp_val;
	myForExp			myForExp_val;
	myIfThenElseExp			myIfThenElseExp_val;
	myIfThenExp			myIfThenExp_val;
	myComparisonExp			myComparisonExp_val;
	myBooleanOperateExp		myBooleanOperateExp_val;
	myAssignmentExp			myAssignmentExp_val;
	myLetExp			myLetExp_val;
	myWhileExp			myWhileExp_val;
	myNegationExp			myNegationExp_val;
	myBreakExp			myBreakExp_val;
}


%token	<int_val>		INTEGER_LITERAL
%token	<myString_val>		ID	STRING_LITERAL	INT	STRING

%token	TYPE	ARRAY	OF	INT	STRING	VAR	FUNCTION
%token	NIL	LET	IN	END
%token	IF	THEN	ELSE	WHILE	DO	FOR	TO	BREAK
%token	LEFT_CURLY_BRACE	RIGHT_CURLY_BRACE	COLON	COMMA
%token	LEFT_PARENTHESIS	RIGHT_PARENTHESIS	DOT	SEMICOLON
%token	LEFT_BRACKET		RIGHT_BRACKET


%right	ASSIGNMENT_SIGN
%left	LOGICAL_AND	LOGICAL_OR
%nonassoc	EQUAL_SIGN	NOT_EQUAL_SIGN	LESS_SIGN
%nonassoc	GREATER_SIGN	LESS_EQUAL_SIGN	GREATER_EQUAL_SIGN
%left	ADD_SIGN	MINUS_SIGN
%left	MULTIPLY_SIGN	DIVIDE_SIGN


%type	<myExp_val>	exp	program
		
%type 	<myDecList_val>	decs
		
%type	<myDec_val>		dec
		
%type	<myTypeDec_val>	tydec

%type	<myVarDec_val>	vardec

%type	<myFuncDec_val>	fundec
		
%type	<myString_val>		build_in_type

%type	<myTy_val>		ty

%type	<myTyFieldList_val>	tyfields	tyfields_aux

%type	<myVarDec_val>		vardec

%type	<myLValue_val>		l_value

%type	<myLValueAux_val>	l_value_aux

%type	<myFunctionCallExp_val>	function_call

%type	<myExpList_val>		function_call_aux	sequencing_aux	expseq	let_aux

%type	<myNilExp_val>			nil

%type	<myIntegerLiteralExp_val>	integer_literal

%type	<myStringLiteralExp_val>	string_literal

%type	<myArrayCreationExp_val>	array_creation

%type	<myRecordCreationExp_val>	record_creation

%type	<myRecordFieldList_val>		record_creation_aux

%type	<myArithmeticExp_val>		arithmetic

%type	<myParenthesesExp_val>		parentheses

%type	<myNoValueExp_val>		no_value

%type	<mySequencingExp_val>	sequencing	
	
%type	<myForExp_val>		for
	
%type	<myIfThenElseExp_val>	if_then_else

%type	<myIfThenExp_val>		if_then

%type	<myComparisonExp_val>	comparison

%type	<myBooleanOperateExp_val>	boolean_operate

%type	<myAssignmentExp_val>	assignment

%type	<myLetExp_val>	let

%type	<myWhileExp_val>	while

%type	<myNegationExp_val>	negation

%type	<myBreakExp_val>	break

	
%start	program

%%

decs	: dec decs		{ $$ = makeMyDecList($1, $2); }
	|			{ $$ = (myDecList)NULL; }	
	;

dec	: tydec		{ $$ = makeMyDec_Type(getCurrentPosInfo(), $1); }
	| vardec	{ $$ = makeMyDec_Var(getCurrentPosInfo(), $1); }
	| fundec	{ $$ = makeMyDec_Func(getCurrentPosInfo(), $1); }
	;

tydec	: TYPE build_in_type EQUAL_SIGN ID
		{ $$ = makeMyTyDec(
			MySymbol_MakeSymbol($2),
			makeMyTy_Named(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol($4)));
		}
	| TYPE ID EQUAL_SIGN ID
		{ $$ = makeMyTyDec(
			MySymbol_MakeSymbol($2),
			    makeMyTy_Named(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol($4)));
		}
	| TYPE build_in_type EQUAL_SIGN ty
		{ $$ = makeMyTyDec(
			MySymbol_MakeSymbol($2),
			$4);
		}
	| TYPE ID EQUAL_SIGN ty
		{ $$ = makeMyTyDec(
			MySymbol_MakeSymbol($2),
			$4);
		}
	;

build_in_type	: INT			{ $$ = "int"; }
		| STRING		{ $$ = "string"; }
		;

/*type_id		: ID 
		| build_in_type
		;*/

ty	: build_in_type
		{ $$ = makeMyTy_Named(
			getCurrentPosInfo(),
		    	MySymbol_MakeSymbol($1));
		}
	| LEFT_CURLY_BRACE tyfields RIGHT_CURLY_BRACE
		{ $$ = makeMyTy_Record(
			getCurrentPosInfo(),
			$2);
		}
	| ARRAY OF build_in_type
		{ $$ = makeMyTy_Array(
			getCurrentPosInfo(),
			MySymbol_MakeSymbol($3));
		}
	| ARRAY OF ID
		{ $$ = makeMyTy_Array(
			getCurrentPosInfo(),
			MySymbol_MakeSymbol($3));
		}
	;
tyfields	: ID COLON build_in_type tyfields_aux
			{ $$ = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol($1),
					MySymbol_MakeSymbol($3)),
				$4);
			}
		| ID COLON ID tyfields_aux
			{ $$ = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol($1),
					MySymbol_MakeSymbol($3)),
				$4);
			}
		|	{ $$ = (myTyFieldList)NULL; }
		;
tyfields_aux	: COMMA ID COLON build_in_type tyfields_aux
			{ $$ = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol($2),
					MySymbol_MakeSymbol($4)),
				$5);
			}
		| COMMA ID COLON ID tyfields_aux
			{ $$ = makeMyTyFieldList(
				makeMyTyField(
					MySymbol_MakeSymbol($2),
					MySymbol_MakeSymbol($4)),
				$5);
			}
		|	{ $$ = (myTyFieldList)NULL; }
		;

vardec	: VAR ID ASSIGNMENT_SIGN exp
		{ $$ = makeMyVarDec_ShortForm(
			makeMyShortFormVar(
				MySymbol_MakeSymbol($2),
				$4));
		}
	| VAR ID COLON build_in_type ASSIGNMENT_SIGN exp
		{ $$ = makeMyVarDec_LongForm(
			makeMyLongFormVar(
				MySymbol_MakeSymbol($2),
				MySymbol_MakeSymbol($4),
				$6));
		}
	| VAR ID COLON ID ASSIGNMENT_SIGN exp
		{ $$ = makeMyVarDec_LongForm(
			makeMyLongFormVar(
				MySymbol_MakeSymbol($2),
				MySymbol_MakeSymbol($4),
				$6));
		}
	;

fundec	: FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS EQUAL_SIGN exp
		{ $$ = makeMyFuncDec_Procedure(
			makeMyProcedureDec(
				MySymbol_MakeSymbol($2),
				$4,
				$7));
		}
	| FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS COLON build_in_type EQUAL_SIGN exp
		{ $$ = makeMyFuncDec_Function(
			makeMyFunctionDec(
				MySymbol_MakeSymbol($2),
				$4,
				MySymbol_MakeSymbol($7),
				$9));
		}
	| FUNCTION ID LEFT_PARENTHESIS tyfields RIGHT_PARENTHESIS COLON ID EQUAL_SIGN exp
		{ $$ = makeMyFuncDec_Function(
			makeMyFunctionDec(
				MySymbol_MakeSymbol($2),
				$4,
				MySymbol_MakeSymbol($7),
				$9));
		}
	;

/*l_value		: ID
		| l_value DOT ID
		| l_value LEFT_BRACKET exp RIGHT_BRACKET
		;*/

l_value		: ID l_value_aux
			{ $$ = makeMyLValue(
				getCurrentPosInfo(),
				MySymbol_MakeSymbol($1),
				$2);
			}
     		;
l_value_aux	: DOT ID l_value_aux
			{ $$ = makeMyLValueAux(
				MySymbol_MakeSymbol($2),
				(myExp)NULL,
				$3);
			}
		| LEFT_BRACKET exp RIGHT_BRACKET l_value_aux
			{ $$ = makeMyLValueAux(
				(mySymbol)NULL,
				$2,
				$4);
			}
     		|	{ $$ = (myLValueAux)NULL; }
     		;

function_call	: ID LEFT_PARENTHESIS RIGHT_PARENTHESIS
			{ $$ = makeMyFunctionCallExp_NoParam(
				makeMyNoParamFunctionCallExp(
					MySymbol_MakeSymbol($1)));
			}
		| ID LEFT_PARENTHESIS exp function_call_aux RIGHT_PARENTHESIS
			{ $$ = makeMyFunctionCallExp_Param(
				makeMyParamFunctionCallExp(
					MySymbol_MakeSymbol($1),
					makeMyExpList(
						$3, $4)));
			}
		;
function_call_aux	: COMMA exp function_call_aux
				{ $$ = makeMyExpList($2, $3); }
			|	{ $$ = (myExpList)NULL; }
			;

nil		: NIL		{ $$ = makeMyNilExp(); }
		;

integer_literal	: INTEGER_LITERAL	{ $$ = makeMyIntegerLiteralExp($1); }
		;
string_literal	: STRING_LITERAL	{ $$ = makeMyStringLiteralExp($1); }
		;

array_creation	: build_in_type LEFT_BRACKET exp RIGHT_BRACKET OF exp
			{ $$ = makeMyArrayCreationExp(
				MySymbol_MakeSymbol($1),
				$3,
				$6);
			}
		| ID LEFT_BRACKET exp RIGHT_BRACKET OF exp
			{ $$ = makeMyArrayCreationExp(
				MySymbol_MakeSymbol($1),
				$3,
				$6);
			}
		;

record_creation	: build_in_type LEFT_CURLY_BRACE ID EQUAL_SIGN exp record_creation_aux RIGHT_CURLY_BRACE
			{ $$ = makeMyRecordCreationExp_Field(
				makeMyFieldRecordCreationExp(
					MySymbol_MakeSymbol($1),
					makeMyRecordFieldList(
						makeMyRecordField(
							MySymbol_MakeSymbol($3),
							$5),
							$6)));
			}
		| ID LEFT_CURLY_BRACE ID EQUAL_SIGN exp record_creation_aux RIGHT_CURLY_BRACE
			{ $$ = makeMyRecordCreationExp_Field(
				makeMyFieldRecordCreationExp(
					MySymbol_MakeSymbol($1),
					makeMyRecordFieldList(
						makeMyRecordField(
							MySymbol_MakeSymbol($3),
							$5),
							$6)));
			}
		| build_in_type LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
			{ $$ = makeMyRecordCreationExp_NoField(
				makeMyNoFieldRecordCreationExp(
					MySymbol_MakeSymbol($1)));
			}
		| ID LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
			{ $$ = makeMyRecordCreationExp_NoField(
				makeMyNoFieldRecordCreationExp(
					MySymbol_MakeSymbol($1)));
			}
		;
record_creation_aux	: COMMA ID EQUAL_SIGN exp record_creation_aux
				{ $$ = makeMyRecordFieldList(
					makeMyRecordField(
						MySymbol_MakeSymbol($2),
						$4),
					$5); }
			|	{ $$ = (myRecordFieldList)NULL; }
			;

arithmetic	: exp ADD_SIGN exp	{ $$ = makeMyArithmeticExp_Plus($1, $3); }
		| exp MINUS_SIGN exp	{ $$ = makeMyArithmeticExp_Minus($1, $3); }
		| exp MULTIPLY_SIGN exp	{ $$ = makeMyArithmeticExp_Multiply($1, $3); }
		| exp DIVIDE_SIGN exp	{ $$ = makeMyArithmeticExp_Divide($1, $3); }
		;

parentheses	: LEFT_PARENTHESIS exp RIGHT_PARENTHESIS
			{ $$ = makeMyParenthesesExp($2); }
		;

no_value	: LEFT_PARENTHESIS  RIGHT_PARENTHESIS
			{ $$ = makeMyNoValueExp(); }
		;

sequencing	: LEFT_PARENTHESIS exp SEMICOLON exp sequencing_aux RIGHT_PARENTHESIS
			{ $$ = makeMySequencingExp($2, $4, $5); }
		;
sequencing_aux	: SEMICOLON exp sequencing_aux	{ $$ = makeMyExpList($2, $3); }
		|				{ $$ = (myExpList)NULL; }
		;

for		: FOR ID ASSIGNMENT_SIGN exp TO exp DO exp
			{ $$ = makeMyForExp(
				MySymbol_MakeSymbol($2),
				$4,
				$6,
				$8);
			}
		;

if_then_else	: IF exp THEN exp ELSE exp	{ $$ = makeMyIfThenElseExp($2, $4, $6); }
		;

if_then		: IF exp THEN exp	{ $$ = makeMyIfThenExp($2, $4); }
		;

comparison	: exp EQUAL_SIGN exp
			{ $$ = makeMyComparisonExp_Equal($1, $3); }
		| exp NOT_EQUAL_SIGN exp
			{ $$ = makeMyComparisonExp_NotEqual($1, $3); }
		| exp LESS_SIGN exp
			{ $$ = makeMyComparisonExp_SmallerThan($1, $3); }
		| exp LESS_EQUAL_SIGN exp
			{ $$ = makeMyComparisonExp_SmallerEqualThan($1, $3); }
		| exp GREATER_SIGN exp
			{ $$ = makeMyComparisonExp_LargerThan($1, $3); }
		| exp GREATER_EQUAL_SIGN exp
			{ $$ = makeMyComparisonExp_LargerEqualThan($1, $3); }
		;

boolean_operate	: exp LOGICAL_AND exp
			{ $$ = makeMyBooleanOperateExp_And($1, $3); }
		| exp LOGICAL_OR exp
			{ $$ = makeMyBooleanOperateExp_Or($1, $3); }
		;

assignment	: l_value ASSIGNMENT_SIGN exp
			{ $$ = makeMyAssignmentExp($1, $3); }
		;

let		: LET decs IN expseq END	{ $$ = makeMyLetExp($2, $4); }
		;
expseq		: exp let_aux		{ $$ = makeMyExpList($1, $2); }
		|			{ $$ = (myExpList)NULL; }
		;
let_aux		: SEMICOLON exp let_aux	{ $$ = makeMyExpList($2, $3); }
		|			{ $$ = (myExpList)NULL; }
		;

while		: WHILE exp DO exp	{ $$ = makeMyWhileExp($2, $4); }
		;

negation	: MINUS_SIGN exp %prec DIVIDE_SIGN
			{ $$ = makeMyNegationExp($2); }
		;

break		: BREAK		{ $$ = makeMyBreakExp(); }
		;

exp	: l_value
	       	{ $$ = makeMyExp_LValue(getCurrentPosInfo(), $1); }
	| function_call
		{ $$ = makeMyExp_FunctionCall(getCurrentPosInfo(), $1); }
	| nil
		{ $$ = makeMyExp_Nil(getCurrentPosInfo(), $1); }
	| integer_literal
		{ $$ = makeMyExp_IntegerLiteral(getCurrentPosInfo(), $1); }
	| string_literal
		{ $$ = makeMyExp_StringLiteral(getCurrentPosInfo(), $1); }
	| array_creation
		{ $$ = makeMyExp_ArrayCreation(getCurrentPosInfo(), $1); }
	| record_creation
		{ $$ = makeMyExp_RecordCreation(getCurrentPosInfo(), $1); }
	| arithmetic
		{ $$ = makeMyExp_Arithmetic(getCurrentPosInfo(), $1); }
	| parentheses
		{ $$ = makeMyExp_Parentheses(getCurrentPosInfo(), $1); }
	| no_value
		{ $$ = makeMyExp_NoValue(getCurrentPosInfo(), $1); }
	| sequencing
		{ $$ = makeMyExp_Sequencing(getCurrentPosInfo(), $1); }
	| for
		{ $$ = makeMyExp_For(getCurrentPosInfo(), $1); }
	| if_then_else
		{ $$ = makeMyExp_IfThenElse(getCurrentPosInfo(), $1); }
	| if_then
		{ $$ = makeMyExp_IfThen(getCurrentPosInfo(), $1); }
	| comparison
		{ $$ = makeMyExp_Comparison(getCurrentPosInfo(), $1); }
	| boolean_operate
		{ $$ = makeMyExp_BooleanOperate(getCurrentPosInfo(), $1); }
	| assignment
		{ $$ = makeMyExp_Assignment(getCurrentPosInfo(), $1); }
	| let
		{ $$ = makeMyExp_Let(getCurrentPosInfo(), $1); }
	| while
		{ $$ = makeMyExp_While(getCurrentPosInfo(), $1); }
	| negation
		{ $$ = makeMyExp_Negation(getCurrentPosInfo(), $1); }
	| break
		{ $$ = makeMyExp_Break(getCurrentPosInfo(), $1); }
	;

		
program	: exp			{ g_AbstractSyntaxTree = $1; }
		
		
%%

void yyerror()			{	errorReport("Syntax analysis error!");	}


myExp parseOneFile(char* parseFile)
{
	resetPos(g_posInfo);

    if ((yyin = fopen(parseFile, "r")) == NULL)
    {
		printf("can not open file: %s\n", parseFile);
        return 0;
    }
	    
    if (yyparse() != 0)//	0 stands for parse error
	{
		fclose(yyin);
		return ABSTRACT_SYNTAX_ERROR;
	}
    
    fclose(yyin);
    return g_AbstractSyntaxTree;
}
