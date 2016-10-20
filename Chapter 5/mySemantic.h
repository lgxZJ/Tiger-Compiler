#ifndef MY_SEMANTIC_H
#define MY_SEMANTIC_H

#include "myTypes.h"
#include "myTable.h"
#include "myAbstractSyntax.h"

/**
 *  mySemantic.h-----------------SEMANTIC MODULE.
 *
 *      This module do tasks of semantic analysis which
 *  include type-checking of abstract syntax.
 */

#define SEMANTIC_ERROR  (void*)1

/*  Translated expression type(intermediate code) */
typedef struct myTranslation_{

}* myTranslation;


/*
 *  NOTE: 
 *      In semantic processing, we'll simply fill NULL in `translation` field. 
 */
 typedef struct myTranslationAndType_ {
     myTranslation      translation;    //  intermediate expression
     myType             type;           //  return type
 }* myTranslationAndType;

myTranslationAndType make_MyTranslationAndType
    (myTranslation translation, myType type);


/**
 *  @brief  Do semantic analysis on a given expression represented by abstract
 *          syntax tree.
 *
 *  @param[in]  varAndFuncEnv   A symbol table containing information about
 *              variables, functions and procedures.
 *  @param[in]  typeEnv         A symbol table containing information about
 *              types.
 *  @param[in]  exp             The expression to analyze.
 *
 *  @return     A myTranslationAndType holds the intermediate expression and
 *              its type after analysis if succeed; otherwise, SEMANTIC_ERROR.
 */
 myTranslationAndType MySemantic_Exp(myExp   exp);

//  todo:
void MySemantic_setVarAndFuncEnvironment    (myTable varAndEnvEnv);
void MySemantic_setTypeEnvironment          (myTable typeEnv);

myTable MySemantic_getVarAndFuncEnvironment(void);
myTable MySemantic_getTypeEnvironment(void);

#endif