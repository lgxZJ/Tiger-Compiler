#ifndef MY_AAI_H
#define MY_AAI_H

#include "../CXX_myTemp.h"
#include <string>
#include <vector>

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

        typedef std::vector<myTemp>     Registers;

        struct BinaryUnion
        {
            enum Kind { Reg, Value }    kind;
            union{   myTemp reg; int value; }u;
        };

		class AAI
		{
		    public:
		        virtual std::string ToString() const = 0;

                virtual Registers GetDstRegs() const = 0;
                virtual Registers GetSrcRegs() const = 0;

                //  Default definition
                virtual myLabel GetLabel() const { return nullptr; }
                virtual myTempList TrashedRegs() const { return nullptr; }
		};

        ////////////////////////////////////////////////////////////////////
        //              Abstract class for common computes
        ////////////////////////////////////////////////////////////////////

        class TwoOperandOperate : public AAI
        {
            protected:
                myTemp      dstReg;
                BinaryUnion srcRep;

                //  default converter.
                std::string ToCommonString(std::string insStr) const;

            public:
                virtual Registers GetDstRegs() const;
                virtual Registers GetSrcRegs() const;

            private:
                std::string OneRegToString(myTemp reg) const;
        };

        class OneOperandOperate : public AAI
        {
            protected:
                BinaryUnion srcRep;

                std::string ToCommonString(std::string insStr) const;

            public:
                virtual Registers GetDstRegs() const;
                virtual Registers GetSrcRegs() const;
        };

#define DEFINE_TWOCOMPUTE_CTOR(CLS)                     \
        CLS::CLS(myTemp oneDstReg, myTemp oneSrcReg)    \
        {                                               \
            dstReg = oneDstReg;                         \
                                                        \
            srcRep.kind = BinaryUnion::Kind::Reg;       \
            srcRep.u.reg = oneSrcReg;                   \
        }                                               \
                                                        \
        CLS::CLS(myTemp oneDstReg, int constValue)      \
        {                                               \
            dstReg = oneDstReg;                         \
                                                        \
            srcRep.kind = BinaryUnion::Kind::Value;     \
            srcRep.u.value = constValue;                \
        }                                   

#define DEFINE_ONECOMPUTE_CTOR(CLS)                     \
        CLS::CLS(myTemp oneSrcReg)                      \
        {                                               \
            srcRep.kind = BinaryUnion::Kind::Reg;       \
            srcRep.u.reg = oneSrcReg;                   \
        }                                               \
                                                        \
        CLS::CLS(int constValue)                        \
        {                                               \
            srcRep.kind = BinaryUnion::Kind::Value;     \
            srcRep.u.value = constValue;                \
        }
	}
}

#endif