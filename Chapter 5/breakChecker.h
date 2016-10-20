#ifndef BREAK_CHECKER_H
#define BREAK_CHECKER_H

#include <stdbool.h>

void enterLoop(void);
void leaveLoop(void);
bool isBreakInsideLoop(void);

#endif