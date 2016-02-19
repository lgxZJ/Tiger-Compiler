#include "introduce.h"

//ststement constructors
A_stm A_CompoundStm (A_stm stm1, A_stm stm2)
{
	A_stm s = (A_stm)checked_malloc (sizeof (*s));
	s->kind = A_compoundStm;
	s->u.compound.stm1 = stm1;
	s->u.compound.stm2 = stm2;
	return s;
}

A_stm A_AssignStm (string id, A_exp exp)
{
	A_stm s = (A_stm)checked_malloc (sizeof (*s));
	s->kind = A_assignStm;
	s->u.assign.id	= id;
	s->u.assign.exp	= exp;
	return s;
}

A_stm A_PrintStm (A_expList exps)
{
	A_stm s = (A_stm)checked_malloc (sizeof (*s));
	s->kind = A_printStm;
	s->u.print.exps = exps;
	return s;
}

//expression constructors
A_exp A_IdExp (string id)
{
	A_exp ptr= (A_exp)checked_malloc (sizeof (*ptr));
	ptr->kind = A_idExp;
	ptr->u.id = id;
	return ptr;
}

A_exp A_NumExp (int num)
{
	A_exp ptr = (A_exp)checked_malloc (sizeof (*ptr));
	ptr->kind = A_numExp;
	ptr->u.num = num;
	return ptr;
}

A_exp A_OpExp (A_exp left, A_binop oper, A_exp right)
{
	A_exp ptr = (A_exp)checked_malloc (sizeof (*ptr));
	ptr->kind = A_opExp;
	ptr->u.op.left = left;
	ptr->u.op.oper = oper;
	ptr->u.op.right = right;
	return ptr;
}

A_exp A_EseqExp (A_stm stm, A_exp exp)
{
	A_exp ptr = (A_exp)checked_malloc (sizeof (*ptr));
	ptr->kind = A_eseqExp;
	ptr->u.eseq.stm = stm;
	ptr->u.eseq.exp = exp;
	return ptr;
}

//expression list constructors
A_expList A_PairExpList (A_exp head, A_expList tail)
{
	A_expList ptr = (A_expList)checked_malloc (sizeof (*ptr));
	ptr->kind = A_pairExpList;
	ptr->u.pair.head = head;
	ptr->u.pair.tail = tail;
	return ptr;
}

A_expList A_LastExpList (A_exp last)
{
	A_expList ptr = (A_expList)checked_malloc (sizeof (*ptr));
	ptr->kind = A_lastExpList;
	ptr->u.last = last;
	return ptr;
}
