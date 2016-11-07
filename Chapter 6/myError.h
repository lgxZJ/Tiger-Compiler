#ifndef MY_ERROR_H
#define MY_ERROR_H

/**
 *  MODULE: 
 *      Error reporting module in the whole project.
 *  TODO:
 *      replace myReport.c with this one.
 */


#include <stdbool.h>

//
//  error codes are all non-negative
//

//  general error code
#define ERROR_NO                        0
#define ERROR_SUBSCRIPT_NOT_INT         1
#define ERROR_RECORD_FIELD_NOT_FOUND    2
#define ERROR_SUBSCRIPT_ERROR           3

//  specific error code
#define ERROR__LET__ECLARATIONS_ILLEGAL                     4
#define ERROR__LET__BODY_ILLEGAL                            5

#define ERROR__LVALUE_SIMPLE_VAR__VAR_NOT_DECLARED          6
#define ERROR__LVALUE_RECORD_FIELD__VAR_NOT_DECLARED        7
#define ERROR__LVALUE_RECORD_FIELD__VAR_NOT_RECORD          8
#define ERROR__LVALUE_ARRAY_SUBSCRIPT__VAR_NOT_DECLARED   	9
#define ERROR__LVALUE_ARRAY_SUBSCRIPT__VAR_NOT_ARRAY_TYPE 	10
#define ERROR__LVALUE_ARRAY_SUBSCRIPT__SUBSCRIPT_NOT_INT    11

#define ERROR__FUNCTION_CALL__FUNC_NOT_DEFINED            	12

#define ERROR__ARRAY_CREATION__TYPE_NOT_ARRAY             	13
#define ERROR__ARRAY_CREATION__SUBSCRIPT_NOT_INT            14
#define ERROR__ARRAY_CREATION__INITIAL_TYPE_NOT_ELEMENT   	15

#define ERROR__RECORD_CREATION__TYPE_NOT_DEFINED_AS_RECORD  16
#define ERROR__RECORD_CREATION__RECORD_TYPE_NOT_DEFINED   	17
#define ERROR__RECORD_CREATION__FIELDS_NOT_MATCH          	18

#define ERROR__ARITHMETIC__EXPRESSION_TYPE_NOT_INT        	19

#define ERROR__FOR__EXPRESSION_NOT_TYPE_INT                 20
#define ERROR__FOR__EXPRESSION_NOT_TYPE_NORETURN          	21

#define ERROR__IF_THEN_ELSE__IF_CONDITION_NOT_TYPE_INT    	22
#define ERROR__IF_THEN_ELSE__CLAUSE_NOT_LEGAL             	23
#define ERROR__IF_THEN_ELSE__CLAUSE_TYPE_NOT_MATCH          24

#define ERROR__IF_THEN__IF_CONDITION_NOT_TYPE_INT         	25
#define ERROR__IF_THEN__THEN_CLAUSE_TYPE_NOT_NORETURN     	26

#define ERROR__COMPARISON__OPERAND_EXPRESSION_NOT_LEGAL     27
#define ERROR__COMPARISON__OPERANDS_TYPE_NOT_MATCH          28

#define ERROR__BOOLEAN_OPERATE__OPERAND_NOT_INT             29

#define ERROR__ASSIGNMENT__OPERAND_NOT_LEGAL                30
#define ERROR__ASSIGNMENT__OPERANDS_TYPE_NOT_MATCH          31

#define ERROR__TYPEDEC_NAMED__TYPE_NOT_DEFINED              32
#define ERROR__TYPEDEC_RECORD__TYPE_NOT_DEFINED             33
#define ERROR__TYPEDEC_ARRAY__TYPE_NOT_DEFINED              34

#define ERROR__VARDEC_SHORT__TYPE_CAN_NOT_BE_NIL_NORETURN   35
#define ERROR__VARDEC_LONG__TYPE_NOT_DEFINED                36
#define ERROR__VARDEC_LONG__VALUE_EXPRESSION_NOT_LEGAL      37
#define ERROR__VARDEC_LONG__OPERANDS_TYPE_NOT_MATCH         38

#define ERROR__FUNCDEC_PROCEDURE__PARAM_TYPE_NOT_DEFINED    39
#define ERROR__FUNCDEC_PROCEDURE__BODY_ILLEGAL              40
#define ERROR__FUNCDEC_PROCEDURE__BODY_TYPE_NOT_NORETURN    41

#define ERROR__FUNCDEC_FUNCTION__PARAM_TYPES_NOT_DEFINED  	42
#define ERROR__FUNCDEC_FUNCTION__RETURN_TYPE_NOT_DEFINED  	43
#define ERROR__FUNCDEC_FUNCTION__BODY_NOT_LEGAL             44
#define ERROR__FUNCDEC_FUNCTION__RETURN_TYPE_NOT_MATCH    	45

#define ERROR__WHILE__CONDITION_NOT_TYPE_INT                46
#define ERROR__WHILE__BODY_NOT_TYPE_NORETURN                47

#define ERROR__NEGATIVE__EXPRESSION_NOT_TYPE_INT            48

#define ERROR__FUNCTION_CALL__PARAM_TYPES_NOT_MATCH         49
#define ERROR__RECURSIVE__ILLEGAL                           50
#define ERROR__BREAK__NOT_INSIDE_LOOP                       51
#define ERROR__FOR_LOOP_VAR__CAN_NOT_BE_ASSIGNED            52



/**
 *  @brief  set an error code.
 */
void MyError_pushErrorCode(unsigned errorCode);

/**
 *  @brief  set an error code along with a msg.
 */
void MyError_pushErrorCodeWithMsg(unsigned errorCode, char* msg);

/**
 *  @brief  get the error code positioned by `index`.
 */
unsigned MyError_getErrorCode(unsigned index);

/**
 *  @brief  get the error message positioned by `index`.
 *  @return return NULL if none;
 *          otherwise, return a pointer to the msg string.
 */
char* MyError_getErrorMsg(unsigned index);

/**
 *  @brief  get the total error count.
 */
unsigned MyError_getErrorCount(void);

/**
 *  @brief  get the capacity of error stack.
 */
unsigned MyError_getErrorCapacity(void);

/**
 *  @brief  check if any error setted.
 */
bool MyError_isErrorSet(void);


/**
 *  @brief  clear all setted error codes.
 */
void MyError_clearErrors(void);

/**
 *  @brief  print an error message indicated by the former setted error code.
 *  @remark after all errors printed, errors are cleared.
 */
void MyError_printErrors(void);

#endif