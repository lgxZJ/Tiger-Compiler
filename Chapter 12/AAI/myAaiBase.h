#ifndef MY_AAI_H
#define MY_AAI_H

#include "../CXX_myTemp.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

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

        class AAI;

        typedef std::vector<std::shared_ptr<IS::AAI>>   Instructions;
        typedef std::vector<myTemp>                     Registers;
        typedef myTemp                                  Register;
        typedef std::map<Register, Register>            RegisterMap;

        struct BinaryUnion
        {
            enum Kind { Reg, Value }    kind;
            union{   myTemp reg; int value; }u;
        };

		class AAI
		{
		    public:
		        virtual std::string ToString() const = 0;
                virtual std::string ToCode(RegisterMap& map) const = 0;

                //  NOTE:
                //      Dst means the destination in arithmetic manner, not in assembly manner.
                //      For example:
                //          cmp eax, ebx
                //      In x86 manner, eax is the destination and ebx is the source. But in
                //      arithmetic, eax is not assigned by ebx, so eax is a source registers.
                //
                //      However, code like the following obeys the arithmetic order:
                //          add eax, ebx
                //      because code above could be written to ---- eax = eax + ebx. So, eax
                //      is really a destination register, and ebx is a source register.
                virtual Registers GetDstRegs() const = 0;
                virtual Registers GetSrcRegs() const = 0;
                 
                virtual myLabel GetDstLabel() const { return nullptr; }
                virtual myLabel GetDefLabel() const { return nullptr; }

                virtual void ReplaceReg(Register oldReg, Register newReg) {}
		};

        ////////////////////////////////////////////////////////////////////
        //              Abstract class for common computes
        ////////////////////////////////////////////////////////////////////

        class TwoOperandOperate : public AAI
        {
            protected:
                myTemp      dstReg;
                BinaryUnion srcRep;

                //  default converters
                std::string ToCommonString(std::string insStr) const;
                std::string ToCommonCode(std::string insStr, RegisterMap& maps) const;

            public:
                //  Two operand operate has the following instruction syntax:
                //      ins dstReg, srcReg(if register)
                //  which has the following arithmetic semantic:
                //      dstReg = dstReg op srcReg
                //  So,
                //      the Dst registers are----dstReg
                //      the Src registers are----dstReg, srcReg.
                virtual Registers GetDstRegs() const;
                virtual Registers GetSrcRegs() const;

                void ReplaceReg(Register oldReg, Register newReg);

            public:
                //  public for reuse in OneOperandOperate class
                static std::string OneRegToString(myTemp reg);
                static std::string OneRegToCode(myTemp reg, RegisterMap& maps);
                static std::string OneValueToCode(int value);
        };

        class OneOperandOperate : public AAI
        {
            protected:
                BinaryUnion srcRep;

                std::string ToCommonString(std::string insStr) const;
                std::string ToCommonCode(std::string insStr, RegisterMap& maps) const;

            public:
                //  One operand operate has the following instruction syntax:
                //      ins srcReg(if register)
                //  which has the following arithmetic semantic:
                //      eax = eax op srcReg,
                //      edx = eax op srcReg 
                //  So,
                //      the Dst registers are----eax, edx
                //      the Src registers are----eax, srcReg.
                virtual Registers GetDstRegs() const;
                virtual Registers GetSrcRegs() const;

                void ReplaceReg(Register oldReg, Register newReg);
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