#include "myAaiBase.h"
#include "../CXX_myFrame.h"

#include <cassert>

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

        void TwoOperandOperate::ReplaceReg(Register oldReg, Register newReg)
        {
            if ( dstReg == oldReg )
                dstReg = newReg;
            if ( srcRep.kind == BinaryUnion::Kind::Reg &&
                 srcRep.u.reg == oldReg )

                srcRep.u.reg = newReg;
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

        string TwoOperandOperate::ToCommonCode(string insStr, RegisterMaps map) const
        {
            string result(insStr + " ");
            result += OneRegToCode(dstReg, map);

            result += ", ";
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result += OneRegToCode(srcRep.u.reg, map);
            else
                result += to_string(srcRep.u.value);

            return result;
        }

        //////////////////////////////////////////////////////////////////

        string TwoOperandOperate::OneRegToString(myTemp reg)
        {
            return "register" + to_string(Temp_getTempNum(reg));
        }

        string TwoOperandOPerate::OneRegToCode(myTemp reg, RegisterMap& map)
        {
            myTemp realReg = map.at(reg);

            if (realReg == Frame_EAX())         return string("eax");
            else if (realReg == Frame_EBX())    return string("ebx");
            else if (realReg == Frame_ECX())    return string("ecx");
            else if (realReg == Frame_EDX())    return string("edx");
            else if (realReg == Frame_ESI())    return string("esi");
            else if (realReg == Frame_EDI())    return string("edi");
            else if (realReg == Frame_EBP())    return string("ebp");
            else                                assert (false);
        }

        ///////////////////////////////////////////////////////////////
        //                      OneOperandOperate
        ///////////////////////////////////////////////////////////////

        Registers OneOperandOperate::GetDstRegs() const
        {
            //  default destination register
            Registers result(1, Frame_EAX());
            result.push_back(Frame_EDX());

            return result;
        }

        Registers OneOperandOperate::GetSrcRegs() const
        {
            Registers result(1, Frame_EAX());

            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result.push_back(srcRep.u.reg);
            
            return result;
        }

        void OneOperandOperate::ReplaceReg(Register oldReg, Register newReg)
        {
            if ( srcRep.kind == BinaryUnion::Kind::Reg &&
                 srcRep.u.reg == oldReg )

                 srcRep.u.reg = newReg;
        }

        string OneOperandOperate::ToCommonString(string insStr) const
        {
            string result(insStr + " ");
            
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result += TwoOperandOperate::OneRegToString(srcRep.u.reg);
            else
                result += to_string(srcRep.u.value);

            return result;
        }

        string OneOperandOperate::ToCommonCode(string insStr, RegisterMap& map) const
        {
            string result(insStr + " ");
            
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result += "register" + TwoOperandOperate::OneRegToCode(srcRep.u.reg, map));
            else
                result += to_string(srcRep.u.value);

            return result;
        }
    }
}