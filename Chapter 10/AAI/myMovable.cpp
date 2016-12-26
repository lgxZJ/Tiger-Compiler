#include "myMovable.h"

using namespace std;

namespace lgxZJ
{
    namespace IS
    {
        Move::Move( myTemp oneDstReg, myTemp oneSrcReg,
                    OperandType oneDstType, OperandType oneSrcType)
            :   dstReg(oneDstReg), dstType(oneDstType)
        {
            srcType = oneSrcType;

            srcRep.kind = BinaryUnion::Kind::Reg;
            srcRep.u.reg = oneSrcReg;
        }

        Move::Move(myTemp oneDstReg, OperandType oneDstType, int constValue)
            :   dstReg(oneDstReg), dstType(oneDstType)
        {
            srcType = OperandType::None;

            srcRep.kind = BinaryUnion::Kind::Value;
            srcRep.u.value = constValue;
        }

        Registers Move::GetDstRegs() const
        {
            if (dstType == OperandType::Content)
                return Registers(1, dstReg);
            else
                return {};
        }

        Registers Move::GetSrcRegs() const
        {
            Registers result;

            if (dstType == OperandType::Memory)
                result.push_back(dstReg);
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result.push_back(srcRep.u.reg);

            return result;
        }

        string Move::ToString() const
        {
            string result("mov ");

            result += OneOperandToString(dstReg, dstType);

            result += ", ";
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                result += OneOperandToString(srcRep.u.reg, srcType);
            else
                result += to_string(srcRep.u.value);

            return result;
        }

        string Move::OneOperandToString(myTemp operand, OperandType type) const
        {
            string operandRegStr = "register" + to_string(Temp_getTempNum(operand));

            if (type == OperandType::Content)
                return operandRegStr;
            else
                return "[" + operandRegStr + "]";
        }
    }
}