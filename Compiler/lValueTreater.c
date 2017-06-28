#include "lValueTreater.h"
#include "mySymbol.h"

#include <assert.h>

/////////////////////////////////////////////

//  treat lvalue as content by default 
//static int g_asContent = 1;
static myTable g_treater = 0;
static void* const KEY = (void*)9;
static void* const CONTENT = (void*)1;
static void* const ADDRESS = (void*)2;

/////////////////////////////////////////////

static void initTable()
{
    g_treater = MyTable_MakeEmptyTable_();
    MyTable_Enter_(g_treater, KEY, CONTENT);
}

void treatLValueAsAddress(void)
{
    if (g_treater == 0) initTable();

    MySymbol_BeginScope(g_treater);
    MyTable_Enter_(g_treater, KEY, ADDRESS);
}

void treatLValueAsContent(void)
{
    if (g_treater == 0) initTable();
    
    MySymbol_BeginScope(g_treater);
    MyTable_Enter_(g_treater, KEY, CONTENT);
}

void cancelTreatAsAddress(void)
{
    assert (g_treater != 0);

    MySymbol_EndScope(g_treater);
}

void cancelTreatAsContent(void)
{
    assert (g_treater != 0);
    
    MySymbol_EndScope(g_treater);
}

bool isLValueAsAddress(void)
{
    if (g_treater == 0) initTable();
    return MyTable_Look_(g_treater, KEY) == ADDRESS;
}

bool isLValueAsContent(void)
{
    if (g_treater == 0) initTable();
    return MyTable_Look_(g_treater, KEY) == CONTENT;
}
