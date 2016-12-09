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

        class CommonTwoCompute : public Computable
        {
            protected:
                myTemp      dstReg;
                BinaryUnion srcRep;

                //  default converter.
                std::string ToCommonString(std::string insStr) const;

            private:
                std::string OneRegToString(myTemp reg) const;
        };

        class CommonOneCompute : public Computable
        {
            private:
                BinaryUnion srcRep;
            //  todo:
        };

        //////////////////////////////////////////////////////////////////
        //              Signed arithmetic instructions
        //////////////////////////////////////////////////////////////////

        class Add : public CommonTwoCompute
        {
            public:
                explicit Add(myTemp oneDstReg, myTemp oneSrcReg);
                explicit Add(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };

        class Sub : public CommonTwoCompute
        {
            public:
                explicit Sub(myTemp oneDstReg, myTemp oneSrcReg); 
                explicit Sub(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };

        //  Default destination register : eax
        //  Default result registers : edx, eax
        class IMul : public CommonOneCompute
        {
            public:
                explicit IMul(myTemp oneSrcReg); 
                explicit IMul(int constValue);

                virtual std::string ToString() const;

            private:
                virtual myTempList TrashedRegs();
        };

        //  Default destination registers : edx, eax
        //  Default result registers : eax, edx
        class IDiv : public CommonOneCompute
        {
            public:
                explicit IDiv(myTemp oneSrcReg); 
                explicit IDiv(int constValue);

                virtual std::string ToString() const;

            private:
                virtual myTempList TrashedRegs();
        };

        ///////////////////////////////////////////////////////////////////////
        //                          Logical instructions
        ///////////////////////////////////////////////////////////////////////

        class Xor: public CommonTwoCompute
        {
            public:
                explicit Xor(myTemp oneDstReg, myTemp oneSrcReg);
                explicit Xor(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };
    }
}

#endif