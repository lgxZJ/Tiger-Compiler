#ifndef MY_CONTROLABLE_H
#define MY_CONTROLABLE_H

#include "myAaiBase.h"

namespace lgxZJ
{
    namespace IS
    {
        class CommonJump : public Controlable
        {
            protected:
                CommonJump(myLabel oneDstLabel);

            private:
                myLabel dstLabel;
        };
#error "here"
        //////////////////////////////////////////////////////////////////////////
        //                          Label definition
        //////////////////////////////////////////////////////////////////////////

        class Label : public Controlable
        {
            public:
                explicit Label(myLabel oneLabel);

            private:
                myLabel label;
        };

        //////////////////////////////////////////////////////////////////////////
        //                      Compare instructions
        //////////////////////////////////////////////////////////////////////////

        class Cmp : public AAI
        {
            public:
                explicit Cmp (myTemp dstReg, myTemp srcReg);
                explicit Cmp (myTemp dstReg, int constValue);

            private:
                myTemp      dstReg;
                BinaryUnion srcRep;
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
        };

        class Jne : public CommonJump
        {
            public:
                explicit Jne(myLabel oneDstLabel);
        };

        class Jg : public CommonJump
        {
            public:
                explicit Jg(myLabel oneDstLabel);
        };

        class Jge : public CommonJump
        {
            public:
                explicit Jge(myLabel oneDstLabel);
        };

        class Jl : public CommonJump
        {
            public:
                explicit Jl(myLabel oneDstLabel);
        };

        class Jle : public CommonJump
        {
            public:
                explicit Jle(myLabel oneDstLabel);
        };

        //      Tiger do not permit unsigned comparison jumps.

        ////////////////////////////////////////////////////////////////////////
        //                          Function calls
        ////////////////////////////////////////////////////////////////////////

        class Call : public Controlable
        {
            public:
                explicit Call (myLabel funcLabel, myTempList regList);

            private:
                static myTempList TrashedRegs();
        }
    }
}

#endif