#ifndef MY_AAI_H
#define MY_AAI_H

#include "CXX_myTemp.h"
#include <string>

namespace lgxZJ
{
    namespace IS    //  stand for Instruction Selection phase
    {

		//  REMARK:
        //      Derived class should throw exception when calling
        //      `Getters` if there are actually no such stuffs.
		class AAI
		{
		    public:
		        virtual std::string ToString() = 0;
                
                virtual myLabel GetLabel() = 0;
		        virtual myTemp GetDstReg() = 0;
		        virtual myTemp GetSrcReg() = 0;
                virtual int GetConstValue() = 0;
		};

        class Controlable : public AAI  {};

        //  REMARK:
        //      Movable instructions have 5 syntax:
        //          1.Register  -   Register
        //          2.Register  -   Memory
        //          3.Register  -   ConstValue
        //          4.Memory    -   Register
        //          5.Memory    -   ConstValue
        class Movable : public AAI {};

        //  REMARK:
        //      Computable instructions have 5 syntax:
        //          1.Register  -   Register
        //          2.Register  -   Memory
        //          3.Register  -   ConstValue
        //          4.Memory    -   Register
        //          5.Memory    -   ConstValue
        class Computable : public AAI {};
	}
}

#endif