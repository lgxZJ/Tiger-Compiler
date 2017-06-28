#include "myMovable.h"

using namespace std;

namespace lgxZJ
{
    namespace IS
    {
        Move::Move( myTemp oneDstReg, myTemp oneSrcReg,
                    OperandType oneDstType, OperandType oneSrcType)
            :   dstReg(oneDstReg), dstType(oneDstType), isLabel(false)
        {
            srcType = oneSrcType;

            srcRep.kind = BinaryUnion::Kind::Reg;
            srcRep.u.reg = oneSrcReg;
        }

        Move::Move(myTemp oneDstReg, OperandType oneDstType, int constValue)
            :   dstReg(oneDstReg), dstType(oneDstType), isLabel(false)
        {
            srcType = OperandType::None;

            srcRep.kind = BinaryUnion::Kind::Value;
            srcRep.u.value = constValue;
        }

        Move::Move(myTemp oneDstReg, myLabel strLabel)
            :   dstReg(oneDstReg), dstType(OperandType::Content)
            ,   stringLabel(strLabel), isLabel(true)
        {
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
            if (!isLabel && srcRep.kind == BinaryUnion::Kind::Reg)
                result.push_back(srcRep.u.reg);

            return result;
        }

        void Move::ReplaceReg(Register oldReg, Register newReg)
        {
            if ( dstReg == oldReg )
                dstReg = newReg;
            if ( !isLabel &&
                 srcRep.kind == BinaryUnion::Kind::Reg && 
                 srcRep.u.reg == oldReg )

                srcRep.u.reg = newReg;
        }

        string Move::ToString() const
        {
            string result("\tmovl ");

            result += OneOperandToString(dstReg, dstType);

            result += ", ";
            if (isLabel)
            {
                result += (string("$") + Temp_getLabelString(stringLabel));
            }
            else
            {
                if (srcRep.kind == BinaryUnion::Kind::Reg)
                    result += OneOperandToString(srcRep.u.reg, srcType);
                else
                    result += to_string(srcRep.u.value);
            }

            return result;
        }

        string Move::ToCode(RegisterMap& map) const
        {
            string result("\tmovl ");

            if (isLabel)
            {
                result += (string("$") + Temp_getLabelString(stringLabel));
            }
            else
            {
                if (srcRep.kind == BinaryUnion::Kind::Reg)
                    result += OneOperandToCode(srcRep.u.reg, srcType, map);
                else
                    result += TwoOperandOperate::OneValueToCode(srcRep.u.value);
            }
            
            result += ", ";
            result += OneOperandToCode(dstReg, dstType, map);
            return result;
        }


        string Move::OneOperandToString(myTemp operand, OperandType type) const
        {
            return OneOperandTo(type, TwoOperandOperate::OneRegToString(operand));
        }

        string Move::OneOperandToCode(myTemp operand, OperandType type, RegisterMap& map) const
        {
            return OneOperandTo(type, TwoOperandOperate::OneRegToCode(operand, map));
        }

        std::string Move::OneOperandTo(OperandType type, std::string operandRegStr) const
        {
            if (type == OperandType::Content)
                return operandRegStr;
            else
                return "(" + operandRegStr + ")";
        }
    }
}