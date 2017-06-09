#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <stdio.h>
#include "../../myFrame.h"
#include "../../myIRTree.h"

void printExpression(FILE* file, IR_myExp exp, int spaceNum);
void printStatement(FILE* file, IR_myStatement statement, int spaceNum);
void printFragments(FILE* file, Frame_myFragList fragments);

#endif