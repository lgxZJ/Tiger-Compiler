/**
 *	@file		introduce.h
 *	@warning	Only used for roughly seeing.
 */
#ifndef INTRODUCE_H
#define INTRODUCE_H

#include "util.h"

/**
 *	@brief	define type \c string from \c char*.
 */
typedef char *string;
/**
 *	@brief	define type \c A_stm to a pointer of \c A_stm_ structure.
 */
typedef struct A_stm_	*A_stm;
/**
 *	@brief	define type \c A_exp to a pointer of \c A_exp_ structure.
 */
typedef struct A_exp_	*A_exp;
/**
 *	@brief	define type \c A_expList to a pointer of \c A_expList_ structure.
 */
typedef struct A_expList_	*A_expList;
/**
 *	@brief	 a typedef of enum.
 */
typedef enum { A_plus, A_minus, A_times, A_div }	A_binop;

/**
 *	@brief	definition of statements.
 */
struct A_stm_
{
	/**
	 *	@brief	kinds of statements.
	 */
	enum { A_compoundStm, A_assignStm, A_printStm }	kind;
	union { struct { A_stm stm1, stm2; }		compound;	/**<	compound statement	*/
		struct { string id; A_exp exp; }	assign;		/**< assignment statement */
		struct { A_expList exps; }			print; 		/**<	print statements	*/
	}u;
};

/**
 *	@brief	make a compound statement.
 */
A_stm A_CompoundStm (A_stm stm1, A_stm stm2);
/**
 *	@brief	make a assignment statement.
 */
A_stm A_AssignStm (string id, A_exp exp);
/**
 *	@brief	make a print statement.
 */
A_stm A_PrintStm (A_expList exps);

/**
 *	@brief	expression structure.
 */
struct A_exp_
{
	/**
	 *	@brief	kinds of containing union.
	 */
	enum { A_idExp, A_numExp, A_opExp, A_eseqExp, }		kind;
	union { string 	id;
			int 	num;
			struct { A_exp left; A_binop oper; A_exp right; }	op;
			struct { A_stm stm; A_exp exp; }					eseq;
	}u;
};

/**
 *	@brief	make an ID expression.
 */
A_exp A_IdExp (string id);
/**
 *	@brief	make an Number expression.
 */
A_exp A_NumExp (int num);
/**
 *	@brief	make an Operation expression.
 */
A_exp A_OpExp (A_exp left, A_binop oper, A_exp right);
/**
 *	@brief	make an Sequence expression.
 */
A_exp A_EseqExp (A_stm stm, A_exp exp);

/**
 *	@brief	expression list structure.
 */
struct A_expList_
{
	/**
	 *	@brief	kinds of containing union.
	 */
	enum { A_pairExpList, A_lastExpList }	kind;
	union { struct { A_exp head; A_expList tail; }	pair;
	A_exp	last;
	}u;
};

/**
 *	@brief	make a Pair expression list.
 */
A_expList A_PairExpList (A_exp head, A_expList tail);
/**
 *	@brief	make a Last expression list.
 */
A_expList A_LastExpList (A_exp last);

#endif
