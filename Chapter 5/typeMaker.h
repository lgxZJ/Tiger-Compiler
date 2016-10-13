#ifndef TYPE_MAKER_H
#define TYPE_MAKER_H

#include "myTypes.h"
#include "myAbstractSyntax.h"

/**
 *  @file   myTypes.h
 *  @author lgxZJ@outlook.com
 *  @date   9/18/2016
 *  
 *  @brief  Module about types in tiger language.
 *
 *          These types will be used while doing semantic analyzing.
 *          Every type carries its own information, for example, 
 *          Name type carries the new type name and the type of it; 
 *
 *  @warning    The test cases may not be enough, if some bugs
 *				were found, please contact me!
 */

#define ERROR_MAKETYPE   -1

/**----------------------------------------------------------------------
                    make functions for type structures
-----------------------------------------------------------------------**/

myType makeType_Int(void);
myType makeType_String(void);
myType makeType_Nil(void);
myType makeType_NoReturn(void);

myType makeType_Record(myTypeFieldList fields);
myType makeType_Array(myType type);
myType makeType_Named(mySymbol name, myType type);

myTypeField     makeType_TypeField(mySymbol name, myType type); 
myTypeFieldList makeType_TypeFieldList(myTypeField head, myTypeFieldList tails);

myTypeList  makeType_TypeList(myType head, myTypeList tails);

 #endif