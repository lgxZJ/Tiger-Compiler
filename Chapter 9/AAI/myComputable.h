#ifndef MY_ARITHMETIC_H
#define MY_ARITHMETIC_H

#include "myAAIs.h"

namespace lgxZJ
{
    namespace IS
    {
        class Add : public Computable
        {
            public:
                explicit Add(myTemp dst, int constValue);
                explicit Add(myTemp dst, myTemp src);
                 
        }
    }
}

#endif