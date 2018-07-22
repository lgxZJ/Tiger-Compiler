#ifndef MY_CXXTEMP_H
#define MY_CXXTEMP_H

#include "CXX_mySymbol.h"

extern "C" {

//  abstract names for local variables
typedef struct myTemp_	*myTemp;
//  abstract names for static memory addresses
typedef mySymbol	    myLabel;



/// \brief      compare two labels if they are equal.
/// \param[in]  lhs     left operand.
/// \param[in]  rhs     right operand.
/// \return     a bool variable. 
bool Temp_isLabelEqual(myLabel lhs, myLabel rhs);

/**
 *  @brief  make a new temporary in register representation.
 *  @param  this function has no parameters.
 *  @return a new temporary register representation.
 */
myTemp 	Temp_newTemp(void);

/**
 *  @brief  make a new label for function address.
 *  @param  this function has no parameters.
 *  @return a new label for function address.
 */
myLabel Temp_newLabel(void);

///
/// @brief      make a new label of the given name.
/// @param[in]  name    the name of the label.
/// @return     a new label.
/// @remark     if a label with the given name is already made, then
///             this function simply returns that one. 
myLabel Temp_newNamedLabel(myString name);

/// \brief      get the label string.
/// \param[in]  label   the label whose string is to be obtained.
/// \return     the string of the given label.
myString Temp_getLabelString(myLabel label);

/// \brief      get the number of the given temporary.
/// \param[in]  temp    a given temp.
/// \return     the number of the given temp.
int Temp_getTempNum(myTemp temp);

////////////////////////

typedef struct myTempList_
{
    myTemp 		head;
    struct myTempList_*	tail;
}* myTempList;

///
/// \brief  a temp list maker.
///
myTempList Temp_makeTempList(myTemp head, myTempList tail);


typedef struct myLabelList_
{
    myLabel			head;
    struct myLabelList_*	tail;
}* myLabelList;

///
/// \brief  a label list maker.
///
myLabelList Temp_makeLabelList(myLabel head, myLabelList tail);

//////////////////////////////////////////////////////////////////
//	more needed, but not now

}

#endif
