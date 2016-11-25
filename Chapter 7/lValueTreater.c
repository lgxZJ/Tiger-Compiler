#include "lValueTreater.h"

/////////////////////////////////////////////

static bool g_asContent = false;

/////////////////////////////////////////////

void treatLValueAsAddress(void)
{
    g_asContent = false;
}

void treatLValueAsContent(void)
{
    g_asContent = true;
}

bool isLValueAsAddress(void)
{
    return !isLValueAsContent();
}

bool isLValueAsContent(void)
{
    return g_asContent;
}