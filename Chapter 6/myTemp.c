#include "myTemp.h"
#include "mySymbol.h"

#include <stdio.h> //  for sprintf()
#include <assert.h> //  for assert()

//  keep track of myTemp and myLabel numbers

struct myTemp_  { int num; };

//////////////////////////////////////////////////////////////
//                      global variables
static int g_tempCount = 0;

int Temp_getTotalTempCount(void)     { return g_tempCount; }


static int g_labelCount = 0;

int Temp_getTotalLabelCount(void)   { return g_labelCount; }


/////////////////////////////////////////////////////////////
//                      functions

bool Temp_isLabelEqual(myLabel lhs, myLabel rhs)
{
    return MySymbol_IsSymbolEqual(lhs, rhs);
}

myTemp 	Temp_newTemp(void)
{
    myTemp temp = makeMemoryBlock(sizeof(*temp), MEMORY_TYPE_NONE);
    assert (temp);

    temp->num = ++g_tempCount;
    return temp;
}

myLabel Temp_newLabel(void)
{
    char* buffer = makeMemoryBlock(sizeof(char) * 20, MEMORY_TYPE_NONE);
    assert (buffer);

    sprintf(buffer, "Label%d", ++g_labelCount);
    return Temp_newNamedLabel(buffer);
}

//  NOTE:   try to call Temp_newLabel() instead, because it do not
//  count correctly.
myLabel Temp_newNamedLabel(myString name)
{
    return MySymbol_MakeSymbol(name);
}

myString Temp_getLabelString(myLabel label)
{
    return MySymbol_GetName(label);
}

myTempList Temp_makeTempList(myTemp head, myTempList tail)
{
    myTempList labels = makeMemoryBlock(sizeof(*labels), MEMORY_TYPE_NONE);
    assert (labels);

    labels->head = head;
    labels->tail = tail;
    return labels;
}

myLabelList Temp_makeLabelList(myLabel head, myLabelList tail)
{
    myLabelList labels = makeMemoryBlock(sizeof(*labels), MEMORY_TYPE_NONE);
    assert (labels);

    labels->head = head;
    labels->tail = tail;
    return labels;
}