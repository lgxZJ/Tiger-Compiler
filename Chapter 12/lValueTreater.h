#ifndef LVALUE_TREATER_H
#define LVALUE_TREATER_H

#include <stdbool.h>

void treatLValueAsAddress(void);
void treatLValueAsContent(void);

void cancelTreatAsAddress(void);
void cancelTreatAsContent(void);

bool isLValueAsAddress(void);
bool isLValueAsContent(void);

#endif