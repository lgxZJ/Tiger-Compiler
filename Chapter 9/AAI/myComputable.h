#ifndef MY_ARITHMETIC_H
#define MY_ARITHMETIC_H

#include "myAaiBase.h"

namespace lgxZJ
{
    namespace IS
    {
        ////////////////////////////////////////////////////////////////////
        //              Abstract class for common computes
        ////////////////////////////////////////////////////////////////////

        class TwoOperandCompute : public Computable
        {
            protected:
                myTemp      dstReg;
                BinaryUnion srcRep;

                //  default converter.
                std::string ToCommonString(std::string insStr) const;

            private:
                std::string OneRegToString(myTemp reg) const;
        };

        class OneOperandCompute : public Computable
        {
            protected:
                BinaryUnion srcRep;

                std::string ToCommonString(std::string insStr) const;
        };

        //////////////////////////////////////////////////////////////////
        //              Signed arithmetic instructions
        //////////////////////////////////////////////////////////////////

        class Add : public TwoOperandCompute
        {
            public:
                explicit Add(myTemp oneDstReg, myTemp oneSrcReg);
                explicit Add(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };

        class Sub : public TwoOperandCompute
        {
            public:
                explicit Sub(myTemp oneDstReg, myTemp oneSrcReg); 
                explicit Sub(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };

        //  Default destination register : eax
        //  Default result registers : edx, eax
        class IMul : public OneOperandCompute
        {
            public:
                explicit IMul(myTemp oneSrcReg); 
                explicit IMul(int constValue);

                virtual std::string ToString() const;
                virtual myTempList TrashedRegs() const;
        };

        //  Default destination registers : edx, eax
        //  Default result registers : eax, edx
        class IDiv : public OneOperandCompute
        {
            public:
                explicit IDiv(myTemp oneSrcReg); 
                explicit IDiv(int constValue);

                virtual std::string ToString() const;
                virtual myTempList TrashedRegs() const;
        };

        ///////////////////////////////////////////////////////////////////////
        //                          Logical instructions
        ///////////////////////////////////////////////////////////////////////

        class Xor: public TwoOperandCompute
        {
            public:
                explicit Xor(myTemp oneDstReg, myTemp oneSrcReg);
                explicit Xor(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };
    }
}

#endif