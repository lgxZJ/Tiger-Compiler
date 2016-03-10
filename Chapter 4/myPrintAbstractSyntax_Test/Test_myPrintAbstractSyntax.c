#include "../myPrintAbstractSyntax.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/*--------------------------test functions---------------------------*/

myPos g_Pos = {1,1};

struct FileList
{
    FILE*		file;
    struct FileList*	next;
};

static struct FileList* g_FileListPtr;
static struct FileList* g_FileListHead;

//	return a valid FILE pointer if succeed, otherwise NULL
FILE* openFile(char* filename)
{
    char tmp[256] = {0};
    sprintf(tmp, "outputTxts/%s", filename);

    //	create or open file
    FILE* output = fopen(tmp, "w");
    if (output == NULL)	return (FILE*)NULL;

    //	allocate memory for next structure
    g_FileListPtr = (struct FileList*)myCheckedMalloc(sizeof(struct FileList));
    if (g_FileListPtr == NULL)
    {
	fclose(output);
	return (FILE*)NULL;
    }

    //	assign onen member
    g_FileListPtr->file = output;
    g_FileListPtr->next = NULL;
    return output;
}

int globalTeardown(void)
{
    struct FileList* ptr = g_FileListHead;
    struct FileList* former = NULL;

    while (ptr)
    {
	//	free memory
	if (former)	free(former);
	//	check if closed successfully
	if (fclose(ptr->file) != 0)	return -1;

	former = ptr;
	ptr = ptr->next;
    }

    //	former must be valid
    free(former);
}

void MyPrint_TyFieldList_ShouldHaveCorrectLineFeed(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_TyFieldList.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, (FILE*)NULL);

    myTyField tyField = makeMyTyField(
	    MySymbol_MakeSymbol("symbol1"),
	    MySymbol_MakeSymbol("symbol11"));
    CU_ASSERT_NOT_EQUAL_FATAL(tyField, (myTyField)NULL);

    myTyFieldList tyFieldList = makeMyTyFieldList(
	tyField, makeMyTyFieldList(tyField, NULL));
    CU_ASSERT_NOT_EQUAL(tyFieldList, (myTyFieldList)NULL);

    //	system under control
    MyPrint_TyFieldList(output, tyFieldList, 0);

    //	verify
    //	see output file to verify (two line in "text : text" format)

    //	see global teardown
}

void MyPrint_Ty_ShouldHaveCorrectOutputFormat(void)
{
    //	fixture setup
    myTy ty_named = makeMyTy_Named(g_Pos, MySymbol_MakeSymbol("symbol_named"));
    myTy ty_record = makeMyTy_Record(g_Pos,
        makeMyTyFieldList(
	    makeMyTyField(
		MySymbol_MakeSymbol("symbol_record"),
		MySymbol_MakeSymbol("symbol_record")),
	    NULL));
    myTy ty_array = makeMyTy_Array(g_Pos, MySymbol_MakeSymbol("symbol_array"));

    FILE* output = openFile("MyPrint_Ty.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    //	system under control
    MyPrint_Ty(output, ty_named, 0);
    fprintf(output, "\n");

    MyPrint_Ty(output, ty_record, 0);
    fprintf(output, "\n");

    MyPrint_Ty(output, ty_array, 0);

    //	see output text to check format
    
    //	see globalTeardown
}

void MyPrint_TypeDec_ShouldHaveCorrectOutputFormat(void)
{
    //	fixture setup
    myTypeDec tyDec = makeMyTyDec(
	MySymbol_MakeSymbol("symbol"),
	makeMyTy_Named(g_Pos, MySymbol_MakeSymbol("ty")));

    FILE* output = openFile("MyPrint_TypeDec.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);
    
    //	system under control
    MyPrint_TypeDec(output, tyDec, 0);

    //	see output
}

void MyPrint_LongFormVar_ShouldHaveCorrectOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_LongFormVar.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myLongFormVar longVar = makeMyLongFormVar(
	MySymbol_MakeSymbol("name"),
	MySymbol_MakeSymbol("type"),
	makeMyExp_NoValue(g_Pos, makeMyNoValueExp()));

    //	system under control
    MyPrint_LongFormVar(output, longVar, 0);

    //	see output to verify
}

void MyPrint_ShortFormVar_ShouldHaveCorrectOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_ShortFormVar.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);
    
    myShortFormVar shortVar = makeMyShortFormVar(
	MySymbol_MakeSymbol("name"),
        makeMyExp_NoValue(g_Pos, makeMyNoValueExp()));

    //	system under control
    MyPrint_ShortFormVar(output, shortVar, 0);

    //	verify
}


void MyPrint_VarDec_ShouldHaveCorrectOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_VarDec.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myVarDec varDec = makeMyVarDec_ShortForm(
	makeMyShortFormVar(
	    MySymbol_MakeSymbol("name"),
	    makeMyExp_NoValue(g_Pos, makeMyNoValueExp())
	)
    );

    //	system under control
    MyPrint_VarDec(output, varDec, 0);

    //	see output file verify
}

void MyPrint_ProcedureDec_ShouldHaveCorrectOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_ProcedureDec.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, (FILE*)NULL);

    myProcedureDec procedureDec = makeMyProcedureDec(
	MySymbol_MakeSymbol("symbol"),
	makeMyTyFieldList(
	    makeMyTyField(
		MySymbol_MakeSymbol("symbol_record"),
		MySymbol_MakeSymbol("symbol_record")),
	    NULL),
	makeMyExp_NoValue(g_Pos, makeMyNoValueExp())
    );
    CU_ASSERT_NOT_EQUAL_FATAL(procedureDec, (myProcedureDec)NULL);

    //	system under control
    MyPrint_ProcedureDec(output, procedureDec, 0);

    //	see ouput to verify
}

void MyPrint_FunctionDec_ShouldHaveCorrectOutputFormat(void)
{
    FILE* output = openFile("MyPrint_FunctionDec.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, (FILE*)NULL);

    myFunctionDec functionDec = makeMyFunctionDec(
	MySymbol_MakeSymbol("name"),
	makeMyTyFieldList(
	    makeMyTyField(
		MySymbol_MakeSymbol("symbol_record"),
		MySymbol_MakeSymbol("symbol_record")),
	    NULL),
	MySymbol_MakeSymbol("returnType"),
	makeMyExp_NoValue(g_Pos, makeMyNoValueExp()));
    CU_ASSERT_NOT_EQUAL_FATAL(functionDec, (myFunctionDec)NULL);

    //	system under control
    MyPrint_FunctionDec(output, functionDec, 0);

    //	see output to verify
}

void MyPrint_FuncDec_ShouldHaveCorrectOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_FuncDec.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myFuncDec funcDec = makeMyFuncDec_Function(
	makeMyFunctionDec(
	    MySymbol_MakeSymbol("name"),
	    makeMyTyFieldList(
		makeMyTyField(
		    MySymbol_MakeSymbol("symbol_record"),
		    MySymbol_MakeSymbol("symbol_record")),
		NULL),
	    MySymbol_MakeSymbol("returnType"),
	    makeMyExp_NoValue(g_Pos, makeMyNoValueExp())));
    CU_ASSERT_NOT_EQUAL_FATAL(funcDec, (myFuncDec)NULL);

    //	system under control
    MyPrint_FuncDec(output, funcDec, 0);

    //	see output	
}

void MyPrint_Dec_ShouldHaveRightOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_Dec.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myDec dec = makeMyDec_Var(g_Pos,
       makeMyVarDec_ShortForm(
	 makeMyShortFormVar(
	   MySymbol_MakeSymbol("name"),
	   makeMyExp_NoValue(g_Pos, makeMyNoValueExp())
	 )
       ));
    CU_ASSERT_NOT_EQUAL_FATAL(dec, NULL);

    //	system under control
    MyPrint_Dec(output, dec, 0);

    //	see output
}

void MyPrint_DecList_ShouldHaveRightOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_DecList.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myDecList decList = makeMyDecList(
	makeMyDec_Var(g_Pos,
		      makeMyVarDec_ShortForm(
			  makeMyShortFormVar(
			      MySymbol_MakeSymbol("name"),
			      makeMyExp_NoValue(g_Pos, makeMyNoValueExp())
			      )
			  )),
	makeMyDecList(
	    makeMyDec_Var(g_Pos,
			  makeMyVarDec_ShortForm(
			      makeMyShortFormVar(
				  MySymbol_MakeSymbol("name"),
				  makeMyExp_NoValue(g_Pos, makeMyNoValueExp())
				  )
			      )),
	    NULL));
    CU_ASSERT_NOT_EQUAL_FATAL(decList, NULL);

    //	system under control
    MyPrint_DecList(output, decList, 0);

    //	see output
}

void MyPrint_LValueExp_ShouldHaveRightOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_SimpleVar.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myLValueExp lValueExp_SimpleVar = makeMyLValue(g_Pos,
					 MySymbol_MakeSymbol("id"),
					 (myLValueAux)NULL);
    CU_ASSERT_NOT_EQUAL_FATAL(lValueExp_SimpleVar, NULL);

    myLValueExp lValueExp_RecordField = makeMyLValue(g_Pos,
						     MySymbol_MakeSymbol("id"),
						     makeMyLValueAux(
							 MySymbol_MakeSymbol("id"),
							 NULL, NULL));
    CU_ASSERT_NOT_EQUAL_FATAL(lValueExp_RecordField, NULL);

    myLValueExp lValueExp_ArraySubscript = makeMyLValue(g_Pos,
							MySymbol_MakeSymbol("id"),
						        makeMyLValueAux(
							    NULL,
							    makeMyExp_NoValue(g_Pos, makeMyNoValueExp()),
							    NULL));
    CU_ASSERT_NOT_EQUAL_FATAL(lValueExp_ArraySubscript, NULL);

    //	system under control
    MyPrint_LValueExp(output, lValueExp_SimpleVar, 0);
    fprintf(output, "\n");

    MyPrint_LValueExp(output, lValueExp_RecordField, 0);
    fprintf(output, "\n");

    MyPrint_LValueExp(output, lValueExp_ArraySubscript, 0);

    //	see output
}

void MyPrint_NoParamFunctionCallExp_ShouldHaveRightOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_NoParamFunctionCallExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myNoParamFunctionCallExp exp = makeMyNoParamFunctionCallExp(
	MySymbol_MakeSymbol("symbol"));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	system under control
    MyPrint_NoParamFunctionCallExp(output, exp, 0);

    //	see output to verify
}

void MyPrint_ParamFunctionCallExp_ShouldHaveRightOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_ParamFunctionCallExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myParamFunctionCallExp exp = makeMyParamFunctionCallExp(
	MySymbol_MakeSymbol("name"),
	makeMyExpList(
	    makeMyExp_NoValue(g_Pos, makeMyNoValueExp()),
	    NULL));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	system under control
    MyPrint_ParamFunctionCallExp(output, exp, 0);

    //	see output to verify
}

void MyPrint_FunctionCallExp_ShouldHaveRightOutputFormat(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_FunctionCallExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myFunctionCallExp functionExp_NoParam = makeMyFunctionCallExp_NoParam(
	makeMyNoParamFunctionCallExp(
	    MySymbol_MakeSymbol("name")));
    CU_ASSERT_NOT_EQUAL_FATAL(functionExp_NoParam, NULL);

    myFunctionCallExp functionExp_Param = makeMyFunctionCallExp_Param(
	    makeMyParamFunctionCallExp(
		MySymbol_MakeSymbol("name"),
		makeMyExpList(
		    makeMyExp_NoValue(g_Pos, makeMyNoValueExp()),
		    NULL)));
    CU_ASSERT_NOT_EQUAL_FATAL(functionExp_Param, NULL);

	//	system under control
	MyPrint_FunctionCallExp(output, functionExp_NoParam, 0);
	fprintf(output, "\n");
	MyPrint_FunctionCallExp(output, functionExp_Param, 0);

    //	see output to verify
}

void MyPrint_NilExp_RightOutput(void)
{
    //	fxiture setup
    FILE* output = openFile("MyPrint_NilExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myNilExp nilExp = makeMyNilExp();
    CU_ASSERT_NOT_EQUAL_FATAL(nilExp, NULL);
    
    //	system under control, see output text to verify
    MyPrint_NilExp(output, nilExp, 0);
}

void MyPrint_IntegerLiteralExp_RightOuput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_IntegerLiteralExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myIntegerLiteralExp integerLiteralExp = makeMyIntegerLiteralExp(1);
    CU_ASSERT_NOT_EQUAL_FATAL(integerLiteralExp, NULL);

    //	test, see output to verify 
    MyPrint_IntegerLiteralExp(output, integerLiteralExp, 0);
}

void MyPrint_StringLiteralExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_StringLiteralExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myStringLiteralExp stringLiteralExp = makeMyStringLiteralExp("string");
    CU_ASSERT_NOT_EQUAL_FATAL(stringLiteralExp, NULL);

    //	test, see output to verify
    MyPrint_StringLiteralExp(output, stringLiteralExp, 0);
}

void MyPrint_ArrayCreationExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_ArrayCreationExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myArrayCreationExp arrayCreationExp = makeMyArrayCreationExp(
	MySymbol_MakeSymbol("tyepeName"),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(2)));
    CU_ASSERT_NOT_EQUAL_FATAL(arrayCreationExp, NULL);

    //	test, see output file
    MyPrint_ArrayCreationExp(output, arrayCreationExp, 0);
}

void MyPrint_NoFieldRecordCreationExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_NoFieldRecordCreationExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myNoFieldRecordCreationExp exp = makeMyNoFieldRecordCreationExp(
	MySymbol_MakeSymbol("typeName"));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test, verify
    MyPrint_NoFieldRecordCreationExp(output, exp, 0);
}

void MyPrint_FieldRecordCreationExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_FieldRecordCreationExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myFieldRecordCreationExp exp = makeMyFieldRecordCreationExp(
	MySymbol_MakeSymbol("type name"),
	makeMyRecordFieldList(
	    makeMyRecordField(
		MySymbol_MakeSymbol("varName"),
		makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1))),
	    NULL));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_FieldRecordCreationExp(output, exp, 0);
}

void MyPrint_RecordCreationExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_RecordCreationExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myRecordCreationExp exp = makeMyRecordCreationExp_NoField(
	makeMyNoFieldRecordCreationExp(
	    MySymbol_MakeSymbol("type name")));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test, verify
    MyPrint_RecordCreationExp(output, exp, 0);
}

void MyPrint_ArithmeticExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_ArithmeticExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myArithmeticExp exp_plus = makeMyArithmeticExp_Plus(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp_plus, NULL);

    myArithmeticExp exp_minus = makeMyArithmeticExp_Minus(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp_minus, NULL);


    myArithmeticExp exp_divide = makeMyArithmeticExp_Divide(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp_divide, NULL);
    
    myArithmeticExp exp_multiply = makeMyArithmeticExp_Multiply(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp_multiply, NULL);

    //	test and verify
    MyPrint_ArithmeticExp(output, exp_plus, 0);
    fprintf(output, "\n");

    MyPrint_ArithmeticExp(output, exp_minus, 0);
    fprintf(output, "\n");

    MyPrint_ArithmeticExp(output, exp_divide, 0);
    fprintf(output, "\n");

    MyPrint_ArithmeticExp(output, exp_multiply, 0);
}

void MyPrint_ParenthesesExp_RightOutput(void)
{
    //	fxiture setup
    FILE* output = openFile("MyPrint_ParenthesesExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myParenthesesExp exp = makeMyParenthesesExp(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_ParenthesesExp(output, exp, 0);
}

void MyPrint_NoValueExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_NoValueExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myNoValueExp exp = makeMyNoValueExp();
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_NoValueExp(output, exp, 0);
}

void MyPrint_SequencingExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_SequencingExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    mySequencingExp exp = makeMySequencingExp(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	NULL);
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_SequencingExp(output, exp, 0);
}

void MyPrint_ForExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_ForExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myForExp forExp = makeMyForExp(
	MySymbol_MakeSymbol("varName"),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(3)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(2)));
    CU_ASSERT_NOT_EQUAL_FATAL(forExp, NULL);

    //	test and verify
    MyPrint_ForExp(output, forExp, 0);
}

void MyPrint_IfThenElseExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_IfThenElseExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myIfThenElseExp exp = makeMyIfThenElseExp(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(3)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(3)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(3)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	TEST AND VERIFY
    MyPrint_IfThenElseExp(output, exp, 0);
}

void MyPrint_IfThenExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_IfThenExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myIfThenExp exp = makeMyIfThenExp(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_IfThenExp(output, exp, 0);
}

void MyPrint_ComparisonExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_ComparisonExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myComparisonExp exp_equal = makeMyComparisonExp_Equal(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    myComparisonExp exp_not_equal = makeMyComparisonExp_NotEqual(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    myComparisonExp exp_larger_than = makeMyComparisonExp_LargerThan(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    myComparisonExp exp_smaller_than = makeMyComparisonExp_SmallerThan(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    myComparisonExp exp_larger_equal_than = makeMyComparisonExp_LargerEqualThan(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    myComparisonExp exp_smaller_equal_than = makeMyComparisonExp_SmallerEqualThan(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_FATAL(exp_equal && exp_not_equal && exp_larger_than &&
		    exp_smaller_than && exp_larger_equal_than && exp_smaller_equal_than);

    //	test and verify
    MyPrint_ComparisonExp(output, exp_equal, 0);
    fprintf(output, ",\n");

    MyPrint_ComparisonExp(output, exp_not_equal, 0);
    fprintf(output, ",\n");

    MyPrint_ComparisonExp(output, exp_larger_than, 0);
    fprintf(output, ",\n");

    MyPrint_ComparisonExp(output, exp_smaller_than, 0);
    fprintf(output, ",\n");

    MyPrint_ComparisonExp(output, exp_larger_equal_than, 0);
    fprintf(output, ",\n");

    MyPrint_ComparisonExp(output, exp_smaller_equal_than, 0);
    fprintf(output, ",\n");
}

void Print_BooleanOperateExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_BooleanOperateExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myBooleanOperateExp exp_or = makeMyBooleanOperateExp_Or(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    myBooleanOperateExp exp_and = makeMyBooleanOperateExp_And(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_FATAL(exp_or && exp_and);

    //	test and verify
    MyPrint_BooleanOperateExp(output, exp_or, 0);
    fprintf(output, ",\n");
    MyPrint_BooleanOperateExp(output, exp_and, 0);
}

void MyPrint_AssignmentExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_AssignmentExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myAssignmentExp exp = makeMyAssignmentExp(
	makeMyLValue(g_Pos,
		     MySymbol_MakeSymbol("id"),
		     (myLValueAux)NULL),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_AssignmentExp(output, exp, 0);
}

void MyPrint_LetExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_LetExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myLetExp exp = makeMyLetExp(
	makeMyDecList(
	    makeMyDec_Var(g_Pos,
			  makeMyVarDec_ShortForm(
			      makeMyShortFormVar(
				  MySymbol_MakeSymbol("name"),
				  makeMyExp_NoValue(g_Pos, makeMyNoValueExp())
				  )
			      )),
	    makeMyDecList(
		makeMyDec_Var(g_Pos,
			      makeMyVarDec_ShortForm(
				  makeMyShortFormVar(
				      MySymbol_MakeSymbol("name"),
				      makeMyExp_NoValue(g_Pos, makeMyNoValueExp())
				      )
				  )),
		NULL)),
	NULL);
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_LetExp(output, exp, 0);
}

void MyPrint_WhileExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_WhileExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myWhileExp exp = makeMyWhileExp(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_WhileExp(output, exp, 0);
}

void MyPrint_NegationExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_NegationExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myNegationExp exp = makeMyNegationExp(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)));
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_NegationExp(output, exp, 0);
}

void MyPrint_BreakExp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_BreakExp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myBreakExp exp = makeMyBreakExp();
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_BreakExp(output, exp, 0);
}

void MyPrint_Exp_RightOutput(void)
{
    //	fixture setup
    FILE* output = openFile("MyPrint_Exp.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myExp exp = makeMyExp_Nil(g_Pos, makeMyNilExp());
    CU_ASSERT_NOT_EQUAL_FATAL(exp, NULL);

    //	test and verify
    MyPrint_Exp(output, exp, 0);
}

void MyPrint_ExpList_RightOutput(void)
{
    FILE* output = openFile("MyPrint_ExpList.txt");
    CU_ASSERT_NOT_EQUAL_FATAL(output, NULL);

    myExpList expList = makeMyExpList(
	makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	makeMyExpList(
	    makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
	    makeMyExpList(
		makeMyExp_IntegerLiteral(g_Pos, makeMyIntegerLiteralExp(1)),
		NULL)));
    CU_ASSERT_NOT_EQUAL_FATAL(expList, NULL);

    //	test and verify
    MyPrint_ExpList(output, exp, 0);
}




/*-------------------------------------------------------------------*/


int main (int argc, char* argv[])
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
	printf("Failed to initialize registry!\n");
	exit(-1);
    }

    CU_pSuite suite = CU_add_suite("suite for myPrintXXX", NULL, globalTeardown);
    if (suite == NULL)
    {
	printf("Failed to add suite!\n");
	exit(-1);
    }

    //	add tests
    CU_TestInfo tests[] = {
	{ "test func----MyPrint_TyFieldList()", MyPrint_TyFieldList_ShouldHaveCorrectLineFeed },
	{ "test func----MyPrint_Ty()", MyPrint_Ty_ShouldHaveCorrectOutputFormat },
	{ "test func----MyPrint_TypeDec()", MyPrint_TypeDec_ShouldHaveCorrectOutputFormat },
	{ "test func----MyPrint_LongFormVar()", MyPrint_LongFormVar_ShouldHaveCorrectOutputFormat },
	{ "test func----MyPrint_ShortFormVar()", MyPrint_ShortFormVar_ShouldHaveCorrectOutputFormat },
	{ "test func---MyPrint_VarDec()", MyPrint_VarDec_ShouldHaveCorrectOutputFormat },
	{ "test func----MyPrint_ProcedureDec()", MyPrint_ProcedureDec_ShouldHaveCorrectOutputFormat },
	{ "test func----MyPrint_FunctionDec()", MyPrint_FunctionDec_ShouldHaveCorrectOutputFormat },
	{ "test func----MyPrint_FuncDec()", MyPrint_FuncDec_ShouldHaveCorrectOutputFormat },
	{ "test func----MyPrint_Dec()",  MyPrint_Dec_ShouldHaveRightOutputFormat },
	{ "test func----MyPrint_DecList()", MyPrint_DecList_ShouldHaveRightOutputFormat },
	{ "test func----MyPrint_LValueExp()", MyPrint_LValueExp_ShouldHaveRightOutputFormat },
	{ "test func----MyPrint_NoParamFunctionCallExp()", MyPrint_NoParamFunctionCallExp_ShouldHaveRightOutputFormat },
	{ "test func----MyPrint_ParamFunctionCallExp()", MyPrint_ParamFunctionCallExp_ShouldHaveRightOutputFormat },
	{ "test func----MyPrint_FunctionCallExp()", MyPrint_FunctionCallExp_ShouldHaveRightOutputFormat },
	{ "test func----MyPrint_NilExp()", MyPrint_NilExp_RightOutput },
	{ "test func----MyPrint_IntegerLiteralExp()", MyPrint_IntegerLiteralExp_RightOuput },
	{ "test func----MyPrint_StringLiteralExp()", MyPrint_StringLiteralExp_RightOutput },
	{ "test func----MyPrint_ArrayCreationExp()", MyPrint_ArrayCreationExp_RightOutput },
	{ "test func----MyPrint_NoFieldRecordCreationExp()", MyPrint_NoFieldRecordCreationExp_RightOutput },
	{ "test func----MyPrint_FieldRecordCreationExp()", MyPrint_FieldRecordCreationExp_RightOutput },
	{ "test func----MyPrint_RecordCreationExp()", MyPrint_RecordCreationExp_RightOutput },
	{ "test func----MyPrint_ArithmeticExp()", MyPrint_ArithmeticExp_RightOutput },
	{ "test func----MyPrint_ParenthesesExp()", MyPrint_ParenthesesExp_RightOutput },
	{ "test func----MyPrint_NoValueExp()", MyPrint_NoValueExp_RightOutput },
	{ "test func----MyPrint_SequencingExp()", MyPrint_SequencingExp_RightOutput },
	{ "test func----MyPrint_ForExp()", MyPrint_ForExp_RightOutput },
	{ "test func----MyPrint_IfThenElseExp()", MyPrint_IfThenElseExp_RightOutput },
	{ "test func----MyPrint_IfThenExp()", MyPrint_IfThenExp_RightOutput },
	{ "test func----MyPrint_Comparison()", MyPrint_ComparisonExp_RightOutput },
	{ "test func----MyPrint_BooleanOperateExp()", Print_BooleanOperateExp_RightOutput },
	{ "test func----MyPrint_AssignmentExp()", MyPrint_AssignmentExp_RightOutput },
	{ "test func----MyPrint_LetExp()", MyPrint_LetExp_RightOutput },
	{ "test func----MyPrint_WhileExp()", MyPrint_WhileExp_RightOutput },
	{ "test func----MyPrint_NegationExp()", MyPrint_NegationExp_RightOutput },
	{ "test func----MyPrint_BreakExp()", MyPrint_BreakExp_RightOutput },
	{ "test func----MyPrint_Exp()", MyPrint_Exp_RightOutput },
	{ "test func----MyPrint_ExpList()", MyPrint_ExpList_RightOutput }
    };
    
    for (unsigned i = 0; i < (sizeof(tests) / sizeof(tests[0])); ++i)
	if (CU_add_test(suite, tests[i].pName, tests[i].pTestFunc) == NULL)
	{
	    printf("attempt to add tests, failed!\n");
	    exit(-1);
	}

    if (CU_basic_run_suite(suite) != CUE_SUCCESS)
    {
	printf("Failed to run suite!\n");
	exit(-1);
    }

    CU_cleanup_registry();
    return 0;
}
