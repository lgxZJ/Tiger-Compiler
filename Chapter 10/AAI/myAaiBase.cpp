#include "myAaiBase.h"
#include "../CXX_myFrame.h"

using namespace std;

namespace lgxZJ
{
    namespace IS
    {
        //////////////////////////////////////////////////////////////
        //                      TwoOperandOperate
        //////////////////////////////////////////////////////////////

        myTemp TwoOperandOperate::GetDstReg() const
        {
            return dstReg;
        }

        myTemp TwoOperandOperate::GetSrcReg() const
        {
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                return srcRep.u.reg;
            else
                return nullptr;
        }

        string TwoOperandOperate::ToCommonString(string insStr) const
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

        string TwoOperandOperate::OneRegToString(myTemp reg) const
        {
            return "register" + to_string(Temp_getTempNum(reg));
        }

        ///////////////////////////////////////////////////////////////
        //                      OneOperandOperate
        ///////////////////////////////////////////////////////////////

        myTemp OneOperandOperate::GetDstReg() const
        {
            //  default destination register
            return Frame_EAX();
        }

        myTemp OneOperandOperate::GetSrcReg() const
        {
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                return srcRep.u.reg;
            else
                return nullptr;
        }

        string OneOperandOperate::ToCommonString(string insStr) const
        {
            string result(insStr + " ");
            
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result += "register" + to_string(Temp_getTempNum(srcRep.u.reg));
            else
                result += to_string(srcRep.u.value);

            return result;
        }

    }
}