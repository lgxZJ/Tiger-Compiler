#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "myTypes.h"

/*==========================================================================
                            type checkers
==========================================================================*/

/**
 *  @brief  check is the given record contains field named `field`.
 *
 *  @return If found, return the type of field; otherwise, NULL.
 */
myType checkType_RecordContainsFields(myTypeRecord typeRecord, mySymbol field);

//  NOTE:   The caller make sures `type` is valid.
bool isTypeInt(myType type);
bool isTypeString(myType type);
bool isTypeNil(myType type);
bool isTypeNoReturn(myType type);
bool isTypeArray(myType type);
bool isTypeRecord(myType type);
bool isTypeNamed(myType type);


bool isTypeEqual(myType lhs, myType rhs);

#endif