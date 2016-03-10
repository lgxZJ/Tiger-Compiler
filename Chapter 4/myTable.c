#include "myTable.h"
#include <assert.h>
#include <string.h>

//	todo: change all assert into check codes


/*	hash function (used internally)
 *	Note:
 *		same string must return same result,
 *		but different string may return same too.
 */
static int hashFunc_MyTable(void* const key)
{ 
	//	if key is a numm-pointer, return -1 for failure.
	//	if key points to an empty string, return -1.
	if (key == (void*)NULL || (*(char const*)key) == '\0')	return -1;
	else			return (*(char const*)key) % TAB_SIZE;
}

/*	
 *	make a new binder and set its fields appropriately(usedinternally)	
 */
static myBinder makeMyBinder(void* const key, void* const value, const myBinder next, void* const prevTop)
{
	//	return NULL if key equals to 0 whick means nothing
	if (key == (void*)NULL || (*(char*)key) == '\0')
		return (myBinder)NULL;
	//	parameters checked by the caller
	
	myBinder binder = myCheckedMalloc(sizeof(*binder));
	binder->key = key;
	binder->value = value;
	binder->next = next;
	binder->prevTop = prevTop;
	
	return binder;
}

myTable	MyTable_MakeEmptyTable_()
{
	myTable	table = myCheckedMalloc(sizeof(*table));
	if (table == NULL)	return NULL;	

	//	initialize
	table->top		= NULL;
	for (unsigned i = 0; i < TAB_SIZE; ++i)
		table->binder[i] = NULL;
	
	return table;
}


//	return true if success, otherwise false
bool MyTable_Enter_(myTable table, void* const key, void* const value)
{
	//	guard if
	if (table == NULL || key == NULL || value == NULL)
		return false;	

	int index = hashFunc_MyTable(key);
	if (index == -1)	return false;
	
	table->binder[index] =
		makeMyBinder(key, value, table->binder[index], table->top);
	table->top = key;

	return true;
}

//	return NULL for invalid params and not found
//	return key's value if found
void* MyTable_Look_(const myTable table, void* const key)
{
  //	return NULL if income invalid params  
  if (table == NULL || key == NULL || (*(char*)key) == '\0')
    return NULL;
 
  //	the statement above this line assures that hashFunc_MyTable() won't return -1
  myBinder binder = table->binder[hashFunc_MyTable(key)];
  while (binder)
    {
      if (strcmp(key, binder->key) == 0)	return binder->value;
      else								binder = binder->next;
    }
	 
  return NULL;
}

/*	must not called when there no binder inside table	*/
void* MyTable_Pop_(myTable table)
{
  if (table == NULL || table->top == NULL)
    return NULL;
  
  //	former gard if assures index's validity
  unsigned index = hashFunc_MyTable(table->top);
  myBinder binder = table->binder[index];
  assert(binder);
	
  table->binder[index] = binder->next;
  table->top = binder->prevTop;
	
  return binder->key;
}


void MyTable_Dump_(const myTable table, ShowFunc show)
{
  if (table == NULL || show == NULL)	return;
	
  while (table->top)
  {
      //	while loop assures index's validity
      unsigned index = hashFunc_MyTable(table->top);
      //	show one
      show(table->binder[index]->key, table->binder[index]->value);
		
      //	pop one
      table->top = table->binder[index]->prevTop;
      table->binder[index] = table->binder[index]->next;
  }
}

