#ifndef MY_TYPES_H
#define MY_TYPES_H

#include "mySymbol.h"

/**-----------------------------------------------------------------
 *                      structures of types
 ------------------------------------------------------------------*/

//  forward declaration
struct myType_;
typedef struct myType_ *myType;

typedef struct myTypeField_{
    mySymbol    name;
    myType      type;
}*  myTypeField;

typedef struct myTypeFieldList_{
    myTypeField         head;
    struct myTypeFieldList_    *tails;
}*  myTypeFieldList;



typedef struct myTypeInt_{
    //  nothing
}*  myTypeInt;


typedef struct myTypeString_{
    //  nothing
}*  myTypeString;


typedef struct myTypeNil_{
    //  nothing
}*  myTypeNil;


typedef struct myTypeNoReturn_{
    //  nothing
}*  myTypeNoReturn;


typedef struct myTypeRecord_{
    //  Each declaretion of a record creates a new type
    //  even if all the fields are similar.
    unsigned long long          uniqueId;
    myTypeFieldList             fieldList;
}*  myTypeRecord;

typedef struct myTypeArray_{
    //  Each declaretion of an array creates a new type
    //  even if all the fields are similar.
    unsigned long long  uniqueId;
    myType              type;
}*  myTypeArray;

typedef struct myTypeNamed_{
    mySymbol    name;
    myType      type;
}*  myTypeNamed;


struct myType_
{
    enum TypeKind 
    { 
        TypeInt, TypeString, TypeNil, TypeNoReturn,
        TypeRecord, TypeArray, TypeNamed, TypeEmpty
    }kind;

    union 
    {
        myTypeInt       typeInt;
        myTypeString    typeString;
        myTypeNil       typeNil;
        myTypeNoReturn  typeNoReturn;
        myTypeRecord    typeRecord;
        myTypeArray     typeArray;
        myTypeNamed     typeNamed;
    }u;

};

typedef struct myTypeList_{
    myType              head;
    struct myTypeList_* tails;
}*  myTypeList;


#endif
