#include "debugPrint.h"
#include "../../myTranslate.h"

#include <assert.h>

static const char* g_binOperatorStr[] = {
    "Plus", "Minus", "Nultiply", "Divide", "And", "Or",
    "LShift", "RShift", "ARShift", "Xor"
};
static const char* g_relOperatorStr[] = {
    "Equal", "NotEqual", "LessThan", "GreaterThan", "LessEqual",
    "GreaterThan", "ULessThan", "ULessEqual", "UGreaterThan", "UGreaterEqual"
};

static void indentSpace(FILE* file, int spaceNum)
{
    for (int i = 0; i < spaceNum; ++i)
        fprintf(file, " ");
}

static void newline(FILE* file)
{
    fprintf(file, "\n");
}

////////////////////////////////////////////

//  forwards
void printExpression(FILE* file, IR_myExp exp, int spaceNum);

//////

void indentAndPrintToFile(FILE* file, int spaceNum, char* str)
{
    indentSpace(file, spaceNum);
    fprintf(file, "%s",  str);
}

void printNull(FILE* file)
{
    fprintf(file, "null");
}

void printSeqStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    fprintf(file, "Seq(");
    printStatement(file, statement->u.seq.left, spaceNum + 2),

    indentAndPrintToFile(file, spaceNum, ", ");
    printStatement(file, statement->u.seq.right, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ")\n");
}

void printCJumpStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    fprintf(file, "CJump(%s, ", g_relOperatorStr[statement->u.cjump.op]);
    printExpression(file,  statement->u.cjump.left, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ", ");
    printExpression(file,  statement->u.cjump.right, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ", ");
    fprintf(file, "%s, ", MySymbol_GetName(statement->u.cjump.trueLabel));
    fprintf(file, "%s, ", MySymbol_GetName(statement->u.cjump.falseLabel));

    indentAndPrintToFile(file, spaceNum, ")\n");
}

void printExpStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    fprintf(file, "Exp(");
    printExpression(file, statement->u.exp, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ")\n");
}

void printJumpStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    fprintf(file, "Jump(");
    printExpression(file, statement->u.jump.exp, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ")\n");
}

void printLabelStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    fprintf(file, "Label(%s)\n", MySymbol_GetName(statement->u.label));
}

void printMoveStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    fprintf(file, "Move(");
    printExpression(file, statement->u.move.dst, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ", ");
    printExpression(file, statement->u.move.src, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ")\n");
}

void printStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    newline(file), indentSpace(file, spaceNum);

    if (statement == NULL) 
        return printNull(file);

    switch (statement->kind)
    {
        case IR_Seq:
            return printSeqStatement(file, statement, spaceNum);
        case IR_CJump:
            return printCJumpStatement(file, statement, spaceNum);
        case IR_Exp:
            return printExpStatement(file, statement, spaceNum);
        case IR_Jump:
            return printJumpStatement(file, statement, spaceNum);
        case IR_Label:
            return printLabelStatement(file, statement, spaceNum);
        case IR_Move:
            return printMoveStatement(file, statement, spaceNum);
        default:        assert (false);
    }
}

//////////////////////////////////////////////////////

void printBinOperation(FILE* file, IR_myExp exp, int spaceNum)
{
    fprintf(file, "BinOperation(%s, ", g_binOperatorStr[exp->u.binOperation.op]);
    printExpression(file, exp->u.binOperation.left, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ", ");
    printExpression(file, exp->u.binOperation.right, spaceNum + 2);

    indentAndPrintToFile(file, spaceNum, ")\n");
}

void printCall(FILE* file, IR_myExp exp, int spaceNum)
{
    fprintf(file, "Call(");
	printExpression(file, exp->u.call.func, spaceNum + 2);

	IR_myExpList exps = exp->u.call.args;
	while (exps)
	{
	    indentAndPrintToFile(file, spaceNum, ", ");
	    printExpression(file, exps->head, spaceNum + 2);

	    exps = exps->tails;
	}

	indentAndPrintToFile(file, spaceNum, ")\n");
}

void printConst(FILE* file, IR_myExp exp, int spaceNum)
{
    fprintf(file, "Const(%d)\n", exp->u.constValue);
}

void printESeq(FILE* file, IR_myExp exp, int spaceNum)
{
    fprintf(file, "Eseq(");
	printStatement(file, exp->u.eseq.statement, spaceNum + 2);

	indentAndPrintToFile(file, spaceNum, ", ");
	printExpression(file, exp->u.eseq.exp, spaceNum + 2);

	indentAndPrintToFile(file, spaceNum, ")\n");
}

void printMem(FILE* file, IR_myExp exp, int spaceNum)
{
    fprintf(file, "Mem(");
	printExpression(file, exp->u.mem, spaceNum + 2);

	indentAndPrintToFile(file, spaceNum, ")\n");
}

void printName(FILE* file, IR_myExp exp, int spaceNum)
{
    fprintf(file, "Name(%s)\n", MySymbol_GetName(exp->u.name));
}

void printTemp(FILE* file, IR_myExp exp, int spaceNum)
{
    if (exp->u.temp == Frame_FP())
	    fprintf(file, "Temp(%s)\n", "frame pointer");
	else if (exp->u.temp == Frame_RV())
        fprintf(file, "Temp(%s)\n", "return register");
	else
        fprintf(file, "Temp(%d)\n", Temp_getTempNum(exp->u.temp));
}

void printExpression(FILE* file, IR_myExp exp, int spaceNum)
{
    newline(file), indentSpace(file, spaceNum);
    if (exp == NULL)
        return printNull(file);
    
    switch (exp->kind)
    {
        case IR_BinOperation:
            return printBinOperation(file, exp, spaceNum);
        case IR_Call:
            return printCall(file, exp, spaceNum);
        case IR_Const:
            return printConst(file, exp, spaceNum);
        case IR_ESeq:
            return printESeq(file, exp, spaceNum);
        case IR_Mem:
            return printMem(file, exp, spaceNum);
        case IR_Name:
            return printName(file, exp, spaceNum);
        case IR_Temp:
            return printTemp(file, exp, spaceNum);
        default:    assert (false);
    }
}

////////////////////////////////////////////////////////////

void printFragments(FILE* file, Frame_myFragList fragments)
{
    while (fragments)
    {
        if (fragments->head->kind == Frame_StringFrag)
        {
            fprintf(file, "%s:\n", fragments->head->u.strFrag.str);
            indentSpace(file, 2);
            fprintf(file, "Label: %s\n\n",
                MySymbol_GetName(fragments->head->u.strFrag.label));
        }
        else
        {
            fprintf(file, "%s:\n", MySymbol_GetName(fragments->head->u.procFrag.funcName));
            printStatement(file, fragments->head->u.procFrag.body, 2);
            fprintf(file, "\n\n");
        }

        fragments = fragments->tail;
    }
}