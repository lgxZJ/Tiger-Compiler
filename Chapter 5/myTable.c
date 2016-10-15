#include "myTable.h"
#include <assert.h>
#include <string.h>

//-----------------------------typedefs---------------------------------//

struct myBinder_
{
	void*		key;		//	key:	a pointer to string containing name
	void*		value;		//	value:	a kind of Exp type which is a pointer of struct actually
	myBinder	next;//	next:	a pointer points to next binder
	void*		prevTop;	//	a previous top key
};

struct myTable_
{
	myBinder	binder[TAB_SIZE];
	void*		top;//	top key
};

//-------------------------------functions--------------------------------//

/*	hash function (used internally)
 *	Note:
 *		same key must return same result,
 */
static int hashFunc_MyTable(void* const key)
{ 
    //	if key is a null-pointer, return -1 for failure.
    if (key == (void*)NULL)		return -1;
    else						return ((unsigned)key) % TAB_SIZE;
}

/*	
 *	make a new binder and set its fields appropriately(usedinternally)	
 */
static myBinder makeMyBinder(void* const key, void* const value, const myBinder next, void* const prevTop)
{
	//	return NULL if key equals to 0 whick means nothing
	if (key == NULL)    return (myBinder)NULL;
	//	parameters checked by the caller
	
	myBinder binder = makeMemoryBlock(sizeof(*binder), MEMORY_TYPE_NONE);
	binder->key = key;
	binder->value = value;
	binder->next = next;
	binder->prevTop = prevTop;
	
	return binder;
}

myTable	MyTable_MakeEmptyTable_()
{
    myTable	table = makeMemoryBlock(sizeof(*table), MEMORY_TYPE_NONE);
    assert (table);	

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
	if (table == NULL || key == NULL)
		return false;	

	myBinder previousBinder;
	void* previousTop;
	if (table->top == NULL)		//	empty
		previousBinder = previousTop = NULL;
	else
	{
		int previousSlotIndex = hashFunc_MyTable(table->top);
		previousBinder = table->binder[previousSlotIndex];
		previousTop = table->top;
	}
	myBinder newBinder = makeMyBinder(
		key, value, previousBinder, previousTop);

	int currentSlotIndex = hashFunc_MyTable(key);
	table->binder[currentSlotIndex] = newBinder;
	table->top = key;

	return true;
}

//	return NULL for invalid params and not found
//	return key's value if found
void* MyTable_Look_(const myTable table, void* const key)
{
	//	return NULL if income invalid params  
	if (table == NULL || key == NULL)
		return NULL;
 
	//	the statement above this line assures that hashFunc_MyTable() won't return -1
	myBinder binder = table->binder[hashFunc_MyTable(key)];
	while (binder)
    {
        //  key are actually pointers, so just compare them.
		if (key == binder->key)	return binder->value;
		else					binder = binder->next;
    }
	 
	return NULL;
}

bool MyTable_Set_(myTable table, void* const key, void* const value)
{
	//	return NULL if income invalid params  
	if (table == NULL || key == NULL)
		return false;
 
	//	the statement above this line assures that hashFunc_MyTable() won't return -1
	myBinder binder = table->binder[hashFunc_MyTable(key)];
	while (binder)
    {
        //  key are actually pointers, so just compare them.
		if (key == binder->key)
		{
			binder->value = value;
			return true;
		}
		else
			binder = binder->next;
    }
	 
	return false;
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
	
  return binder->value;
}

/*
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
}*/

