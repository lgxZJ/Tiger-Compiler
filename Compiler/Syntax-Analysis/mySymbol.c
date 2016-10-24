#include "mySymbol.h"
#include "../Commom/makeMemory.h"

#include <string.h>
#include <assert.h>

#define HASH_SIZE	109	/* Should be prime? */


struct mySymbol_
{	
	myString		name;
	mySymbol		next;
};


/* The hash table holding all symbols (used internally) */
static mySymbol	hashTable[HASH_SIZE] = {0};

/*
 *	NOTE:
 *	Because any symbol be of NULL or "" is not a valid symbol
 *	and makes no meanings when making structures, so check this
 *	circustoms first.
 *	(Used internally)
 */
bool MySymbol_InvalidSymbol(mySymbol symbol)
{
    return ((symbol == (mySymbol)NULL) ||
			(*(char*)MySymbol_GetName(symbol) == '\0'))
		? true
		: false;
}

bool MySymbol_IsSymbolEqual(mySymbol lhs, mySymbol rhs)
{
	assert (!MySymbol_InvalidSymbol(lhs));
	assert (!MySymbol_InvalidSymbol(rhs));
	return lhs == rhs;
}

/*
 *	Make a symbol (used internally)
 */
mySymbol makeSymbol_(myString name, mySymbol next)
{
	//	next could be NULL
	if (name == NULL || (*(char*)name) == '\0')
		return NULL;

	mySymbol one = makeMemoryBlock(sizeof(*one), MEMORY_TYPE_NONE);
	
	one->name = name;
	one->next = next;
	
	return one;
}

/*
 *	The hash function to get a hash value (used internally)
 */
int hashFunc_MySymbol(char* c)
{
	/* return -1 if failed, otherwise positive */
	if (c == NULL || (*c) == '\0')	return -1;
	return (*c) % HASH_SIZE;
}

/*  
 *		make and add a symbol in the list
 *	@return	NULL for invalid param, a symbol for valid param
 */
mySymbol MySymbol_MakeSymbol(myString name)
{
	//	make sure name is valid
	if (name == NULL || (*(char*)name) == '\0')
		return NULL;

	/*	search if already exists	*/
	int index = hashFunc_MySymbol((char*)name) % HASH_SIZE;
	for (mySymbol i = hashTable[index]; i != NULL; i = i->next)
	{
		//	if name is valid, compare it
		if (i->name && strcmp(i->name, name) == 0)	return i;
	}
	
	/*	add a new one in the list	*/
	mySymbol	tmp = makeSymbol_(name, hashTable[index]);
	hashTable[index] = tmp;
	
	return tmp;
}

myString MySymbol_GetName(const mySymbol symbol)
{
	assert (symbol);
	return symbol->name;
}

/*
 *	NOTE:
 *		following function are delegations, and parameters are checked
 *		by the cellee.
 */

/*	make a new empty table	*/
myTable MySymbol_MakeNewTable()
{
	return MyTable_MakeEmptyTable_();
}

/*	make a binder enter into a table	*/
void MySymbol_Enter(myTable table, const mySymbol symbol, void* const value)
{
	assert (table);
	assert (!MySymbol_InvalidSymbol(symbol));
    MyTable_Enter_(table, symbol, value);
}

/*	return value of key "symbol", or NULL */
void* MySymbol_Look(const myTable table, const mySymbol symbol)
{
	assert (table);
	assert (!MySymbol_InvalidSymbol(symbol));
    return MyTable_Look_(table, symbol);
}

bool MySymbol_Set(const myTable table, mySymbol symbol, void* value)
{
	assert (table);
	assert (!MySymbol_InvalidSymbol(symbol));
	return MyTable_Set_(table, symbol, value);
}

/*	mark of scope (used internally)	*/
struct mySymbol_ scopeMark_ = { "scope", NULL };
static const mySymbol scopeMark = &scopeMark_;

/*	start a new scope	*/
void MySymbol_BeginScope(myTable table)
{
	assert (table);
    MySymbol_Enter(table, scopeMark, scopeMark);
}

/*	end the current scope (including remove binders)	*/
void MySymbol_EndScope(myTable table)
{
	//	there msust be a scope already, caller assures it
	mySymbol sym;
	//      end loop once a pop fails
	do
	  sym = MyTable_Pop_(table);
	while (sym != NULL && sym != scopeMark);
}

////////////////////////////////////////////////////////////////////
