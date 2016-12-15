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

        myTemp Move::GetDstReg() const
        {
            return dstReg;
        }

        myTemp Move::GetSrcReg() const
        {
            if (srcRep.kind == BinaryUnion::Kind::Reg)
                return srcRep.u.reg;
            else
                return nullptr;
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