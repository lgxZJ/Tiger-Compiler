#include "myComputable.h"

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

namespace lgxZJ
{
    namespace IS
    {
        //////////////////////////////////////////////////////////////
        //                      CommonTwoCompute
        //////////////////////////////////////////////////////////////

        string CommonTwoCompute::ToCommonString(string insStr) const
        {
            string result(insStr + " ");
            result += OneRegToString(dstReg);

            result += ", ";
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result += OneRegToString(srcRep.u.reg);
            else
                result += to_string(srcRep.u.value);

            return result;
        }

        string CommonTwoCompute::OneRegToString(myTemp reg) const
        {
            return "register" + to_string(Temp_getTempNum(reg));
        }

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

        
    }
}