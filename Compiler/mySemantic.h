#ifndef MY_SEMANTIC_H
#define MY_SEMANTIC_H

#include "myTypes.h"
#include "myTable.h"
#include "myAbstractSyntax.h"
#include "myTranslate.h"

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
     IR_myExp           translation;    //  intermediate expression
     myType             type;           //  return type
 }* myTranslationAndType;

myTranslationAndType make_MyTranslationAndType(
    IR_myExp translation, myType type);


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

/**
 *  @brief      Set the variable and function environment when do semantic
 *              analyzing.
 *  @param[in]  varAndFuncEnv   the environment to set.
 *  @return     This function has no return value.
 *
 *  @remark     Once set, the whole module's variable and function environment
 *              is affected.  
 */
void MySemantic_setVarAndFuncEnvironment    (myTable varAndFuncEnv);

/**
 *  @brief      Set the type environment when do semantic analyzing.
 *
 *  @param[in]  typeEnv   the environment to set.
 *  @return     This function has no return value.
 *
 *  @remark     Once set, the whole module's type environment is affected.  
 */
void MySemantic_setTypeEnvironment          (myTable typeEnv);

/**
 *  @brief      Set the current level when do Activation Records.
 *
 *  @param[in]  newLevel   the level to set.
 *  @return     This function has no return value.
 *
 *  @remark     Once set, the whole module's current level is affected.  
 */
void MySemantic_setCurrentLevel(Trans_myLevel newLevel);

/**
 *  @brief  get semantic module's variable and function environment.
 */
myTable MySemantic_getVarAndFuncEnvironment(void);

/**
 *  @brief  get semantic module's type environment.
 */
myTable MySemantic_getTypeEnvironment(void);

/**
 *  @brief  get semantic module's variable and function environment.
 */
Trans_myLevel MySemantic_getCurrentLevel(void);

#endif