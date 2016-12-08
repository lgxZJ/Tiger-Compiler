#ifndef MY_CONTROLABLE_H
#define MY_CONTROLABLE_H

#include "myAAIs.h"

namespace lgxZJ
{
    namespace IS
    {

        class Label : public Controlable
        {
            public:
                explicit Label(myLabel oneLabel);

            private:
                myLabel label;
        };

        class Jmp : public Controlable
        {
            public:
                //      In fact, i only use Jump to directly jump to one place and
                //  i don't know the use of multi-jumps, so i just use one label to
                //  initialize it.
                //      If one day there is a need, we can use overload instead.
                explicit Jmp(myLabel oneLabel);

            private:
                myLabel label;
        };

#error "here, change CJump to real ones"
        class CJump : public Controlable
        {
            public:
                //      After former phase(Basic Blocks and Trace), Every CJump is followed
                //  its false label, so we can only keep the true label here.
                explicit CJump(myLabel oneTrueLabel);

            private:
                myLabel trueLabel;
        };

    }
}

#endif