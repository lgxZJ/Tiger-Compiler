#ifndef MYTABLE_H
#define MYTABLE_H

/*
 *	Generic hash table module:
 *		No algorithm should use these functions directly, because
 *		programming with void* is too error-prone.	Instead, each
 *		module should make "wrapper" functions that take well-typeded
 *		arguments and call the _ functions.
 */
 
 #include "makeMemory.h"
 #include <stdbool.h>
 
 #define		TAB_SIZE		127
 
typedef struct myBinder_
{
	void*		key;		//	key:	a pointer to string containing name
	void*		value;		//	value:	a kind of Exp type which is a pointer of struct actually
	struct	myBinder_*	next;		//	next:	a pointer points to next binder
	void*		prevTop;	//	a previous top key
}*	myBinder;
 
typedef struct myTable_
{
	myBinder	binder[TAB_SIZE];
	void*		top;//	top key
}*	myTable;


//	make a new empty table mapping "key -> value"s
myTable	MyTable_MakeEmptyTable_();

//	enter a new binder into a table
bool MyTable_Enter_(myTable table, void* const key, void* const value);

//	look a key most recently entered inside a table, return its value
void* MyTable_Look_(const myTable table, void* const key);

//	pop the top binding inside a table, return its key
void* MyTable_Pop_(myTable table);


//	the caller makes params valid
typedef void (*ShowFunc)(void* key, void* value);
//	dump table using a giving function
void MyTable_Dump_(const myTable table, ShowFunc show);

#endif
