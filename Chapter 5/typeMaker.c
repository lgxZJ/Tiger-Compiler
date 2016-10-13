#include "typeMaker.h"
#include "mySymbol.h"

#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static myType wrapToType_(enum TypeKind kind, void* member)
{
    myType type = makeMemoryBlock(sizeof(*type), MEMORY_TYPE_NONE);
    assert (type);

    type->kind = kind;
    switch (kind)
    {
        case TypeInt:   type->u.typeInt = member;   break;
        case TypeNil:   type->u.typeNil = member;   break;
        case TypeNoReturn:type->u.typeNoReturn = member;break;
        case TypeString:type->u.typeString = member;break;
        case TypeArray: type->u.typeArray = member; break;
        case TypeRecord:type->u.typeRecord = member;break;
        case TypeNamed: type->u.typeNamed = member; break;
    }

    return type;
}


static struct myTypeInt_ g_typeInt;
myType makeType_Int(void)
{
    return wrapToType_(TypeInt, &g_typeInt);
}

static struct myTypeString_ g_typeString;
myType makeType_String(void)
{
    return wrapToType_(TypeString, &g_typeString);
}


static struct myTypeNil_ g_typeNil;
myType makeType_Nil(void)
{
    return wrapToType_(TypeNil, &g_typeNil);
}

static struct myTypeNoReturn_ g_typeNoReturn;
myType makeType_NoReturn(void)
{
    return wrapToType_(TypeNoReturn, &g_typeNoReturn);
}

////////////////////////////////////////////////////////////////////////////

myType makeType_Record(myTypeFieldList fields)
{
    static unsigned long long idCount = 0;
    //  check if `idCount` has overflowed(too many record types declared)
    if (++idCount == 0)     exit(ERROR_MAKETYPE);   

    myTypeRecord recordType = 
        makeMemoryBlock(sizeof(*recordType), MEMORY_TYPE_NONE);
    assert(recordType != NULL);

    recordType->uniqueId = idCount;
    recordType->fieldList = fields;

    return wrapToType_(TypeRecord, recordType);
} 

myType makeType_Array(myType type)
{
    static unsigned long long idCount = 0;
    //  check if `idCount` has overflowed(too many array types declared)
    if (++idCount == 0)     exit(ERROR_MAKETYPE); 

    myTypeArray arrayType =
        makeMemoryBlock(sizeof(*arrayType), MEMORY_TYPE_NONE);
    assert (arrayType != NULL);

    arrayType->uniqueId = idCount;
    arrayType->type = type;

    return wrapToType_(TypeArray, arrayType);
}

myType makeType_Named(mySymbol name, myType type)
{
    myTypeNamed namedType =
        makeMemoryBlock(sizeof(*namedType), MEMORY_TYPE_NONE);
    assert (namedType != NULL);

    namedType->name = name;
    namedType->type = type;

    return wrapToType_(TypeNamed, namedType);

}

////////////////////////////////////////////////////////////////////////

myTypeField makeType_TypeField(mySymbol name, myType type)
{
    myTypeField typeField = 
        makeMemoryBlock(sizeof(*typeField), MEMORY_TYPE_NONE);
    assert (typeField != NULL);
    
    typeField->name = name;
    typeField->type = type;

    return typeField;
}  

myTypeFieldList makeType_TypeFieldList(myTypeField head, myTypeFieldList tails)
{
    myTypeFieldList typeFieldList = 
        makeMemoryBlock(sizeof(*typeFieldList), MEMORY_TYPE_NONE);
    assert (typeFieldList != NULL);

    typeFieldList->head = head;
    typeFieldList->tails = tails;

    return typeFieldList;
}

myTypeList makeType_TypeList(myType head, myTypeList tails)
{
    myTypeList typeList = 
        makeMemoryBlock(sizeof(*typeList), MEMORY_TYPE_NONE);
    assert (typeList != NULL);

    typeList->head = head;
    typeList->tails = tails;

    return typeList;
}

/////////////////////////////////////////////////////////////////////////////