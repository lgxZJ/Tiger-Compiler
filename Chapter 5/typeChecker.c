#include "typeChecker.h"
#include "typeMaker.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////

myType checkType_RecordContainsFields(myTypeRecord typeRecord, mySymbol field)
{
    if (typeRecord == NULL || typeRecord->fieldList == NULL)  
        return NULL;
    if (field == NULL || MySymbol_GetName(field) == NULL)
        return makeType_NoReturn();//   todo:
    
    myTypeFieldList typeFieldList = typeRecord->fieldList; 
    while(typeFieldList)
    {
        if (MySymbol_IsSymbolEqual(typeFieldList->head->name, field))
            return typeFieldList->head->type;
        typeFieldList = typeFieldList->tails;
    }
    
    return NULL;
}

///////////////////////////////////////////////////////////////////////

bool isTypeInt(myType type)     { return type->kind == TypeInt; }
bool isTypeString(myType type)  { return type->kind == TypeString; }
bool isTypeNil(myType type)     { return type->kind == TypeNil; }
bool isTypeNoReturn(myType type){ return type->kind == TypeNoReturn; }
bool isTypeArray(myType type)   { return type->kind == TypeArray; }
bool isTypeRecord(myType type)  { return type->kind == TypeRecord; }
bool isTypeNamed(myType type)   { return type->kind == TypeNamed; }

////////////////////////////////////////////////////////////////////////////

//  DO:
//      1.check type id.
bool isArrayEqual_(myTypeArray lhs, myTypeArray rhs)
{
    return  lhs->uniqueId == rhs->uniqueId;
}

//  DO:
//      1.check type id.
bool isRecordEqual_(myTypeRecord lhs, myTypeRecord rhs)
{
    return lhs->uniqueId == rhs->uniqueId;
}

//  DO:
//      1.check actual type.
bool isNamedEqual_(myTypeNamed lhs, myTypeNamed rhs)
{
    return isTypeEqual(lhs->type, rhs->type);
}

bool isTypeEqual(myType lhs, myType rhs)
{
    switch (lhs->kind)
    {
        case TypeInt:       return isTypeInt(rhs);
        case TypeString:    return isTypeString(rhs);
        case TypeNil:       return isTypeNil(rhs);
        case TypeNoReturn:  return isTypeNoReturn(rhs);
        case TypeArray:     
            return  isTypeArray(rhs) && 
                    isArrayEqual_(lhs->u.typeArray, rhs->u.typeArray);
        case TypeRecord:
            return  isTypeRecord(rhs) && 
                    isRecordEqual_(lhs->u.typeRecord, rhs->u.typeRecord);
        case TypeNamed:
            return  isTypeNamed(rhs) &&
                    isNamedEqual_(lhs->u.typeNamed, rhs->u.typeNamed);
        default:
            assert(false);
    }
}
