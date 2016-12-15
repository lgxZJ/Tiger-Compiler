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

        Registers TwoOperandOperate::GetDstRegs() const
        {
            return Registers(1, dstReg);
        }

        Registers TwoOperandOperate::GetSrcRegs() const
        {
            Registers result = GetDstRegs();            

            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result.push_back(srcRep.u.reg);

            return result;
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

        Registers OneOperandOperate::GetDstRegs() const
        {
            //  default destination register
            return Registers(1, Frame_EAX());
        }

        Registers OneOperandOperate::GetSrcRegs() const
        {
            Registers result = GetDstRegs();

            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result.push_back(srcRep.u.reg);
            
            return result;
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