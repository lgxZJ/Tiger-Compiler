#include "frontTester.h"
#include "myParser.h"
#include "mySemantic.h"
#include "myError.h"
#include "myEnvironment.h"

#include <stdlib.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////////

IR_myStatement frontTester(char* filename)
{
    MySemantic_setVarAndFuncEnvironment(myEnvironment_BaseVarAndFunc());
    MySemantic_setTypeEnvironment(myEnvironment_BaseType());
    MyError_clearErrors();

    //  lexical and syntax analysis
    myExp exp = parseOneFile(filename);
    assert (exp != NULL);
    assert (exp != ABSTRACT_SYNTAX_ERROR);

    //  semantic analysis and tranlation
    myTranslationAndType result = MySemantic_Exp(exp);
    if (MyError_isErrorSet())
        MyError_printErrors();

    //  the whole program is a function call, no other statements
    assert (result->translation->kind == IR_ESeq);
    return result->translation->u.eseq.statement;
}

///////////////////////////////////////////////////////////////////////