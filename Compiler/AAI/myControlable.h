#ifndef MY_CONTROLABLE_H
#define MY_CONTROLABLE_H

#include "myAaiBase.h"
#include "../CXX_myFrame.h"

namespace lgxZJ
{
    namespace IS
    {
        class CommonJump : public AAI
        {
            public:
                std::string ToCommonString(std::string ins) const
                {
                    return ins + " " + std::string(MySymbol_GetName(dstLabel));
                }

                virtual Registers GetDstRegs() const { return {}; }
                virtual Registers GetSrcRegs() const { return {}; }
                virtual myLabel GetDstLabel() const { return dstLabel; }

            protected:
                myLabel dstLabel;
        };

        //////////////////////////////////////////////////////////////////////////
        //                          Label definition
        //////////////////////////////////////////////////////////////////////////

        class Label : public AAI
        {
            public:
                explicit Label(myLabel oneLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;

                virtual Registers GetDstRegs() const { return {}; }
                virtual Registers GetSrcRegs() const { return {}; }

                virtual myLabel GetDefLabel() const { return label; }

            protected:
                myLabel label;
        };

        //////////////////////////////////////////////////////////////////////////
        //                      Compare instructions
        //////////////////////////////////////////////////////////////////////////

        class Cmp : public TwoOperandOperate
        {
            public:
                explicit Cmp (myTemp oneDstReg, myTemp oneSrcReg);
                explicit Cmp (myTemp oneDstReg, int constValue);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;

                virtual Registers GetDstRegs() const;
                virtual Registers GetSrcRegs() const;
        };

        ///////////////////////////////////////////////////////////////////////////////
        //      Uncondition jump
        //  REMARK:
        //          In fact, i only use Jump to directly jump to one place and
        //      i don't know the use of multi-jumps, so i just use one label to
        //      initialize it.
        //          If one day there is a need, we can use overload instead.
        //////////////////////////////////////////////////////////////////////////////

        class Jmp : public CommonJump
        {
            public:
                explicit Jmp(myLabel oneDstLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;
        };

        /////////////////////////////////////////////////////////////////////////////////
        //  Condition jumps:
        //          After former phase(Basic Blocks and Trace), Every CJump is followed
        //      its false label, so we can only keep the true label here.
        /////////////////////////////////////////////////////////////////////////////////

        //      Signed comparasion jumps
        class Je : public CommonJump
        {
            public:
                explicit Je(myLabel oneDstLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;
        };

        class Jne : public CommonJump
        {
            public:
                explicit Jne(myLabel oneDstLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;
        };

        class Jg : public CommonJump
        {
            public:
                explicit Jg(myLabel oneDstLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;
        };

        class Jge : public CommonJump
        {
            public:
                explicit Jge(myLabel oneDstLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;
        };

        class Jl : public CommonJump
        {
            public:
                explicit Jl(myLabel oneDstLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;
        };

        class Jle : public CommonJump
        {
            public:
                explicit Jle(myLabel oneDstLabel);

                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;
        };

        //      Tiger do not permit unsigned comparison jumps.

        ////////////////////////////////////////////////////////////////////////
        //                          Function calls
        ////////////////////////////////////////////////////////////////////////

        class Call : public AAI
        {
            public:
                explicit Call (myString oneFuncName, myTempList oneRegList);

                virtual Registers GetDstRegs() const;
                virtual Registers GetSrcRegs() const;

                virtual void ReplaceReg(Register oldReg, Register newReg);
                std::string ToString() const;
                std::string ToCode(RegisterMap& map) const;

            protected:
                    myString funcName;
                    myTempList regList;
        };
    }
}

#endif