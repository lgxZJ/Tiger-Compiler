#ifndef MY_AAI_H
#define MY_AAI_H

#include "../CXX_myTemp.h"
#include <string>

namespace lgxZJ
{
    namespace IS    //  stand for Instruction Selection phase
    {
        ///////////////////////////////////////////////////////////////////////
        //  NOTE:
        //      Almost every instructions in X86 have 5 syntax:
        //          1.Register  -   Register
        //          2.Register  -   Memory
        //          3.Register  -   ConstValue
        //          4.Memory    -   Register
        //          5.Memory    -   ConstValue
        //      But, only some of them are implemented in this module because
        //      not all of them are needed in tiger.
        ///////////////////////////////////////////////////////////////////////

        struct BinaryUnion
        {
            enum Kind { Reg, Value }    kind;
            union{   myTemp reg; int value; }u;
        };

		class AAI
		{
		    public:
		        virtual std::string ToString() const = 0;

                //  Default definition
                virtual myTempList TrashedRegs() const { return nullptr; }
		};

        class Controlable : public AAI  {};
        class Computable :  public AAI  {};
        class Movable :     public AAI  {};

        ////////////////////////////////////////////////////////////////////
        //              Abstract class for common computes
        ////////////////////////////////////////////////////////////////////

        class TwoOperandOperate
        {
            protected:
                myTemp      dstReg;
                BinaryUnion srcRep;

                //  default converter.
                std::string ToCommonString(std::string insStr) const;

            private:
                std::string OneRegToString(myTemp reg) const;
        };

        class OneOperandOperate
        {
            protected:
                BinaryUnion srcRep;

                std::string ToCommonString(std::string insStr) const;
        };
	}
}

#endif