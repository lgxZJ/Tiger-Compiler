/**
 *	@file		program.c
 *	@brief		main source file.
 *	@author		lgxZJ@outlook.com
 *	@date		03/05/2016
 *	@warning	This source is not properly documented.
 *	@note		Trying.
 *	@bug		Due to my less attention, some such as @ref tree structure,
 *				@ref insert() function, e.g are \b not \b correct. \b Do
 *				\b not \b use!!
 */
#include"introduce.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/**
 *	@brief	define \c T_tree to a pointer of \c tree structure.
 */
typedef struct tree *T_tree;
/**
 *	@brief	define \c tree structure.
 *	@warning	Not properly defined. Do not use it!
 */
struct tree { T_tree left; string key; T_tree right; };

/**
 *	@brief	make a new tree from an older one.
 *	@param	l		The left node of tree.
 *	@param	r		The right node of tree.
 *	@param	k		A string.		
 *	@return	A new tree.
 *	@warning	Not properly defined. Do not use it!
 */
T_tree Tree (T_tree l, string k, T_tree r)
{
	T_tree t = (T_tree)checked_malloc (sizeof (*t));
	t->left = l;
	t->key = k;
	t->right = r;
	return t;
}

/**
 *	@brief	insert a key into a binary tree.
 *	@param	key		The key to be inserted.
 *	@param	t		The table into which the key is inserted.
 *	@return	A new table.
 *	@warning	Not properly defined. Do not use it!
 */
T_tree insert (string key, T_tree t)
{
	if (t == NULL)	return Tree(NULL, key, NULL);
	else if (strcmp (key, t->key) < 0)
		return Tree(insert(key, t->left), t->key, t->right);
	else if (strcmp (key, t->key) > 0)
		return Tree(t->left, t->key, insert(key, t->right));
	else if (strcmp (key, t->key) == 0)
		return Tree(t->left, t->key, t->right);
}

/**
 *	@brief	determine whether a given key could be found inside a tree.
 *	@param	t		The table in which the key is searched.
 *	@param	key		The key to search.
 *	@return	TRUE if found, FALSE otherwise.
 *	@warning	Not properly defined. Do not use it!
 */
int find (T_tree t, string key)

{
	if (t == NULL)	return FALSE;
	if (strcmp (key, t->key) == 0)	
		return TRUE;
	else if (strcmp (key, t->key) < 0)
		return find (t->left, key);
	else if (strcmp (key, t->key) > 0)
		return find (t->right, key);
}

/*	a forward declaration.	*/
int maxargs (A_stm stm);

/**
 *	@brief	return numbers of expressions which can be treated as
 *			parameter.
 *	@param	ptr			An expression to count parameters.
 *	@param	otherMaxPtr	A pointer to an caller int variable where max
 *			arguments is stored. If this counter is not greater than
 *			former one, this value is not changed.
 *			Since @ref subroutine_Exp() calls @ref maxargs() inside,
 *			we have to pass this parameter for result storing.
 *	@return	Count of arguments of this expression.
 */
int subRoutine_Exp (A_exp ptr, int *otherMaxPtr)
{
	int count = 0;
	//increment count by one
	if (ptr->kind == A_idExp ||
		ptr->kind == A_numExp ||
		ptr->kind == A_opExp)
		count = 1;

	if (ptr->kind == A_eseqExp)
	{
		A_stm	temp;
		temp = ptr->u.eseq.stm;
		
		//consider assign statement as one parameter inside sequence expression
		if (temp->kind == A_assignStm)
			count++;
		//compute max args in this condition and return if is larger
		if (temp->kind == A_printStm)
			maxargs (ptr->u.eseq.stm) > *otherMaxPtr ?
				*otherMaxPtr = maxargs (ptr->u.eseq.stm) :
				0,
			++count;
		//no possible of compound statement inside sequence expression
		if (temp->kind == A_compoundStm)
			fprintf (stderr, "syntax error!"),
			exit (1);
		//add remained parameter numbers
		count += subRoutine_Exp (ptr->u.eseq.exp, otherMaxPtr);
	}
	return count;
}

/**
 *	@brief	Count the max arguments of a given statement.
 *	@param	stm		A statement to count for.
 *	@return	The max argument counter.
 */
int maxargs (A_stm stm)
{
	if (stm == NULL)	return 0;
	
	int max_args = 0,
		otherMax = 0;
	A_stm ptr = stm;
	if (ptr->kind == A_assignStm)	
		max_args = 0,
		subRoutine_Exp (ptr->u.assign.exp, &otherMax);
	else if (ptr->kind == A_printStm)
	{
		A_expList	expListPtr = stm->u.print.exps;
		A_exp		expPtr = NULL;

lable_expList:
		if (expListPtr->kind == A_lastExpList)
		{
			expPtr = expListPtr->u.last;
			max_args += subRoutine_Exp (expPtr, &otherMax);
		}
		else if (expListPtr->kind == A_pairExpList)
		{
			expPtr = expListPtr->u.pair.head;
			max_args += subRoutine_Exp (expPtr, &otherMax);
			expListPtr = expListPtr->u.pair.tail;
			goto lable_expList;
		}
	}
	else if (ptr->kind == A_compoundStm)
	{
		if (maxargs (ptr->u.compound.stm1) > otherMax ||
			maxargs (ptr->u.compound.stm2) > otherMax)
			otherMax = 	maxargs (ptr->u.compound.stm1) > maxargs (ptr->u.compound.stm2) ?
						maxargs (ptr->u.compound.stm1) :
						maxargs (ptr->u.compound.stm2);
	}
	return 	max_args > otherMax ?
			max_args :
			otherMax;
}

/**
 *	@brief	generate a `stm` for use.
 *
 *	Codes commented in body are alternatively correct `stm`s.
 *	@param	This function has no parameters.
 *	@return	A statement.
 */
A_stm prog(void) {
	/*
return 
 A_CompoundStm(A_AssignStm("a",
                 A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
  A_CompoundStm(A_AssignStm("b",
      A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"),
                 A_LastExpList(A_OpExp(A_IdExp("a"), A_minus, 
                                       A_NumExp(1))))),
              A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
   A_PrintStm(A_PairExpList (A_IdExp("b"), A_PairExpList (A_IdExp("b"), A_LastExpList (A_IdExp("b")))))));
*/
/*
return 
 A_CompoundStm(A_AssignStm("a",
                 A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
  A_CompoundStm(A_AssignStm("b",
      A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"), A_PairExpList (A_IdExp ("b"),
                 A_LastExpList(A_OpExp(A_IdExp("a"), A_minus, 
                                       A_NumExp(1)))))),
              A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
   A_PrintStm(A_LastExpList(A_IdExp("b")))));	*/
  
/*
return 
 A_CompoundStm(A_AssignStm("a",
                 A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
  A_CompoundStm(A_AssignStm("b",
      A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"),
                 A_LastExpList(A_OpExp(A_IdExp("a"), A_minus, 
                                       A_NumExp(1))))),
              A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
	A_CompoundStm (
   A_PrintStm(A_LastExpList(A_IdExp("b"))),
   
   A_PrintStm(A_PairExpList (A_IdExp("b"), A_PairExpList (A_IdExp("b"), A_LastExpList (A_IdExp("b")))))
   )));*/
	return
		A_CompoundStm(A_AssignStm("a",
		A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
		A_CompoundStm(A_AssignStm("b",
		A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"),
		A_LastExpList(A_OpExp(A_IdExp("a"), A_minus,
		A_NumExp(1))))),
		A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
		A_PrintStm(A_LastExpList(A_IdExp("b")))));
} 

/**
 *	@brief	define type \c Table_ to a pointer of \c table structure.
 */
typedef struct table *Table_;
/**
 *	@brief	\c table structure.
 */
struct table { string id; int value; Table_ tail; };
/**
 *	@brief	make a new Table from an older one.
 *
 *	Indeed Table is a pointer to a table structure which makes table
 *	actually a linked list.
 *	@param	id		The new symbol name.
 *	@param	value	The new symbol value.
 *	@param	tail	The old symbol table
 *	@return	The new table.
 */
Table_ Table (string id, int value, struct table *tail)
{
	Table_	t = (Table_)malloc (sizeof (*t));
	t->id = id;
	t->value = value;
	t->tail = tail;
	return t;
}


int lookup (Table_ t, string key);
Table_ interpStm (A_stm s, Table_ t);
Table_ update (string id, int value, Table_ t);

/**
 *	@brief	intended structure to be used for \c interpExp() function.
 */
struct IntAndTable { int i; Table_ t; };
/**
 *	@brief	interpret an expression to its real value.
 *	@param	e		An expression.
 *	@param	t		An table into whcih to search.
 *	@return	An @ref IntAndTable structure containing a result value and a
 *			new table which are already interpreted.
 *	@note	One should use the table inside @ref IntAndTable structure,
 *			not the former table t any more.
 */
struct IntAndTable interpExp (A_exp e, Table_ t)
{
	struct IntAndTable tmp;
	if (e->kind == A_idExp)
	{
		tmp.i = lookup (t, e->u.id);
		tmp.t =  t;
		return tmp;
	}
	if (e->kind == A_numExp)
	{
		tmp.i = e->u.num;
		tmp.t = t;
		return tmp;
	}
	if (e->kind == A_opExp)
	{
		struct IntAndTable temp1 = interpExp (e->u.op.left, t),
					temp2 = interpExp (e->u.op.right, temp1.t),
					ret;
		switch (e->u.op.oper)
		{
		case A_plus:
			ret.i = temp1.i + temp2.i;	break;
		case A_minus:
			ret.i = temp1.i - temp2.i;	break;
		case A_times:
			ret.i = temp1.i * temp2.i;	break;
		case A_div:
			ret.i = temp1.i / temp2.i;	break;
		}
		ret.t = temp2.t;
		return ret;
	}
	if (e->kind == A_eseqExp)
	{
		return interpExp (e->u.eseq.exp, 
						  interpStm (e->u.eseq.stm, t));
	}
}

/**
 *	@brief	interpret a `stm`.
 *	@param	s		A statement.
 *	@param	t		A table to be used when interpreted.
 *	@return	A interpreted table.
 *	@note	One cannot use the former table when this function returns.
 */
Table_ interpStm (A_stm s, Table_ t)
{
	if (s->kind == A_compoundStm)
	{
		/*Table_ result = t;
		AddTable(&result, interpStm(s->u.compound.stm1, result));
		AddTable(&result, interpStm(s->u.compound.stm2, result));*/
		Table_ result = t;
		result = interpStm(s->u.compound.stm1, result);
		result = interpStm(s->u.compound.stm2, result);
		return result;
	}
	if (s->kind == A_assignStm)
	{
		struct IntAndTable result = interpExp(s->u.assign.exp, t);
		return update(s->u.assign.id, result.i, result.t);
	}
	if (s->kind == A_printStm)
	{
		struct IntAndTable	result = { 0, t };
		A_expList	expListPtr = s->u.print.exps;
		do 
		{
			if (expListPtr->kind == A_lastExpList)
				result = interpExp (expListPtr->u.last, result.t);
			else
				result = interpExp (expListPtr->u.pair.head, result.t);
		}
		while  (expListPtr->kind == A_pairExpList &&
				(expListPtr = expListPtr->u.pair.tail));
				
		return result.t;
	}
	assert (0);//should not enter here
}

/**
 *	@brief	update an \c id inside a table \c t with the new \c value.
 *
 *	For more details, see <a href="http:\\www.baidu.com">Tiger Book</a>.
 *	@param	id		The symbol id.
 *	@param	value	The value of symbol.
 *	@param	t		The table to be updated.
 *	@return	An updated table.
 */
Table_ update (string id, int value, Table_ t)
{
	return Table (id, value, t);
}

/**
 *	@brief	loop up with a special key in the given table.
 *	@param	t		A table to look into.
 *	@param	key		The key to look.
 *	@return	Return the value of the key.
 *	@note	The caller assures the key must be in the table.
 */
int lookup (Table_ t, string key)
{
	while (t && strcmp (t->id, key) != 0)
		t = t->tail;
	
	if (t && strcmp (t->id, key) == 0)
		return t->value;
	else assert (0);//should never encounter!!
}

/**
 *	@brief	print table content.
 *
 *	Default print into \b STDOUT.
 *	@param	table		An table containing "key-value" paris.
 *	@return	This function has no return value.
 */
void outputResult (Table_ table)
{
	while (table)
		fprintf (stdout, "ID:\t%s\nValue:\t%i\n", table->id, table->value),
		table = table->tail;
}

int main(int argc, char* argv[])
{
	//	test
	printf ("%4i\n", maxargs(prog()));

	//	test
	Table_ result = interpStm (prog(), NULL);
	outputResult (result);
	getchar ();
	return 0;
}


