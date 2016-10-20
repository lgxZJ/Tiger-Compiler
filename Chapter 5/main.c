#include "myParser.h"
#include "myError.h"
#include "myEnvironment.h"
#include "mySemantic.h"
#include "myAbstractSyntax.h"

#include <stdio.h>
#include <assert.h>

bool semanticAnalyzeOne(char* filename)
{
    myExp rootExp = parseOneFile(filename);
    assert (rootExp != ABSTRACT_SYNTAX_ERROR);

    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    myTranslationAndType result =
        MySemantic_Exp(
            rootExp);
        
    return !MyError_isErrorSet();
} 

void analyzeOneAndOutput(char* filepath)
{
    semanticAnalyzeOne(filepath);
        
    if (MyError_isErrorSet())
        MyError_printErrors(),
        MyError_clearErrors();
    else
        fprintf(stderr, "\t\tsucceed!");

    fprintf(stderr, "\n");
}

void analyzeCorrectFiles()
{
    for (int i = 1; i <= 22; ++i)
    {
        fprintf(stderr, "No.%d:\n", i);

        char filepath[256];
        sprintf(filepath, "testcases/correct/test%d.tig", i);

        analyzeOneAndOutput(filepath);
    }
}

void analyzeAnotherCorrectFiles()
{
    char* filenames[2] = { "merge.tig", "queens.tig" };
    for (int i = 0; i < 2; ++i)
    {
        fprintf(stderr, "File : %s\n", filenames[i]);

        char filepath[256];
        sprintf(filepath, "testcases/correct/%s", filenames[i]);

        analyzeOneAndOutput(filepath);
    }
}

void analyzeIncorrectFiles()
{
    for (int i = 1; i <= 34; ++i)
    {
        fprintf(stderr, "No.%d:\n", i);

        char filepath[256];
        sprintf(filepath, "testcases/incorrect/test%d.tig", i);

        analyzeOneAndOutput(filepath);
    }
}

int main (int argc, char* argv[])
{
    analyzeCorrectFiles();
    fprintf(stderr, "=========================\n\n");
    analyzeAnotherCorrectFiles();
    fprintf(stderr, "=========================\n\n");
    analyzeIncorrectFiles();

    return 0;
}