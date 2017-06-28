#ifndef BREAK_GETTER_H
#define BREAK_GETTER_H

#include "myTemp.h"


//  todo: remove target limits
void    pushBreakTarget(myLabel targetLabel);
void    popBreakTarget(void);// a pair of functions
myLabel getBreakTarget(void);// use this to get


#endif