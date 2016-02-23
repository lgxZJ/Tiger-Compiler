#ifndef MYSYMBOL_H
#define MYSYMBOL_H

//
//	Symbol module:
//
//	mySymbol is just a address of mySymbol_ structure
//		containing symbol names of language source file
//	By using this pointer it makes up a symbol list
//

#include "myTable.h"

typedef char*	myString;

typedef struct mySymbol_
{	
	myString		name;
	struct mySymbol_*	next;
}* mySymbol;

//	make a symbol from a string and add
mySymbol MySymbol_MakeSymbol(myString str);

//	get the name string of a certain symbol
myString MySymbol_GetName(const mySymbol symbol);

//	make a new emmpty table
myTable MySymbol_MakeNewTable();

//	make a binder enter s table
bool MySymbol_Enter(myTable table, const mySymbol symbol, void* const value);

//	look up the most recent binding of "symbol" in table, return value
void* MySymbol_Look(const myTable table, const mySymbol symbol);

//	start a new scope in table
bool MySymbol_BeginScope(myTable table);

//	remove any bindings entered since the current scope begin,	
//	and end the current scope
void MySymbol_EndScope(myTable table);

#endif
