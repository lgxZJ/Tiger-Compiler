#ifndef MY_ARITHMETIC_H
#define MY_ARITHMETIC_H

#include "myAaiBase.h"

namespace lgxZJ
{
    namespace IS
    {
        //////////////////////////////////////////////////////////////////
        //              Signed arithmetic instructions
        //////////////////////////////////////////////////////////////////

        class Add : public TwoOperandOperate, public Computable
        {
            public:
                explicit Add(myTemp oneDstReg, myTemp oneSrcReg);
                explicit Add(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };

        class Sub : public TwoOperandOperate, public Computable
        {
            public:
                explicit Sub(myTemp oneDstReg, myTemp oneSrcReg); 
                explicit Sub(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };

        //  Default destination register : eax
        //  Default result registers : edx, eax
        class IMul : public OneOperandOperate, public Computable
        {
            public:
                explicit IMul(myTemp oneSrcReg); 
                explicit IMul(int constValue);

                virtual std::string ToString() const;
                virtual myTempList TrashedRegs() const;
        };

        //  Default destination registers : edx, eax
        //  Default result registers : eax, edx
        class IDiv : public OneOperandOperate, public Computable
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

        class Xor: public TwoOperandOperate, public Computable
        {
            public:
                explicit Xor(myTemp oneDstReg, myTemp oneSrcReg);
                explicit Xor(myTemp oneDstReg, int constValue);

                virtual std::string ToString() const;
        };
    }
}

#endif