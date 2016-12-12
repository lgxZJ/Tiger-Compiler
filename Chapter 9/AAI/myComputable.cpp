#include "myComputable.h"
#include "../CXX_myFrame.h"


using namespace std;

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

namespace lgxZJ
{
    namespace IS
    {
        //////////////////////////////////////////////////////////////
        //                      Add class
        //////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Add);

        string Add::ToString() const
        {
            return ToCommonString("add");
        }

        //////////////////////////////////////////////////////////////////
        //                          Sub class
        //////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Sub);

        string Sub::ToString() const
        {
            return  ToCommonString("sub");
        }

        //////////////////////////////////////////////////////////////////
        //                          IMul class
        //////////////////////////////////////////////////////////////////

        DEFINE_ONECOMPUTE_CTOR(IMul);

        string IMul::ToString() const
        {
            return ToCommonString("imul");
        }

        myTempList IMul::TrashedRegs() const
        {
            //  When multiply, high order 32-bits are stored in `edx` register.
            return Temp_makeTempList(Frame_EDX(), nullptr);
        }

        ////////////////////////////////////////////////////////////////////
        //                          IDiv class
        ////////////////////////////////////////////////////////////////////

        DEFINE_ONECOMPUTE_CTOR(IDiv);

        string IDiv::ToString() const
        {
            return ToCommonString("idiv");
        }

        myTempList IDiv::TrashedRegs() const
        {
            //  When multiply, low order 32-bits(namely remainder) are stored in `edx` register.
            return Temp_makeTempList(Frame_EDX(), nullptr);
        }

        ////////////////////////////////////////////////////////////////////
        //                          Xor class
        ////////////////////////////////////////////////////////////////////

        DEFINE_TWOCOMPUTE_CTOR(Xor);

        string Xor::ToString() const
        {
            return ToCommonString("xor");
        }
    }
}