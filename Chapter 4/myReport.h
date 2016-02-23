#ifndef MYREPORT_H
#define MYREPORT_H

#include "stdbool.h"


typedef struct
{
  int	line;
  int	column;
}PosInfo;

void	resetPos();
void 	recordTokenPos		(bool newLine, int textLength);
PosInfo getCurrentPosInfo	();
int 	outputReport		(int tokenNum, int optionalValue, char* optionalText);
int 	errorReport		();

#endif
