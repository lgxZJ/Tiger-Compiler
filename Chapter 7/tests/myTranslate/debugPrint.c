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
void printExpression(FILE* file, IR_myExp exp, int spaceNum);

//////

void printStatement(FILE* file, IR_myStatement statement, int spaceNum)
{
    newline(file), indentSpace(file, spaceNum);

    if (statement == NULL) 
    {
        fprintf(file, "null");
        return;
    }

    switch (statement->kind)
    {
        case IR_Seq:
            fprintf(file, "Seq(");
            printStatement(file, statement->u.seq.left, spaceNum + 2),

            indentSpace(file, spaceNum);
            fprintf(file, ", ");
            printStatement(file, statement->u.seq.right, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break; 
        case IR_CJump:
            fprintf(file, "CJump(%s, ", g_relOperatorStr[statement->u.cjump.op]);
            printExpression(file,  statement->u.cjump.left, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ", ");

            printExpression(file,  statement->u.cjump.right, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ", ");

            fprintf(file, "%s, ", MySymbol_GetName(statement->u.cjump.trueLabel));
            fprintf(file, "%s, ", MySymbol_GetName(statement->u.cjump.falseLabel));

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        case IR_Exp:
            fprintf(file, "Exp(");
            printExpression(file, statement->u.exp, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        case IR_Jump:
            fprintf(file, "Jump(");
            printExpression(file, statement->u.jump.exp, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        case IR_Label:
            fprintf(file, "Label(%s)\n", MySymbol_GetName(statement->u.label));
            break;
        case IR_Move:
            fprintf(file, "Move(");
            printExpression(file, statement->u.move.dst, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ", ");
            printExpression(file, statement->u.move.src, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        default:        assert (false);
    }
}

void printExpression(FILE* file, IR_myExp exp, int spaceNum)
{
    newline(file), indentSpace(file, spaceNum);
    if (exp == NULL)
    {
        fprintf(file, "null");
        return;
    }
    
    switch (exp->kind)
    {
        case IR_BinOperation:
            fprintf(file, "BinOperation(%s, ", g_binOperatorStr[exp->u.binOperation.op]);
            printExpression(file, exp->u.binOperation.left, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ", ");
            printExpression(file, exp->u.binOperation.right, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        case IR_Call:
            fprintf(file, "Call(");
            printExpression(file, exp->u.call.func, spaceNum + 2);

            IR_myExpList exps = exp->u.call.args;
            while (exps)
            {
                indentSpace(file, spaceNum);
                fprintf(file, ", ");
                printExpression(file, exps->head, spaceNum + 2);
                exps = exps->tails;
            }

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        case IR_Const:
            fprintf(file, "Const(%d)\n", exp->u.constValue);
            break;
        case IR_ESeq:
            fprintf(file, "Eseq(");
            printStatement(file, exp->u.eseq.statement, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ", ");
            printExpression(file, exp->u.eseq.exp, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");;
            break;
        case IR_Mem:
            fprintf(file, "Mem(");
            printExpression(file, exp->u.mem, spaceNum + 2);

            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        case IR_Name:
            fprintf(file, "Name(%s)\n", MySymbol_GetName(exp->u.name));
            break;
        case IR_Temp:
            fprintf(file, "Temp(");
            //  todo:
            indentSpace(file, spaceNum);
            fprintf(file, ")\n");
            break;
        default:    assert (false);
    }
}

void printFragments(FILE* file, Frame_myFragList fragments)
{
    while (fragments)
    {
        if (fragments->head->kind == Frame_StringFrag)
        {
            //  todo:
        }
        else
        {
            fprintf(file, "%s:\n", MySymbol_GetName(fragments->head->u.procFrag.funcName));
            printStatement(file, fragments->head->u.procFrag.body, 0);
            fprintf(file, "\n\n");
        }

        fragments = fragments->tail;
    }
}